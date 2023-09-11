#pragma once


#include <iostream>
#include <cmath>
#include <string>
#include <numbers>
#include <type_traits>
#include <format>

using namespace std::string_literals;

enum class MyDistanceKind : int { without, meter, kilometer, miles, yards, foot, inchs };

template<typename ty>
inline constexpr bool always_false_distance = false;

template <typename ty>
concept my_param_distance = std::is_floating_point_v<ty> || (std::is_integral_v<ty> && !std::is_same_v<ty, bool>);

template <my_param_distance ty = double, MyDistanceKind kind = MyDistanceKind::without>
class MyDistance {
   friend std::ostream& operator << (std::ostream& out, MyDistance<ty, kind> const& dist) {
      return out << dist.Distance() << " " << dist.unit();
      }

public:
   constexpr MyDistance(void) = default;

   template <my_param_distance OtherTy>
   constexpr MyDistance(OtherTy const& val) : theDistance(static_cast<ty>(val)) {}

   constexpr MyDistance(MyDistance const& other) {
      theDistance = other.theDistance;
      }

   template <MyDistanceKind other_kind, my_param_distance OtherTy = ty>
   constexpr MyDistance(MyDistance<OtherTy, other_kind> const& other) {
      theDistance = other.convert_to<kind>();
      }


   template <MyDistanceKind other_kind, my_param_distance OtherTy = ty>
   MyDistance& operator = (MyDistance<OtherTy, other_kind> const& other) {
      theDistance = other.convert_to<kind, ty>();
      return *this;
      }

   MyDistance(MyDistance&&) noexcept = default;

   template <my_param_distance OtherTy, MyDistanceKind other_kind>
   operator MyDistance<OtherTy, other_kind> ()  {
      return MyDistance<OtherTy, other_kind>(*this);
      }

   auto operator <=> (MyDistance const& other) const {
      if constexpr (std::is_floating_point_v<ty> == true) {
         static const double epsilon = 1e-6; // Toleranz für Rundungsdifferenzen
         if (std::fabs(theDistance - other.theDistance) < epsilon) return std::strong_ordering::equal;
         else if (theDistance < other.theDistance) return std::strong_ordering::less;
              else return std::strong_ordering::greater;
         }
      else {
         return theDistance <=> other.theDistance;
         }
      }

   template <MyDistanceKind other_kind, my_param_distance OtherTy = ty>
   bool operator== (const MyDistance<OtherTy, other_kind>& other) const {
      MyDistance<ty, kind> compare_value(other);
      return (*this <=> compare_value) == 0;
      }

   constexpr ty const& Distance(void) const { return theDistance;  }
   void Distance(ty const& val) { theDistance = val;  }

   operator std::string () const {
      if constexpr (kind == MyDistanceKind::meter)          return std::format("{} m", theDistance);
      else if constexpr (kind == MyDistanceKind::kilometer) return std::format("{} km", theDistance);
      else if constexpr (kind == MyDistanceKind::miles)     return std::format("{} mi", theDistance);
      else if constexpr (kind == MyDistanceKind::yards)     return std::format("{} yd", theDistance);
      else if constexpr (kind == MyDistanceKind::foot)      return std::format("{} ft", theDistance);
      else if constexpr (kind == MyDistanceKind::inchs)     return std::format("{} in", theDistance);
      else {
         static_assert(always_false_distance<kind>, "unexpected unit for distance");
         }
      }

   std::string unit(void) const {
      if constexpr (kind == MyDistanceKind::meter)          return "m"s;
      else if constexpr (kind == MyDistanceKind::kilometer) return "km"s;
      else if constexpr (kind == MyDistanceKind::miles)     return "mi"s;
      else if constexpr (kind == MyDistanceKind::yards)     return "yd"s;
      else if constexpr (kind == MyDistanceKind::foot)      return "ft"s;
      else if constexpr (kind == MyDistanceKind::inchs)     return "in"s;
      else {
         static_assert(always_false_distance<kind>, "unexpected unit for distance");
         }
      }

   template<MyDistanceKind other_kind>
   void constexpr convert_from(ty const& val) {
      if constexpr (kind == other_kind) {
         theDistance = val;
         }
      else if constexpr (kind == MyDistanceKind::without || other_kind == MyDistanceKind::without) {
         static_assert(always_false_distance<kind>, "a distance without unit can't converted to or from a distance with unit");
         }
      else {
         if constexpr (kind == MyDistanceKind::meter) {
            if constexpr (other_kind == MyDistanceKind::kilometer) {
               theDistance = set_value<ty>(val * 1000);
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               theDistance = set_value<ty>(val * 1609.34);
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               theDistance = set_value<ty>(val * 0.9144);
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               theDistance = set_value<ty>(val * 0.3048);
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               theDistance = set_value<ty>(val * 0.0254);
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::kilometer) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               theDistance = set_value<ty>(val / 1000);
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               theDistance = set_value<ty>(val * 1.60934);
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               theDistance = set_value<ty>(val * 0.0009144);
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               theDistance = set_value<ty>(val * 0.0003048);
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               theDistance = set_value<ty>(val * 0.0000254);
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::miles) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               theDistance = set_value<ty>(val * 0.000621371);
               }
            else if constexpr (other_kind == MyDistanceKind::kilometer) {
               theDistance = set_value<ty>(val * 0.621371);
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               theDistance = set_value<ty>(val / 1760);
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               theDistance = set_value<ty>(val / (3 * 1760));
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               theDistance = set_value<ty>(val / (12 * 3 * 1760));
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::yards) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               theDistance = set_value<ty>(val * 1.09361);
               }
            else if constexpr (other_kind == MyDistanceKind::kilometer) {
               theDistance = set_value<ty>(val * 1093.61);
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               theDistance = set_value<ty>(val * 1760);
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               theDistance = set_value<ty>(val / 3);
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               theDistance = set_value<ty>(val / (12 * 3));
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::foot) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               theDistance = set_value<ty>(val * 3.28084);
               }
            else if constexpr (other_kind == MyDistanceKind::kilometer) {
               theDistance = set_value<ty>(val * 3280.84);
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               theDistance = set_value<ty>(val * (3 * 1760));
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               theDistance = set_value<ty>(val * 3);
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               theDistance = set_value<ty>(val / 12);
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::inchs) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               theDistance = set_value<ty>(val * 39.3701);
               }
            else if constexpr (other_kind == MyDistanceKind::kilometer) {
               theDistance = set_value<ty>(val * 39370.1);
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               theDistance = set_value<ty>(val * (12 * 3 * 1760));
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               theDistance = set_value<ty>(val * (12 * 3));
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               theDistance = set_value<ty>(val * 12);
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else {
            static_assert(always_false_distance<kind>, "unexpected unit for distance");
            }
         }
      }


      template <MyDistanceKind other_kind, my_param_distance ret_ty = ty>
      ret_ty constexpr convert_to() const {
         if constexpr (kind == other_kind) {
            return set_value<ret_ty>(theDistance);
            }
         else if constexpr (kind == MyDistanceKind::without || other_kind == MyDistanceKind::without) {
            static_assert(always_false_distance<kind>, "a distance without unit can't be converted to or from a distance with unit");
            }
         else {
            if constexpr (kind == MyDistanceKind::meter) {
               if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return set_value<ret_ty>(theDistance / 1000);
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return set_value<ret_ty>(theDistance / 1609.34);
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return set_value<ret_ty>(theDistance / 0.9144);
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return set_value<ret_ty>(theDistance / 0.3048);
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return set_value<ret_ty>(theDistance / 0.0254);
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::kilometer) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return set_value<ret_ty>(theDistance * 1000);
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return set_value<ret_ty>(theDistance * 1.60934);
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return set_value<ret_ty>(theDistance * 1093.61);
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return set_value<ret_ty>(theDistance * 3280.84);
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return set_value<ret_ty>(theDistance * 39370.1);
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::miles) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return set_value<ret_ty>(theDistance * 1609.34);
                  }
               else if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return set_value<ret_ty>(theDistance * 1.60934);
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return set_value<ret_ty>(theDistance * 1760);
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return set_value<ret_ty>(theDistance * (3 * 1760));
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return set_value<ret_ty>(theDistance * (12 * 3 * 1760));
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::yards) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return set_value<ret_ty>(theDistance * 0.9144);
                  }
               else if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return set_value<ret_ty>(theDistance * 0.0009144);
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return set_value<ret_ty>(theDistance / 1760);
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return set_value<ret_ty>(theDistance * 3);
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return set_value<ret_ty>(theDistance * (3 * 12));
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::foot) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return set_value<ret_ty>(theDistance * 0.3048);
                  }
               else if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return set_value<ret_ty>(theDistance * 0.0003048);
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return set_value<ret_ty>(theDistance / (3 * 1760));
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return set_value<ret_ty>(theDistance / 3);
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return set_value<ret_ty>(theDistance * 12);
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::inchs) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return set_value<ret_ty>(theDistance * 0.0254);
                  }
               else if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return set_value<ret_ty>(theDistance * 0.0000254);
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return set_value<ret_ty>(theDistance / (12 * 3 * 1760));
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return set_value<ret_ty>(theDistance / (12 * 3));
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return set_value<ret_ty>(theDistance / 12);
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else {
               static_assert(always_false_distance<kind>, "unexpected unit for distance");
               }
         }
       //  return static_cast<ty>(0); // Default value if none of the conditions match, 
      }

   private:
      /// method to convert different types with rules
      template <my_param_distance ret_ty, typename other_ty>
      auto set_value(other_ty&& val) const -> std::conditional_t<std::is_same_v<other_ty, ret_ty>, ret_ty&&, ret_ty>  {
         if constexpr (std::is_integral_v<ret_ty> && !std::is_integral_v<other_ty>) {
            return static_cast<ret_ty>(std::round(val));
            }
         else if constexpr (!std::is_same_v<ret_ty, other_ty>) {
            return static_cast<ret_ty>(val);
            }
         else {
            return std::move(val);
            }
         }

      ty theDistance;
   };

MyDistance<double, MyDistanceKind::meter> operator""_meter(long double val) {
      return MyDistance<double, MyDistanceKind::meter>(static_cast<double>(val));
   }


MyDistance<double, MyDistanceKind::yards> operator""_yards(long double val) {
   return MyDistance<double, MyDistanceKind::yards>(static_cast<double>(val));
   }

/*
   MyAngle<double> operator""_rad(long double val) {
      MyAngle<double> tmp;
      tmp.convert_from<MyAngleKind::radian>(static_cast<double>(val));
      return tmp;
   }
*/

/*
enum class MyAngleKind : int { degree, radian };

template<typename T>
inline constexpr bool always_false_angle = false;

template <std::floating_point ty = double, MyAngleKind kind = MyAngleKind::degree>
class MyAngle {
   friend std::ostream& operator << (std::ostream& out, MyAngle<ty, kind> const& angle) {
      return out << angle.flAngle << " " << angle.unit();
      }

   public:
      MyAngle(void) = default;

      template <typename OtherTy>
         requires std::is_floating_point_v<OtherTy> || 
                  (std::is_integral_v<OtherTy> && !std::is_same_v<OtherTy, bool>)
      MyAngle(OtherTy const& val) : flAngle(static_cast<ty>(val)) {}

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


*/
