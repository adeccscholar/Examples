#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <stdexcept>
#include <iterator>

using myDate = std::tuple<int, int, int>;
using myData = std::tuple<int, myDate, myDate, int, double, double, std::string, std::string>;

int tokenize(std::string const& strSrc, std::vector<int> const& vecGrp, std::vector<std::string>& vecToken) {
   size_t iStart = 0u, iCount = 0u, iLen = strSrc.length();
   vecToken.clear();
   if(iLen == 0 || strSrc.empty()) return 0;

   for(auto const& offset : vecGrp) {
      if(iStart > iLen) break;
      vecToken.push_back(strSrc.substr(iStart, offset));
      iStart += offset;
      ++iCount;
      }
   return iCount;
   }



// -----------------------------------------------------------

std::string trim_left(std::string const& strText, std::string const& strDelimiter = " ") {
   std::string strVal { strText };
   strVal.erase(0, strVal.find_first_not_of(strDelimiter));
   return strVal;
   }

std::string trim_right(std::string const& strText, std::string const& strDelimiter = " ") {
   std::string strVal { strText };
   strVal.erase(strVal.find_last_not_of(strDelimiter) + 1, strVal.length());
   return strVal;
   }

std::string trim(std::string const& strText, std::string const& strDelimiter = " ") {
   std::string strVal { strText };
   strVal.erase(0, strVal.find_first_not_of(strDelimiter));
   strVal.erase(strVal.find_last_not_of(strDelimiter) + 1, strVal.length());
   return strVal;
   }


// -----------------------------------------------------------

myData interpreting(std::vector<std::string>& token) {
   static std::vector<int> dateoffsets { 4, 2, 2 };
   if(token.size() < 8) throw std::runtime_error("error with data");

   token[0] = trim_right(trim_left(token[0], "0 "), " ");
   for(size_t i = 1; i < 8; ++i) token[i] = trim(token[i], " ");

   size_t pos;
   int iStation   = std::stoi(token[0], &pos);

   std::vector<std::string> dateparts;
   tokenize(token[1], dateoffsets, dateparts);
   myDate start  { std::stoi(dateparts[0], &pos),
                   std::stoi(trim_left(dateparts[1], "0 "), &pos),
                   std::stoi(trim_left(dateparts[2], "0 "), &pos) };

   tokenize(token[2], dateoffsets, dateparts);
   myDate finish { std::stoi(dateparts[0], &pos),
                   std::stoi(trim_left(dateparts[1], "0 "), &pos),
                   std::stoi(trim_left(dateparts[2], "0 "), &pos) };

   int iHeight   = std::stoi(token[3], &pos);
   double flLat  = std::stod(token[4], &pos);
   double flLong = std::stod(token[5], &pos);

   return { iStation, start, finish, iHeight, flLat, flLong, token[6], token[7] };

   }

// -----------------------------------------------------------

int main(void) {
   static const std::vector<int> parts { 5, 9, 9, 15, 12, 10, 41, 30 };
   std::map<int, myData> mpData, mpBerlin;
   ifstream ifs;
   ifs.open("D:\\Test\\Wetterstationen.txt");
   if(!ifs.is_open()) {
      std::cerr << "file can't open" << std::endl;
      return 1;
      }

   try {
      std::string strRow;
      while(std::getline(ifs, strRow)) {
         std::vector<std::string> token;
         tokenize(strRow, parts, token);
         myData data = interpreting(token);
         mpData.insert( { std::get<0>(data), data } );
         }
      }
   catch(std::exception& ex) {
      std::cerr << "error: " << ex.what() << std::endl;
      }


   std::copy_if(mpData.begin(), mpData.end(), std::inserter(mpBerlin, mpBerlin.end()),
                     [](auto val) {
                        return std::get<7>(val.second) == "Berlin";
                        });

   std::for_each(mpBerlin.begin(), mpBerlin.end(), [](auto const& val) {
                 std::cout << val.first << " " << std::get<6>(val.second) << std::endl;
                 });

   /*
   if(auto it = std::find(mpData.begin(), mpData.end(), 403); it != mpData.end()) {

      }
   */
  // std::cout << std::get<6>(mpData[403]) << std::endl;
   getchar();
   }
