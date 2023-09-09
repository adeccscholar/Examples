#pragma once

#include <iostream>
#include <sstream>
#include <utility>
#include <stdexcept>
#include <cmath>
#include <string>
#include <numbers>

class MyPoint : public std::pair<double, double> {
   friend std::ostream& operator << (std::ostream& out, MyPoint const& point) {
      return out << "(" << point.first << ", " << point.second << ")";
      }

   friend std::istream& operator >> (std::istream& in, MyPoint& point) {
      char c;
      in >> c >> point.first >> c >> point.second >> c;
      return in;
      }

   friend MyPoint operator + (MyPoint const& a, MyPoint const& b) {
      return MyPoint(a.first + b.first, a.second + b.second);
      }

   friend MyPoint operator + (MyPoint const& a, double const& b) {
      return MyPoint(a.first + b, a.second);
      }

   friend MyPoint operator - (MyPoint const& a, MyPoint const& b) {
      return MyPoint(a.first - b.first, a.second - b.second);
      }

   friend MyPoint operator - (MyPoint const& a, double const& b) {
      return MyPoint(a.first - b, a.second);
      }

public:
   constexpr MyPoint(void) : MyPoint(0.0, 0.0) { }
   constexpr MyPoint(double const& x, double const& y = 0.0) : std::pair<double, double>(x, y) { }
   MyPoint(std::string const& text) {
      std::istringstream ss(text);
      ss >> *this;
      if (!ss) throw std::invalid_argument("ungültiges Koordinatenpaar");
   }

   constexpr MyPoint(MyPoint const& other) noexcept = default;
   constexpr MyPoint(MyPoint&& other) noexcept = default;

   MyPoint& operator = (MyPoint const&) = default;
   MyPoint& operator = (MyPoint&&) noexcept = default;

   MyPoint& operator += (MyPoint const& other) {
      first += other.first;
      second += other.second;
      return *this;
   }

   MyPoint& operator += (double const& other) {
      first += other;
      return *this;
   }

   MyPoint& operator -= (MyPoint const& other) {
      first -= other.first;
      second -= other.second;
      return *this;
   }

   MyPoint& operator -= (double const& other) {
      first -= other;
      return *this;
   }

   operator std::string() const {
      std::stringstream ss;
      ss << *this;
      return ss.str();
   }

   auto operator <=> (MyPoint const& other) const = default;


   double const& X(void) const { return first; }
   double const& Y(void) const { return second; }

   void X(double const& newVal) { first = newVal; }
   void Y(double const& newVal) { second = newVal; }

   double distance(MyPoint const& other) const {
      double dx = other.first - first;
      double dy = other.second - second;
      return std::sqrt(dx * dx + dy * dy);
   }


   double angle(void) const {
      return std::atan2(second, first) * (180.0 / std::numbers::pi_v<double>);
   }

   double angle(MyPoint const& other) const {
      auto tmp = other - *this;
      return tmp.angle();
   }

   MyPoint addDistanceAndAngle(double const& distance, double const& angleInDegrees) const {
      double angleInRadians = angleInDegrees * (std::numbers::pi_v<double> / 180.0); // Umrechnung in Bogenmaß
      return MyPoint(X() + distance * std::cos(angleInRadians), Y() + distance * std::sin(angleInRadians));
   }
};
