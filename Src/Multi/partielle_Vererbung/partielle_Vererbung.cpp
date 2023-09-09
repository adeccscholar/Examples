#include <iostream>

class TTest {
protected:
   int i = 0;
public:
   TTest() : i(0) { }
   TTest(int p ) : i(p) { }
   TTest(TTest const& ref) : i(ref.i) { }
   virtual ~TTest(void) { }

   virtual void Test1() = 0;
   virtual void Test2() = 0;

   virtual void Test0() = 0;
   };


class TTest1 : virtual public TTest {
public:
   TTest1() : TTest(0) { }
   TTest1(int p) : TTest(p) { }
   TTest1(TTest1 const& ref) : TTest(ref) { }
   virtual ~TTest1(void) { }


   virtual void Test0() override;
   virtual void Test1() override;

   // void Check() { delete new TTest1;  }
   };

class TTest2 : virtual public TTest {
public:
   TTest2() : TTest(1) { }
   TTest2(int p) : TTest(p) { }
   TTest2(TTest1 const& ref) : TTest(ref) { }
   virtual ~TTest2(void) { }

   virtual void Test0() override;
   virtual void Test2() override;

   //void Check() { delete new TTest2; }
};


class TTestAll : public TTest1, public TTest2 {
public:
   TTestAll() : TTest1(), TTest2(), TTest(3) { }

   virtual void Test0() override;

   void Check() { delete new TTestAll; }
};

void TTest1::Test0() {
   std::cout << "Test0 for TTest1 called.\n";
   return;
}

void TTest1::Test1() {
   std::cout << "Test1 called.\n";
   return;
   }

void TTest2::Test0() {
   std::cout << "Test0 for TTest2 called.\n";
   return;
}

void TTest2::Test2() {
   std::cout << "Test2 called.\n";
   return;
}


void TTestAll::Test0() {
   TTest1::Test0();
   TTest2::Test0();
   return;
}

int main()
{
   TTestAll test;
    std::cout << "Hello World!\n";
    test.Test0();
    test.Test1();
    test.Test2();
}

