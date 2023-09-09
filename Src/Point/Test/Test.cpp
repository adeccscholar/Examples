
#include <iostream>
#include <sstream>
#include <type_traits>
#include <utility>
#include <cmath>
#include <string>
#include <numbers>

//#include "MyDistance5.h"
#include "MyPoint4.h"

// Wir beginnen mit einer einfachen Klasse 


int main() {
   std::cout << "Systemschicht, entwickeln eigener allgemeiner Klassen!\n";
   auto x = 25.0_rad;
   std::cout << x.toRadians() << "\n";
   /*
   //MyDistance<double, MyDistanceKind::meter> testX(1000);
   auto testX = 1000._meter;
   MyDistance<double, MyDistanceKind::yards> testY;
   testY = testX;
   
   bool test = (testX == testY);

   std::cout << testY << "  " << sizeof(testY) << "\n";

   auto angle1 = 45._deg; // Winkel in Grad
   auto angle2 = 0.7854_rad; // Winkel in Radian
   MyAngle<double, MyAngleKind::radian> angle3(angle1);
   MyAngle<float> testA(45);

   MyAngle testB;
   testB = testA;

   std::cout << angle1 << "\n" << angle2 << "\n" << angle3 << "\n\n";

   std::cout << angle1.toRadians() << "\n";

   MyPoint<double> pkt1(7.3, 5.3);
   MyPoint<double> pkt2("(4.1, 4.2)");
   MyPoint<double> pkt3(81.0, angle1);
   MyPoint<double> pkt4(pkt3);
   std::string testh = pkt1;
   std::cout << testh << ", " << pkt1.angle() << "\n"
             << pkt2 << "\n"
             << pkt4 << " -> " << pkt4.Distance() << " / " << pkt4.Angle() << "\n"
             << pkt1.distance(pkt2) << " - " << pkt1.angle(pkt2) << "\n";
   */
   }
