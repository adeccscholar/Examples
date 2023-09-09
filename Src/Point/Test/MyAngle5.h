#pragma once


#include <iostream>
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

      template <MyAngleKind other_kind = kind, typename = std::enable_if_t<other_kind == MyAngleKind::degree>>
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

