// Day21.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <variant>
#include <functional>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <regex>

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

enum class EOperations : char {
   Addition       = '+',
   Subtraction    = '-',
   Multiplication = '*',
   Division       = '/',
   Modulo         = '%',
   undefined      = '$'
   }; 

using base_ty = long long;
using operation_ty = std::map<EOperations, std::pair<std::function<base_ty (base_ty, base_ty)>, EOperations>>;

const operation_ty operations = {
   { EOperations::Addition,       { [](base_ty a, base_ty b) -> base_ty{ return a + b;  }, EOperations::Subtraction } },
   { EOperations::Subtraction,    { [](base_ty a, base_ty b) -> base_ty{ return a - b;  }, EOperations::Addition } },
   { EOperations::Multiplication, { [](base_ty a, base_ty b) -> base_ty{ return a * b;  }, EOperations::Division } },
   { EOperations::Division,       { [](base_ty a, base_ty b) -> base_ty{ return a / b;  }, EOperations::Multiplication } },
   { EOperations::undefined,      { [](base_ty ,  base_ty  ) -> base_ty{ throw std::runtime_error("undefined operation in mission."); }, EOperations::undefined } }
    };

struct TOperations {
   std::string strOperand1;
   std::string strOperand2;
   EOperations _operator;
   };

using expression_ty = std::variant<base_ty, TOperations>;
using expressions_ty = std::map<std::string, expression_ty>;
using expressions_vec = std::vector<std::pair<std::string, expression_ty>>;

struct VisitHlp {
   std::ostream& out;
   VisitHlp() = delete;
   VisitHlp(VisitHlp const&) = delete;
   VisitHlp(std::ostream& p) : out(p) { }

   void operator()(base_ty const& val)   { out << val;  }
   void operator()(TOperations const& val) { out << val.strOperand1 << " " << static_cast<char>(val._operator) << " "
                                                 << val.strOperand2; }
   };

std::ostream& operator << (std::ostream& out, expression_ty const& data) {
   std::visit(VisitHlp(out), data);
   return out;
   }

std::ostream& Print(std::ostream& out, expressions_ty const& expressions, std::string const& variable, std::string const& end = ""s) {
   if (auto expression = expressions.find(variable); expression != expressions.end()) {
      auto& [key, value] = *expression;
      out << key << ": " << value << end;
      }
   else throw std::runtime_error("variable with name \""s + variable + "\" not found in our mission."s);
   return out;
   }

bool IsContant(expressions_ty& expressions, std::string const& variable) {
   if (auto expression = expressions.find(variable); expression != expressions.end()) {
      auto& [key, value] = *expression;
      return (std::holds_alternative<base_ty>(value) ? true : false);
   }
   else throw std::runtime_error("variable with the name \""s + variable + "\" not found in the mission."s);
}

void Set(expressions_ty& expressions, std::string const& variable, base_ty const& llValue) {
   if (auto expression = expressions.find(variable); expression != expressions.end()) {
      auto& [key, value] = *expression;
      value = llValue;
      }
   else {
      throw std::runtime_error("variable with the name = \""s + variable + "\" not found in function Set in our mission."s);
      }
   }

base_ty Get(expressions_ty& expressions, std::string const& variable, bool compress = true) {
   if(auto expression = expressions.find(variable); expression != expressions.end()) {
      auto& [key, value] = *expression;
      if (std::holds_alternative<base_ty>(value)) return std::get<base_ty>(value);
      else {
         auto const& statement = std::get<TOperations>(value);
         if (auto op = operations.find(statement._operator); op != operations.end()) {
            base_ty llValue = op->second.first(Get(expressions, statement.strOperand1, compress),
                                                 Get(expressions, statement.strOperand2, compress));
            if (compress) value = llValue;
            return llValue;
            }
         else throw std::runtime_error("operator "s + static_cast<char>(statement._operator) +
                                       " for \""s + key + "\"not found in our mission."s);
         }
      }
   else {
      throw std::runtime_error("variable with the name = \""s + variable + "\" not found in our mission."s);
      }
}


base_ty Get(expressions_ty const& expressions, std::string const& variable) {
   if (auto expression = expressions.find(variable); expression != expressions.end()) {
      auto& [key, value] = *expression;
      if (std::holds_alternative<base_ty>(value)) return std::get<base_ty>(value);
      else {
         auto const& statement = std::get<TOperations>(value);
         if (auto op = operations.find(statement._operator); op != operations.end()) {
            base_ty llValue = op->second.first(Get(expressions, statement.strOperand1),
               Get(expressions, statement.strOperand2));
            return llValue;
         }
         else throw std::runtime_error("operator "s + static_cast<char>(statement._operator) +
            " for \""s + key + "\"not found in our mission."s);
      }
   }
   else {
      throw std::runtime_error("variable with the name = \""s + variable + "\" not found in our mission."s);
   }
}


/*
cczh: sllz + lgvd
zczc: 2
ptdq: humn - dvpt
*/
std::pair<std::string, expression_ty> Parse(std::string const& line) {
   
   expression_ty expression;
   static const std::regex parser_input("^([a-z]{4})\\s*:\\s*([0-9]{1,4}|[a-z]{4}\\s*(.)\\s*[a-z]{4})");
   static const std::regex parser_expression("\\s*([a-z]{4})\\s*(.)\\s*([a-z]{4})");
   static const std::string strFmtVariable = "$1";
   static const std::string strFmtExpression = "$2";
   static const std::string strFmtOperand1 = "$1";
   static const std::string strFmtOperation = "$2";
   static const std::string strFmtOperand2 = "$3";

   if(std::regex_match(line, parser_input)) {
      std::string strVariable   = std::regex_replace(line, parser_input, strFmtVariable);
      std::string strExpression = std::regex_replace(line, parser_input, strFmtExpression);
      if(std::regex_match(strExpression, parser_expression)) {
         TOperations operations;
         operations.strOperand1 = std::regex_replace(strExpression, parser_expression, strFmtOperand1);
         std::string strOperation = std::regex_replace(strExpression, parser_expression, strFmtOperation);
         operations.strOperand2 = std::regex_replace(strExpression, parser_expression, strFmtOperand2);
         switch(strOperation[0]) {
            case '+': operations._operator = EOperations::Addition; break;
            case '-': operations._operator = EOperations::Subtraction; break;
            case '*': operations._operator = EOperations::Multiplication; break;
            case '/': operations._operator = EOperations::Division; break;
            case '%': operations._operator = EOperations::Modulo; break;
            default: throw std::runtime_error("unexpected operation "s + strOperation + "in "s + line);
            }

         expression = operations;
         return { strVariable, expression };
         }
      else {
         size_t err_pos;
         base_ty llValue = std::stoll(strExpression, &err_pos);
         if (err_pos < strExpression.length()) throw std::runtime_error("error reading expression \""s + strExpression + "\"."s);
         expression = llValue;
         return { strVariable, expression };
         }
      }
   else {
      throw std::runtime_error("can't parse the line \""s + line + "\"."s);
      }
  }

expressions_ty ReadAll(std::string const& strFilename) {
   auto ifs = input(strFilename);
   std::string strRow;
   expressions_ty expressions;

   while(std::getline(ifs, strRow)) {
      auto [variable, expression] = Parse(strRow);
      expressions.insert({ variable, expression });
      }

   return expressions;
   }


void ReadValues_1(std::string const& strFilename) {
   //auto expressions = ReadAll(strFilename);   // read all data from file
   //base_ty llValue = Get(expressions, "root"); // find root element and calculate recursive
   //std::cout << "Day 21/1: " << llValue << "\n"; // output of result for root

   std::cout << "Day 21/1: " << Get(ReadAll(strFilename), "root") << "\n";

}


std::tuple<expressions_vec, std::string, base_ty> Follow(expressions_ty const& expressions, std::string const& from, std::string const& to = "root"s) {
   expressions_vec reverse;
   std::string start = from, next;
   base_ty start_value;

   expression_ty expression = Get(expressions, from);
   reverse.push_back({ from, expression });

   while(start != to) {
         if (auto it = std::find_if(expressions.begin(), expressions.end(), [&next, &start](auto const& val) {
            if (std::holds_alternative<TOperations>(val.second)) {
               TOperations op = std::get<TOperations>(val.second);
               if (op.strOperand1 == start) {
                  start = val.first;
                  next = op.strOperand2;
                  return true;
               }
               if (op.strOperand2 == start) {
                  start = val.first;
                  next = op.strOperand1;
                  return true;
                  }
               }
         return false;
         }); it != expressions.end()) {
            reverse.push_back(*it);
            } 
      }

   if(auto it = expressions.find(next); it != expressions.end()) {
      start_value = Get(expressions, next);
      }
   // else error message
   return { reverse, next, start_value };
   }

const std::string strFilename = "D:\\test\\AOC2022\\values_day21.txt"s;


int main() {
   std::cout << "Advent of Code 21!\n";
   //ReadValues_1(strFilename);
   auto expressions = ReadAll(strFilename);
   base_ty solution = 0ll;

   if(auto expr_root = expressions.find("root"s); expr_root != expressions.end()) {
      auto & [key, value] = *expr_root;
      if (std::holds_alternative<base_ty>(value)) throw std::runtime_error(" term \"root\" contain a value.");
      else {
         auto& statement = std::get<TOperations>(value);
         statement._operator = EOperations::Subtraction;
         }
      }
   else {
      throw std::runtime_error("critical, root not found!");
      }

   auto [terms, start, val] = Follow(expressions, "humn"s, "root"s);
   std::cout << "\nstart with: root = 0 and " << start << " = " << val << "\n";



   std::string current = start, next;
   base_ty value = 0ll;
   for (size_t i = terms.size() - 1; i > 0; --i) {
      if (!std::holds_alternative<TOperations>(terms[i].second)) throw std::runtime_error("not solvable");
      auto const& term = std::get<TOperations>(terms[i].second);
      next = terms[i - 1].first;
      if (auto op = operations.find(term._operator); op != operations.end()) {
         if (auto rev_op = operations.find(op->second.second); rev_op != operations.end()) {
            base_ty new_value;
            if (term.strOperand1 == next) {
               new_value = Get(expressions, term.strOperand2, false);
               //std::cout << term.strOperand2 << ": " << new_value << "  " << (IsContant(expressions, term.strOperand2) ? " ja " : " nein ") << "\n";
               value = rev_op->second.first(value, new_value);
            }
            else if (term.strOperand2 == next) {
               new_value = Get(expressions, term.strOperand1, false);
               //std::cout << term.strOperand1 << ": " << new_value << "  " << (IsContant(expressions, term.strOperand1) ? " ja " : " nein ") << "\n";
               if (term._operator == EOperations::Subtraction || term._operator == EOperations::Division)
                  value = op->second.first(new_value, value);
               else
                  value = rev_op->second.first(value, new_value);
            }
            else throw std::runtime_error("operand not found");
         }
         else {
            throw std::runtime_error("reverse operation for this term not found");
         }
      }
      else {
         throw std::runtime_error("operation in this term not found");
      }
     // std::cout << terms[i].first << ": " << terms[i].second << "\n";
     // std::cout << next << ": " << value << "\n";
   }
   std::cout << next << ": " << value << "\n";



   }

