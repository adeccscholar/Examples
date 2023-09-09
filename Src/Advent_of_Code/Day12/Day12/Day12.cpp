#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <set>
#include <utility>
#include <optional>
#include <algorithm>


using namespace std::literals::string_literals;


static constexpr auto input = [](std::string const& strFile) {
   std::ifstream ifs;
   ifs.open(strFile);
   if (!ifs.is_open()) {
      std::ostringstream os;
      os << "File " << strFile << " couldn't opened.";
      throw std::runtime_error(os.str());
   }
   return ifs;
};


class TRoute {

   friend void swap(TRoute& lhs, TRoute& rhs) noexcept {
      std::swap(lhs.the_current, rhs.the_current);
      std::swap(lhs.the_pathway, rhs.the_pathway);
   }

public:
   enum class EDirection : char { right, up, left, down };

   // chessboard topology
   using topography_value = char;
   using topography_row = std::vector<topography_value>;
   using topography_grid = std::vector<topography_row>;
   using topography_pos = std::pair<size_t, size_t>;
   using topography_path = std::vector<topography_pos>;

   using path_row = std::vector<std::optional<size_t>>;
   using path_grid = std::vector<path_row>;

private:
   topography_pos  the_current;
   topography_path the_pathway;

   static topography_pos the_start_pos, the_end_pos;
   static topography_grid the_grid;

public:
   TRoute() {
      the_current = { 0u, 0u };
      }

   TRoute(TRoute& ref) {
      the_current = ref.the_current;
      the_pathway = ref.the_pathway;
      }

   TRoute(TRoute&& ref) noexcept {
      std::swap(the_current, ref.the_current);
      std::swap(the_pathway, ref.the_pathway);
   }

   TRoute(topography_path const& until, topography_pos start) {
      the_current = start;
      the_pathway = until;
      the_pathway.emplace_back(start);
      }

   ~TRoute() = default;

   TRoute& operator = (TRoute& ref) {
      the_current = ref.the_current;
      the_pathway = ref.the_pathway;
      return *this;
      }

   /*
   TRoute& operator = (TRoute&& ref) noexcept {
      std::swap(the_current, ref.the_current);
      std::swap(the_pathway, ref.the_pathway);
      return *this;
      }
      */

   static topography_pos& start_pos() { return the_start_pos; }
   static topography_pos& end_pos() { return the_end_pos; }
   static topography_grid& grid() { return the_grid; }

   static size_t max_x() { return the_grid[0].size();  }
   static size_t max_y() { return the_grid.size(); }

   topography_pos& current() { return the_current; }
   topography_path& pathway() { return the_pathway; }
   topography_pos const& current() const { return the_current; }
   topography_path const& pathway() const { return the_pathway; }

   static void reset_grid() {
      the_start_pos = { 0u, 0u };
      the_end_pos = { 0u, 0u };
      for (auto& row : the_grid) row.clear();
      the_grid.clear();
      }

   static auto grid_element(topography_pos const& pos) {
      return the_grid[pos.second][pos.first];
      }

   void step(topography_pos const& pos) {
      the_pathway.emplace_back(pos);
      the_current = pos;
      }

   void start(topography_pos const& pos) {
      the_start_pos = pos;
      the_pathway.clear();
      step(pos);
      }

   std::pair<topography_pos, bool> go_to(EDirection direction) const {
      topography_pos max = { max_x(), max_y() };
      topography_pos pos = the_current;
      bool retval = false;

      switch(direction) {
      case EDirection::right:
         if(the_current.first < max.first - 1) {
            ++pos.first;
            retval = true;
            }
         break;
      case EDirection::up:
         if (the_current.second > 0) {
            --pos.second;
            retval = true;
         }

         break;
      case EDirection::left:
         if (the_current.first > 0) {
            --pos.first;
            retval = true;
         }
         break;
      case EDirection::down:
         if (the_current.second < max.second - 1) {
            ++pos.second;
            retval = true;
         }
         break;
      default: throw std::runtime_error("unexpected direction in function go_to"s);
         }
      return { pos, retval };
      }

   std::set<topography_pos> possible_steps() const {
      std::set<topography_pos> steps;
      auto current_height = grid_element(the_current);
      for(auto dir : { EDirection::down, EDirection::right, EDirection::up, EDirection::left }) {
         if(auto [next_pos, has_moved] = go_to(dir); has_moved) {
            auto next_height = grid_element(next_pos);
            if(next_height -1 <= current_height) {
               if(std::find(the_pathway.begin(), the_pathway.end(), next_pos) == the_pathway.end()) {
                  steps.insert(next_pos);
                  }
               }
            }
         }
      return steps;
      }

};

TRoute::topography_pos TRoute::the_start_pos = { 0u, 0u };
TRoute::topography_pos TRoute::the_end_pos = { 0u, 0u };
TRoute::topography_grid TRoute::the_grid;

TRoute::topography_pos operator + (TRoute::topography_pos const& lhs, TRoute::topography_pos const& rhs) {
   TRoute::topography_pos sum = lhs;
   sum.first += rhs.first;
   sum.second += rhs.second;
   return sum;
   }

std::ostream& operator << (std::ostream& out, TRoute::topography_pos const& pos) {
   out << "( " << std::setw(2) << pos.first << ", " << std::setw(2) << pos.second << ")";
   return out;
   }

std::pair<TRoute, bool> MakeSteps(TRoute&& route, TRoute::path_grid& distances) {
   if (route.current() == route.end_pos()) {
      return { route, true };
   }

   auto& pos = distances[route.current().second][route.current().first];
   if (!pos) pos = route.pathway().size();
   else {
      if (*pos > route.pathway().size()) pos = route.pathway().size();
      else return { route, false };
   }

   auto next = route.possible_steps();
   if (next.size() == 0) {
      return { route, false };
   }

   std::optional<size_t> counter = std::nullopt;
   TRoute retval;
   for (auto& val : next) {
      auto [alternative, success] = MakeSteps({ route.pathway(), val }, distances);
      if (success) {
         if (!counter) {
            counter = alternative.pathway().size();
            retval = alternative;
         }
         else if (alternative.pathway().size() < *counter) {
            counter = alternative.pathway().size();
            retval = alternative;
         }
      }
   }

   if (counter) return { std::move(retval), true };
   else return { std::move(retval), false };
   }

std::pair<TRoute, TRoute::topography_pos> ReadValues(std::string const& strFilename) {
   TRoute::reset_grid();
   TRoute route;
   TRoute::topography_pos starting_here;
   auto ifs = input(strFilename);
   size_t x_pos = 0u, y_pos = 0u;
   std::string strRow;
   while (std::getline(ifs, strRow)) {
      TRoute::topography_row current;
      x_pos = 0u;
      for (auto c : strRow) {
         switch (c) {
         case 'S':
            starting_here = { x_pos, y_pos };
            current.emplace_back('a');
            break;
         case 'E':
            route.end_pos() = { x_pos, y_pos };
            current.emplace_back('z');
            break;
         default:
            if (c < 'a' || c > 'z') throw std::runtime_error("unexpected input for file \""s + strFilename + "\" in row "s +
               std::to_string(y_pos) + " and col " + std::to_string(x_pos) + "."s);
            current.emplace_back(c);
         }
         ++x_pos;
      }
      route.grid().emplace_back(current);
      ++y_pos;
   }


   size_t x_max = route.grid()[0].size();
   for (auto const& row : route.grid()) if (row.size() != x_max) throw std::runtime_error("unexpected len in grid"s);
   return { route, starting_here };
}


int main(void) {
   
   //auto [route, starting_here] = ReadValues("d:\\test\\aoc2022\\test_values12.txt");
   auto [route, starting_here] = ReadValues("d:\\test\\aoc2022\\values_day12.txt");
   route.start(starting_here);

   TRoute::path_grid grid_distance(route.max_y(), TRoute::path_row(route.max_x(), std::nullopt));
   auto [new_route, success] = MakeSteps(std::forward<TRoute>(route), grid_distance);

   if(success) {
      std::cout << "Day 12/1: " << new_route.pathway().size() - 1 << "\n";
      }
   else {
      std::cout << "Day 12/1: " << "Error: " << new_route.pathway().size() - 1 << "\n";
      }

   return 0;
   }