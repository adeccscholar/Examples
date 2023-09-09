// Day16.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <optional>
#include <regex>
#include <tuple>
#include <utility>
#include <vector>
#include <map>
#include <algorithm> 
#include <chrono>

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

/*
Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
Valve BB has flow rate=13; tunnels lead to valves CC, AA
Valve CC has flow rate=2; tunnels lead to valves DD, BB
Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE
Valve EE has flow rate=3; tunnels lead to valves FF, DD
Valve FF has flow rate=0; tunnels lead to valves EE, GG
Valve GG has flow rate=0; tunnels lead to valves FF, HH
Valve HH has flow rate=22; tunnel leads to valve GG
Valve II has flow rate=0; tunnels lead to valves AA, JJ
Valve JJ has flow rate=21; tunnel leads to valve II
*/


using distance_value = std::optional<std::pair<size_t, size_t>>;
using distance_data  = std::map<std::string, distance_value>;
using valve_data     = std::tuple<size_t, std::vector<std::string>, distance_data>;
using valves_data    = std::map<std::string, valve_data>;

//using workflow_data = std::map<std::string, std::tuple<size_t, size_t>>;
using workflow_data = std::map<std::pair<std::string, size_t>, std::tuple<size_t, size_t>>;

std::ostream& operator << (std::ostream& out, valves_data const& data) {
   for(auto const& line : data) {
      out << line.first << " with " << std::get<0>(line.second) << "% and targets ";
      for (size_t i = 0u; auto const& v : std::get<1>(line.second)) out << (i++ ? ", " : "") << v;
      out << " -> " << std::get<2>(line.second).size() << "\n";
      }
   return out;
   }

std::ostream& operator << (std::ostream& out, distance_data::value_type const& data) {
   if (data.second)
      out << " - " << data.first << " in " << data.second->first << " with " << data.second->second << "%";
   return out;
}

std::ostream& operator << (std::ostream& out, distance_data const& data) {
   std::for_each(data.begin(), data.end(), [&out](auto const& val) { out << val << "\n"; });
   return out;
}


std::pair<std::string, valve_data> Parse(std::string const& data) {
   static const std::regex parser_input("^(\\bValve\\b)\\s*([A-Z]{2})\\s*(\\bhas flow rate\\b)\\s*=\\s*([0-9]{1,3})"
      ";\\s*[\\btunnels lead to valves\\b | \\btunnel leads to valve\\b]\\s*(.{1,80})$");
   static const std::regex parser_valves("[A-Z]{2}");

   static const std::string strFmtValve   = "$2"; // Valve
   static const std::string strFmtPercent = "$4"; // Percent
   static const std::string strFmtTargets = "$5"; // list of neighbors
   std::pair<std::string, valve_data> result;

   if (std::regex_match(data, parser_input)) {
      std::string strValve = std::regex_replace(data, parser_input, strFmtValve);
      std::string strPercent = std::regex_replace(data, parser_input, strFmtPercent);
      std::string strTargets = std::regex_replace(data, parser_input, strFmtTargets);

      size_t err_pos;
      size_t iValue = stoi(strPercent, &err_pos);
      if (err_pos < strPercent.length()) throw std::runtime_error("error reading percent in "s + data);
      else {
         std::vector<std::string> vecTargets;
         std::sregex_token_iterator it(strTargets.begin(), strTargets.end(), parser_valves);
         std::sregex_token_iterator end;
         for (; it != end; ++it) {
            vecTargets.push_back(it->str());
         }
         result = { strValve, { iValue, vecTargets, { } } };
      }
   }
   else {
      throw std::runtime_error("unexpected row in inputfile"s + data);
   }
   return result;
}

valves_data ReadInput(std::string const& strFilename) {
   valves_data retval;
   auto ifs = input(strFilename);
   std::string strRow;
   while(std::getline(ifs, strRow)) {
      retval.insert(Parse(strRow));
      }
   return retval;
   }



void Build(valves_data& data, std::string const& start, std::vector<std::string> const& targets, size_t dept, distance_data& distances) {
   if (dept == 1) {
      // initialize the distances
      for (auto const& [valve, other] : data)
         if (valve != start) distances.insert({ valve, std::nullopt });

      if (auto elem = data.find(start); elem != data.end()) {
         for (auto const& target : targets) {
            if (auto child = data.find(target); child != data.end()) {
               distances[target] = { dept, std::get<0>(child->second) };
               Build(data, start, std::get<1>(child->second), dept + 1, distances);
            }
            // else throw
         }
      }
      else throw std::runtime_error("internal error, element for item "s + start + " not found");
   }
   else {
      for (auto& target : targets) {
         if (target != start && (!distances[target] || distances[target]->first > dept)) {
            if (auto child = data.find(target); child != data.end()) {
               distances[target] = { dept, std::get<0>(child->second) };
               Build(data, start, std::get<1>(child->second), dept + 1, distances);
            }
         }
      }
   }
};


void Clean(valves_data& data) {
   std::for_each(data.begin(), data.end(), [](auto& val) mutable {
      std::erase_if(std::get<2>(val.second), [](auto const& elem) {
         auto const& [key, value] = elem;
         return value ? value->second == 0 : true;
         });
      });
}

void Build(valves_data& data) {
   auto func_start = std::chrono::high_resolution_clock::now();
   for (auto& elem : data) {
      Build(data, elem.first, std::get<1>(elem.second), 1, std::get<2>(elem.second));
      }
   Clean(data);
   auto func_ende = std::chrono::high_resolution_clock::now();
   auto time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);
   //std::cout << "Build in " << time.count() / 1000. << " sec\n";
   //std::cout << data << "\n\n";
}


size_t Evaluate(size_t max_time, workflow_data& workflow, valves_data const& data) {
   size_t retval = 0u;
   std::for_each(workflow.begin(), workflow.end(), [&retval, &max_time](auto const& val) {
      if (auto const& time = std::get<0>(val.second); time < max_time) {
         if (auto const& value = std::get<1>(val.second); value > 0) {
            retval += (max_time - time) * value;
            }
         }
      });
   return retval;
}


std::optional<size_t> MakeStep(std::string const& target, workflow_data workflow, size_t max_time, size_t next_time, valves_data const& data) {
   if (next_time >= max_time) return Evaluate(max_time, workflow, data);
   if (auto step = data.find(target); step != data.end()) {   // open only if effect opening need 1 second
      if (auto const& value = std::get<0>(step->second); value > 0) {
         ++next_time;
         workflow.insert({ { target, 0 }, { next_time, value } });
      }
      std::optional<size_t> max_value = Evaluate(max_time, workflow, data);
      for (auto const& [next, value_next] : std::get<2>(step->second)) {
         //if (auto cntrl = workflow.find(next); cntrl == workflow.end()) {  // player 2, find_if
         if (auto cntrl = find_if(workflow.begin(), workflow.end(), [&next](auto const& val) {
                                                                  auto const& t = val.first;
                                                                  return t.first == next;
                                                                  }); cntrl == workflow.end()) {
            if (value_next && (max_time > next_time + value_next->first) && value_next->second > 0) {
               auto val = MakeStep(next, workflow, max_time, next_time + value_next->first, data);
               if (val && *val > max_value) {
                  max_value = val;
               }
            }
         }
      }
      return max_value;
   }
   else throw std::runtime_error("Critical error: item with "s + target + " didn't found in data!");
}







int main()  {
   try {
    std::cout << "Hello World!\n";
    auto data = ReadInput("d:\\test\\aoc2022\\values_day16.txt");
    Build(data);
    //std::cout << data << "\n";

    auto func_start = std::chrono::high_resolution_clock::now();
    auto value = MakeStep("AA"s, { }, 30, 0, data);
    auto func_ende = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(func_ende - func_start);
    if (value) std::cout << "Day 16/1: " << *value << " in " << time.count() / 1000. << " sec\n";


   }
   catch(std::exception& ex) {
      std::cerr << "error: " << ex.what() << "\n";
      }
}

