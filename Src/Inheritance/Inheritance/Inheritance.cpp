// Inheritance.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>

class Base {
public:
   virtual ~Base() = default;

   virtual int test() = 0;

};

int Base::test() { return 42; }

class derived : Base {
public:
   virtual int test() { return Base::test(); }

};


void check(double d) {
   std::cout << d << "\n";
}

template <typename ty> void check(ty) = delete;

int main()
{
   //Base a;
   derived b;
   check(42.0);
    std::cout << b.test() << "\n";
}

