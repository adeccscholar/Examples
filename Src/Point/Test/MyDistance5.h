#pragma once


#include <iostream>
#include <cmath>
#include <string>
#include <numbers>
#include <type_traits>
#include <format>

using namespace std::string_literals;

enum class MyDistanceKind : int { without, meter, kilometer, miles, yards, foot, inchs };

template<typename T>
inline constexpr bool always_false_distance = false;


template <std::floating_point ty = double, MyDistanceKind kind = MyDistanceKind::without>
class MyDistance {
   friend std::ostream& operator << (std::ostream& out, MyDistance<ty, kind> const& dist) {
      return out << dist.Distance() << " " << dist.unit();
      }

public:
   constexpr MyDistance(void) = default;

   template <typename OtherTy>
      requires std::is_floating_point_v<OtherTy> || (std::is_integral_v<OtherTy> && !std::is_same_v<OtherTy, bool>)
   constexpr MyDistance(OtherTy const& val) : flDistance(static_cast<ty>(val)) {}

   constexpr MyDistance(MyDistance const& other) {
      flDistance = other.flDistance;
      }

   template <MyDistanceKind other_kind, std::floating_point OtherTy = ty>
   constexpr MyDistance(MyDistance<OtherTy, other_kind> const& other) {
      flDistance = other.convert_to<kind>();
      }


   template <MyDistanceKind other_kind, std::floating_point OtherTy = ty>
   MyDistance& operator = (MyDistance<OtherTy, other_kind> const& other) {
      flDistance = other.convert_to<kind>();
      return *this;
      }

   MyDistance(MyDistance&&) noexcept = default;

   template <MyDistanceKind other_kind, std::floating_point OtherTy = ty>
   operator MyDistance<OtherTy, other_kind> () const {
      MyDistance<OtherTy, other_kind> val;
      val.convert_from<ty>(flDistance);
      return val;
      }

   auto operator <=> (MyDistance const& other) const {
      static const double epsilon = 1e-6; // Toleranz für Rundungsdifferenzen
      if (std::fabs(flDistance - other.flDistance) < epsilon) return std::strong_ordering::equal;
      else if (flDistance < other.flDistance) return std::strong_ordering::less;
           else return std::strong_ordering::greater;
      }

   template <MyDistanceKind other_kind, std::floating_point OtherTy = ty>
   bool operator== (const MyDistance<OtherTy, other_kind>& other) const {
      MyDistance<ty, kind> compare_value(other);
      return (*this <=> compare_value) == 0;
      }

   constexpr ty const& Distance(void) const { return flDistance;  }
   void Distance(ty const& val) { flDistance = val;  }

   operator std::string () const {
      if constexpr (kind == MyDistanceKind::meter)          return std::format("{} m", flDistance);
      else if constexpr (kind == MyDistanceKind::kilometer) return std::format("{} km", flDistance);
      else if constexpr (kind == MyDistanceKind::miles)     return std::format("{} mi", flDistance);
      else if constexpr (kind == MyDistanceKind::yards)     return std::format("{} yd", flDistance);
      else if constexpr (kind == MyDistanceKind::foot)      return std::format("{} ft", flDistance);
      else if constexpr (kind == MyDistanceKind::inchs)     return std::format("{} in", flDistance);
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
         flDistance = val;
         }
      else if constexpr (kind == MyDistanceKind::without || other_kind == MyDistanceKind::without) {
         static_assert(always_false_distance<kind>, "a distance without unit can't converted to or from a distance with unit");
         }
      else {
         if constexpr (kind == MyDistanceKind::meter) {
            if constexpr (other_kind == MyDistanceKind::kilometer) {
               flDistance = val * 1000.0;
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               flDistance = val * 1609.34;
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               flDistance = val * 0.9144;
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               flDistance = val * 0.3048;
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               flDistance = val * 0.0254;
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::kilometer) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               flDistance = val / 1000.0;
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               flDistance = val * 1.60934;
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               flDistance = val * 0.0009144;
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               flDistance = val * 0.0003048;
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               flDistance = val * 0.0000254;
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::miles) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               flDistance = val * 0.000621371;
               }
            else if constexpr (other_kind == MyDistanceKind::kilometer) {
               flDistance = val * 0.621371;
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               flDistance = val * 0.000568182;
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               flDistance = val * 0.000189394;
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               flDistance = val * 0.000015783;
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::yards) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               flDistance = val * 1.09361;
               }
            else if constexpr (other_kind == MyDistanceKind::kilometer) {
               flDistance = val * 1093.61;
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               flDistance = val * 1760.0;
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               flDistance = val * 0.333333;
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               flDistance = val * 0.0277778;
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::foot) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               flDistance = val * 3.28084;
               }
            else if constexpr (other_kind == MyDistanceKind::kilometer) {
               flDistance = val * 3280.84;
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               flDistance = val * 5280.0;
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               flDistance = val * 3.0;
               }
            else if constexpr (other_kind == MyDistanceKind::inchs) {
               flDistance = val * 0.0833333;
               }
            else {
               static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
               }
            }
         else if constexpr (kind == MyDistanceKind::inchs) {
            if constexpr (other_kind == MyDistanceKind::meter) {
               flDistance = val * 39.3701;
               }
            else if constexpr (other_kind == MyDistanceKind::kilometer) {
               flDistance = val * 39370.1;
               }
            else if constexpr (other_kind == MyDistanceKind::miles) {
               flDistance = val * 63360.0;
               }
            else if constexpr (other_kind == MyDistanceKind::yards) {
               flDistance = val * 36.0;
               }
            else if constexpr (other_kind == MyDistanceKind::foot) {
               flDistance = val * 12.0;
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

      template <MyDistanceKind other_kind>
      ty constexpr convert_to() const {
         if constexpr (kind == other_kind) {
            return flDistance;
            }
         else if constexpr (kind == MyDistanceKind::without || other_kind == MyDistanceKind::without) {
            static_assert(always_false_distance<kind>, "a distance without unit can't be converted to or from a distance with unit");
            }
         else {
            if constexpr (kind == MyDistanceKind::meter) {
               if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return flDistance / 1000.0;
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return flDistance / 1609.34;
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return flDistance / 0.9144;
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return flDistance / 0.3048;
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return flDistance / 0.0254;
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::kilometer) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return flDistance * 1000.0;
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return flDistance * 1.60934;
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return flDistance * 1093.61;
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return flDistance * 3280.84;
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return flDistance * 39370.1;
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::miles) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return flDistance * 1609.34;
                  }
               else if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return flDistance * 1.60934;
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return flDistance * 1760.0;
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return flDistance * 5280.0;
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return flDistance * 63360.0;
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::yards) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return flDistance * 0.9144;
                  }
               else if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return flDistance * 0.0009144;
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return flDistance * 0.000568182;
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return flDistance * 3.0;
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return flDistance * 36.0;
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::foot) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return flDistance * 0.3048;
                  }
               else if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return flDistance * 0.0003048;
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return flDistance * 0.000189394;
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return flDistance * 0.333333;
                  }
               else if constexpr (other_kind == MyDistanceKind::inchs) {
                  return flDistance * 12.0;
                  }
               else {
                  static_assert(always_false_distance<other_kind>, "unexpected unit for distance");
                  }
               }
            else if constexpr (kind == MyDistanceKind::inchs) {
               if constexpr (other_kind == MyDistanceKind::meter) {
                  return flDistance * 0.0254;
                  }
               else if constexpr (other_kind == MyDistanceKind::kilometer) {
                  return flDistance * 0.0000254;
                  }
               else if constexpr (other_kind == MyDistanceKind::miles) {
                  return flDistance * 0.000015783;
                  }
               else if constexpr (other_kind == MyDistanceKind::yards) {
                  return flDistance * 0.0277778;
                  }
               else if constexpr (other_kind == MyDistanceKind::foot) {
                  return flDistance * 0.0833333;
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
      ty flDistance;
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
