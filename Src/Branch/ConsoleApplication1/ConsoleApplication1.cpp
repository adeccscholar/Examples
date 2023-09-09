// ConsoleApplication1.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <chrono>
#include <functional>

enum class EDistribution : int { normal, uniform, undefined = 999 };

std::vector<double> Build(EDistribution distr = EDistribution::uniform, size_t count = 100'000,  std::pair<double, double> const& limits = { -1'000.0, 1'000.0 }) {
   using myParam = std::pair<double, double>;

   static std::map<EDistribution, std::function<std::function<double()>(myParam const&)>> funct = {
      { EDistribution::normal,  [](myParam const& limits) {
             std::mt19937 rndNbr;
             rndNbr.seed(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));
             double middle = limits.first + (limits.second - limits.first) / 2;
             double range = (limits.second - limits.first) / 4;
             std::normal_distribution<double> distNbr(middle, range);
             return std::bind(distNbr, rndNbr);
             }    },
      { EDistribution::uniform, [](myParam const& limits) {
             std::mt19937 rndNbr;
             rndNbr.seed(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()));
             std::uniform_real_distribution<double> distNbr(limits.first, limits.second);
             //distNbr.param(std::uniform_real_distribution<double>(limits.first, limits.second).param());
             return std::bind(distNbr, rndNbr);
             }    }
      // { EDistribution::normal, [](myParam const& limits) {}    }

      };

   if (limits.first >= limits.second) throw std::range_error("error for calling Build, wrong parameter for limits");
   std::vector<double> data(count);

   if (auto it = funct.find(distr); it != funct.end()) {
      auto generator = it->second(limits);
      for (auto& val : data) {
         do {
            val = generator();
         } while (val < limits.first || val > limits.second);
      }
   }
   else {
      throw std::runtime_error("error: entry for Distribution not found");
   }


   return data;
}

template <typename tty, typename fty, typename... Args>
inline auto Call(tty& time, fty function, Args... args) {
   auto func_start = std::chrono::high_resolution_clock::now();
   auto ret = function(std::forward<Args>(args)...);
   auto func_ende = std::chrono::high_resolution_clock::now();
   time = std::chrono::duration_cast<tty>(func_ende - func_start);
   return ret;
   }

using myTime = std::chrono::microseconds;

bool Lesser(double& a, double& b) {
   bool RetVal;
   if (a < b) {
      RetVal = true;
      }
   else {
      RetVal = false;
      }
   return RetVal;
   }

bool Lesser2(double& a, double& b) {
   return a < b ? true : false;
   }

int Sortiere(std::vector<double>& values) {
   std::sort(values.begin(), values.end(), Lesser2);
   return values.size();
   }

int main() {
    std::cout << "Hello World!\n";
    auto test_data = Build(EDistribution::normal, 250'000);
    std::cout << test_data.size() << "\n";
    myTime time;
    Call(time, Sortiere, std::ref(test_data));
    std::cout << time << "\n";
}

