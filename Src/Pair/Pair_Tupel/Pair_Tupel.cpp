// Pair_Tupel.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <iomanip>
#include <utility>
#include <tuple>
#include <fstream>

constexpr int Fak(int i) {
   return (i > 1 ? i * Fak(i - 1) : 1);
   }

int Feld[Fak(5)];

std::tuple<int, int, int> Funktion() {
   return { Fak(5), 5, 1 };
   }

/*
void Write(int val) {
   std::cout << val << std::endl;
   }

void Write(double val) {
   std::cout << std::setprecision(3) << val << std::endl;
}
*/

class TWrite {
   std::ostream& out;
public:
   TWrite(std::ostream& par) : out(par) { }
   void operator ()(int val) {
      out << val << std::endl;
      }
   void operator ()(double val) {
      out << std::setprecision(3) << val << std::endl;
      }

};

int main() {
   /*
   auto x = Funktion();
   std::cout << "Hello World!\n"
      << std::get<0>(x) << " " << std::get<1>(x) << " " << std::get<2>(x) << std::endl;
   */
   int a, b = 7, c;
   //auto [a, b, c] = Funktion();
   std::tie(a, std::ignore, c) = Funktion();
   std::cout << "Hello World!\n"
      << a << " " << b << " " << c << std::endl;
   std::cout.setf(std::ios::fixed);
   std::ofstream of("d:\\test.txt");
   TWrite Write(of);
   Write(10);
   Write(10.2);

}

