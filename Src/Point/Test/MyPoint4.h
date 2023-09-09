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

enum class MyAngleKind : int { degree, radian };

template<typename T>
inline constexpr bool always_false_angle = false;

template <std::floating_point ty = double, MyAngleKind kind = MyAngleKind::degree>
class MyAngle {
   friend std::ostream& operator << (std::ostream& out, MyAngle<ty, kind> const& angle) {
      return out << angle() << " " << angle.unit();
      }

   public:
      MyAngle(void) = default;

      template <typename OtherTy>
         requires std::is_floating_point_v<OtherTy> || 
                  (std::is_integral_v<OtherTy> && !std::is_same_v<OtherTy, bool>)
      MyAngle(OtherTy const& val) : flAngle(static_cast<ty>(val)) {}

      //MyAngle(ty const& val) : flAngle(val) { }
      //MyAngle(MyAngle<ty, kind> const& other) : flAngle(other.flAngle()) {}

      template <MyAngleKind other_kind, std::floating_point OtherTy = ty>
      MyAngle(MyAngle<OtherTy, other_kind> const& other) {
         if constexpr (other_kind == kind) {
            flAngle = static_cast<ty>(other.flAngle);
            }
         if constexpr (other_kind == MyAngleKind::degree && kind == MyAngleKind::radian) {
            flAngle = static_cast<ty>(other.convert_to<MyAngleKind::radian>());
            }
         else if constexpr (other_kind == MyAngleKind::radian && kind == MyAngleKind::degree) {
            flAngle = static_cast<ty>(other.convert_to<MyAngleKind::degree>());
            }
         else static_assert(always_false_angle<kind>, "invalid kind of angle");
         }

      MyAngle(MyAngle&&) noexcept = default;


      template <MyAngleKind other_kind, std::floating_point OtherTy = ty>
      MyAngle& operator = (MyAngle<OtherTy, other_kind> const& other) {
         convert_from<other_kind>(other.Angle());
         return *this;
         }
      
      MyAngle& operator = (MyAngle&&) = default;

      auto operator <=> (MyAngle const&) const = default;

      ty& operator () (void) { return flAngle; }
      ty const& operator () (void) const { return flAngle; }

      operator ty& (void) & { return flAngle; }
      operator ty const& (void) const& { return flAngle; }

      operator std::string(void) const { 
         if constexpr (kind == MyAngleKind::degree) return std::format("{} deg", flAngle);
         else if constexpr (kind == MyAngleKind::radian) return std::format("{} rad", flAngle);
         else static_assert(always_false_angle<kind>, "invalid kind of angle");
         }

      ty const& Angle(void) const { return flAngle; }

      std::string unit(void) const {
         if constexpr (kind == MyAngleKind::degree) return "deg";
         else if constexpr (kind == MyAngleKind::radian) return "rad";
         else static_assert(always_false_angle<kind>, "invalid kind of angle");
         }

      template <MyAngleKind other_kind = kind, typename = std::enable_if_t<other_kind == MyAngleKind::degree>>
      ty toRadians(void) const { return flAngle * std::numbers::pi_v<ty> / 180.0; }

      void Angle(ty const& val) { flAngle = val; }

      void fromRadians(ty const& val) { flAngle = convert<MyAngleKind::radian>(val); }
   private:
      ty flAngle;


   public:
      template<MyAngleKind other_kind>
      void convert_from(ty const& val) {
         if constexpr (kind == MyAngleKind::degree && other_kind == MyAngleKind::radian) {
            flAngle = val * 180.0 / std::numbers::pi_v<ty>;
            }
         else if constexpr (kind == MyAngleKind::radian && other_kind == MyAngleKind::degree) {
            flAngle = val * std::numbers::pi_v<ty> / 180.0;
            }
         else {
            flAngle = val;
            }
         }

      template<MyAngleKind other_kind>
      ty convert_to() const {
         if constexpr (kind == MyAngleKind::degree && other_kind == MyAngleKind::radian) {
            return flAngle * std::numbers::pi_v<ty> / 180.0;
         }
         else if constexpr (kind == MyAngleKind::radian && other_kind == MyAngleKind::degree) {
            return flAngle * 180.0 / std::numbers::pi_v<ty>;
         }
         else {
            return flAngle;
         }
      }
   };

MyAngle<double> operator""_deg(long double val) {
   return MyAngle<double>(static_cast<double>(val));
   }

MyAngle<double> operator""_rad(long double val) {
   MyAngle<double> tmp;
   tmp.convert_from<MyAngleKind::radian>(static_cast<double>(val));
   return tmp;
   }


template <std::floating_point ty>
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
   constexpr MyPoint(ty const& distance, MyAngle<ty> const& angle) : MyPoint<ty>() { 
      DistanceAndAngle(distance, angle); 
      }

   MyPoint(std::string const& text) {
      std::istringstream ss(text);
      ss >> *this;
      if (!ss) throw std::invalid_argument("ung¸ltiges Koordinatenpaar");
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
   ty Distance(void) const { return distance(*this, MyPoint<ty>()); }
   MyAngle<ty> Angle(void) const { return angle(MyPoint<ty>(), *this); }

   constexpr void X(ty const& newVal) { this->first = newVal; }
   constexpr void Y(ty const& newVal) { this->second = newVal; }

   constexpr void DistanceAndAngle(ty const& distance, MyAngle<ty> const& angleInDegrees) {
      const ty angleInRadians = angleInDegrees * (std::numbers::pi_v<ty> / 180.0); 
      X(X() + distance * std::cos(angleInRadians));
      Y(Y() + distance * std::sin(angleInRadians));
      }

   ty distance(MyPoint const& other) const { return distance(*this, other); }

   MyAngle<ty> angle(void) const {
      return std::atan2(this->second, this->first) * (180.0 / std::numbers::pi_v<ty>);
      }

   MyAngle<ty> angle(MyPoint const& other) const {  return angle(*this, other); }

   auto constexpr addDistanceAndAngle(ty const& distance, MyAngle<ty> const& angleInDegrees) const {
      const ty angleInRadians = angleInDegrees * (std::numbers::pi_v<ty> / 180.0); // Umrechnung in Bogenmaﬂ
      return MyPoint<ty>(X() + distance * std::cos(angleInRadians), Y() + distance * std::sin(angleInRadians));
      }


   static ty distance(MyPoint const& a, MyPoint const& b) {
      ty dx = a.first - b.first;
      ty dy = a.second - b.second;
      return std::sqrt(dx * dx + dy * dy);
      }

   static MyAngle<ty> angle(MyPoint const& a, MyPoint const& b) {
      return (b - a).angle();
      }

   static ty manhattan_distance(MyPoint const& a, MyPoint const& b) {
      return std::fabs(a.first - b.first) + std::fabs(a.second - b.second);
      }

   };
