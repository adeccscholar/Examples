#pragma once


#include <iostream>
#include <sstream>
#include <utility>
#include <stdexcept>
#include <cmath>
#include <string>
#include <numbers>
#include <type_traits>
#include <format>


template <std::floating_point ty = double>
class MyPoint : public std::pair<ty, ty> {
   friend std::ostream& operator << (std::ostream& out, MyPoint<ty> const& point) {
      return out << "(" << point.first << ", " << point.second << ")";
      }

   friend std::istream& operator >> (std::istream& in, MyPoint<ty>& point) {
      char c;
      in >> c >> point.first >> c >> point.second >> c;
      return in;
      }

   friend MyPoint<ty> operator + (MyPoint<ty> const& a, MyPoint<ty> const& b) {
      return MyPoint<ty>(a.first + b.first, a.second + b.second);
      }

   friend MyPoint<ty> operator + (MyPoint const& a, ty const& b) {
      return MyPoint<ty>(a.first + b, a.second);
      }

   friend MyPoint<ty> operator - (MyPoint<ty> const& a, MyPoint<ty> const& b) {
      return MyPoint<ty>(a.first - b.first, a.second - b.second);
      }

   friend MyPoint<ty> operator - (MyPoint<ty> const& a, double const& b) {
      return MyPoint<ty>(a.first - b, a.second);
      }

public:
   constexpr MyPoint(void) : MyPoint<ty>(0.0, 0.0) { }
   constexpr MyPoint(ty const& x, ty const& y = 0.0) : std::pair<ty, ty>(x, y) { }
   MyPoint(std::string const& text) {
      std::istringstream ss(text);
      ss >> *this;
      if (!ss) throw std::invalid_argument("ung�ltiges Koordinatenpaar");
   }

   constexpr MyPoint(MyPoint const& other) noexcept = default;
   constexpr MyPoint(MyPoint&& other) noexcept = default;

   MyPoint& operator = (MyPoint const&) = default;
   MyPoint& operator = (MyPoint&&) noexcept = default;

   MyPoint& operator += (MyPoint const& other) {
      this->first += other.first;
      this->second += other.second;
      return *this;
      }

   MyPoint& operator += (ty const& other) {
      this->first += other;
      return *this;
      }

   MyPoint& operator -= (MyPoint const& other) {
      this->first -= other.first;
      this->second -= other.second;
      return *this;
      }

   MyPoint& operator -= (ty const& other) {
      this->first -= other;
      return *this;
      }

   operator std::string() const { return std::format("({}, {})", this->first, this->second); }

   auto operator <=> (MyPoint const& other) const = default;


   ty const& X(void) const { return this->first; }
   ty const& Y(void) const { return this->second; }

   void X(ty const& newVal) { this->first = newVal; }
   void Y(ty const& newVal) { this->second = newVal; }

   ty distance(MyPoint const& other) const { return euclidean_distance(*this, other); }
   ty distance(void) const { return distance(MyPoint<ty>()); }

   ty angle(void) const {
      return std::atan2(this->second, this->first) * (180.0 / std::numbers::pi_v<ty>);
      }

   ty angle(MyPoint const& other) const {  return angle(*this, other); }

   auto addDistanceAndAngle(ty const& distance, ty const& angleInDegrees) const {
      ty angleInRadians = angleInDegrees * (std::numbers::pi_v<ty> / 180.0); // Umrechnung in Bogenma�
      return MyPoint<ty>(X() + distance * std::cos(angleInRadians), Y() + distance * std::sin(angleInRadians));
      }


   static ty euclidean_distance(MyPoint const& a, MyPoint const& b) {
      ty dx = a.first - b.first;
      ty dy = a.second - b.second;
      return std::sqrt(dx * dx + dy * dy);
      }

   static ty angle(MyPoint const& a, MyPoint const& b) {
      return (b - a).angle();
      }

   static ty manhattan_distance(MyPoint const& a, MyPoint const& b) {
      return std::fabs(a.first - b.first) + std::fabs(a.second - b.second);
      }
   };
