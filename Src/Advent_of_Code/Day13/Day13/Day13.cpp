
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <variant>
#include <stdexcept>
#include <optional>
#include <algorithm>
#include <functional>

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



// ------------------------------------------------------------------------------------
template <typename ty>
using VarTy = std::variant<size_t, std::vector<ty>>;

template <template <typename> class K>
struct Fixed : K<Fixed<K>> {
   using K<Fixed>::K;
};

using param_type2 = Fixed<VarTy>;
using param_list2 = std::vector<param_type2>;


std::ostream& operator << (std::ostream& out, param_type2 const& data) {
   if (std::holds_alternative<size_t>(data)) out << std::get<size_t>(data);
   else {
      param_list2 const& list = std::get<param_list2>(data);
      if (list.size() == 0) out << "[ ]";
      else {
         for (size_t pos = 0u; auto const& val : list) out << (pos++ ? ", " : "[ ") << val;
         out << " ]";
      }
   }
   return out;
}

std::strong_ordering Compare(param_type2 const& lhs, param_type2 const& rhs) {
   if (std::holds_alternative<size_t>(lhs) && std::holds_alternative<size_t>(rhs)) {
      return std::get<size_t>(lhs) <=> std::get<size_t>(rhs);
   }
   else if (std::holds_alternative<size_t>(lhs)) {
      param_list2 left_list = param_list2{ std::get<size_t>(lhs) };
      return Compare(left_list, rhs);
   }
   else if (std::holds_alternative<size_t>(rhs)) {
      param_list2 right_list = param_list2{ std::get<size_t>(rhs) };
      return Compare(lhs, right_list);
   }
   else {
      param_list2 const& lstLeft = std::get<param_list2>(lhs);
      param_list2 const& lstRight = std::get<param_list2>(rhs);
      auto max_size = std::pair(lstLeft.size(), lstRight.size());
      for (auto [l, r] = std::make_pair(lstLeft.begin(), lstRight.begin()); l != lstLeft.end() && r != lstRight.end(); ++l, ++r) {
         if (auto val = Compare(*l, *r); val != 0) return val;
      }
      return max_size.first <=> max_size.second;
   }
}


using value_list = std::vector<std::pair<param_type2, param_type2>>;

param_type2 parse(std::string input) {
   static auto trim = [](std::string&& text) {
      text.erase(0, text.find_first_not_of(" \t"));
      text.erase(text.find_last_not_of("\t ") + 1, text.length());
      return text;
      };

   param_type2 retval;

   std::stack<param_list2> workflow;
   size_t pos = 0u, end = input.length();

   while((end = input.find_first_of(",[]", pos)) != std::string::npos) {
      std::optional<size_t> value = std::nullopt;
      std::string strPart = trim(input.substr(pos, end - pos));
      if(strPart.length() > 0) {
         size_t err_pos;
         value = std::stoi(strPart, &err_pos);
         if (err_pos != strPart.size()) throw std::runtime_error("unexpected char in input."s);
         }
      if (value && workflow.empty()) throw std::runtime_error("stack is empty"s);

      switch(input[end]) {
         case '[' :
            if (value) workflow.top().push_back(*value);
            workflow.push(param_list2{ });
            break;
         case ',':
            if (!workflow.empty()) {
               if (value) workflow.top().push_back(*value);
               }
            else throw std::runtime_error("no param list open");
            break;
         case ']':
            if(!workflow.empty()) {
               if (value) workflow.top().push_back(*value);
               param_list2 elem = workflow.top();
               workflow.pop();
               if (!workflow.empty())
                  workflow.top().push_back(elem);
               else
                  retval = elem;
               }
            break;
         default: throw std::runtime_error("critical, unhandled char in parsing!!"s);
         }
      pos = end + 1;
      }
   if (!workflow.empty()) throw std::runtime_error("wrong input, ] missed."s);
   return retval;
   }

value_list ReadPairs(std::string const& strFilename) {
   value_list all_values;
   auto ifs = input(strFilename);
   std::string strRow;
   size_t lrows = 0u;
   std::pair<param_type2, param_type2> value;
   while (std::getline(ifs, strRow)) {
      if (lrows % 3 == 0) value.first = parse(strRow);
      else if (lrows % 3 == 1) {
         value.second = parse(strRow);
         all_values.push_back(value);
      }
      ++lrows;
   }

   return all_values;
}

std::vector<param_type2> ReadAll(std::string const& strFilename) {
   std::vector<param_type2> all_values;
   auto ifs = input(strFilename);
   std::string strRow;
   size_t lrows = 0u;
   std::pair<param_type2, param_type2> value;
   while (std::getline(ifs, strRow)) {
      if (strRow.length() > 0) {
         all_values.emplace_back(parse(strRow));
      }
   }
   return all_values;
}

int main() {
   std::vector<param_type2> values = ReadAll("d:\\test\\aoc2022\\values_day13.txt");
   param_type2 p1 = parse("[[2]]"), p2 = parse("[[6]]");
   values.push_back(p1);
   values.push_back(p2);

   std::sort(values.begin(), values.end(), [](auto const& lhs, auto const& rhs) { return Compare(lhs, rhs) < 0;  });

   size_t value = 1u;
   for (size_t index = 1u; auto const& val : values) {
      if (Compare(val, p1) == 0 || Compare(val, p2) == 0) {
         value *= index;
      }
      ++index;
      std::cout << val << "\n";
   }
   std::cout << "Day 13/2: " << value << "\n";
}


