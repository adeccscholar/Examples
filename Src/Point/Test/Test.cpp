
#include <iostream>
#include <iomanip>
#include <sstream>
#include <type_traits>
#include <utility>
#include <cmath>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <numbers>
#include <charconv>
#include <ranges>

#include "MyDistance5.h"
#include "MyPoint5.h"

using namespace std::string_view_literals;

enum class MyGeodeticRefType : int { WGS84, ETRS89, MercatorETRS89 };

template <typename ty>
concept my_integral_param = std::is_integral_v<ty> && !std::is_same_v<ty, bool>;

template <my_integral_param ty>
class MySexagesimalAngle : public std::tuple<ty, ty, ty>  {
   
   friend std::ostream& operator << (std::ostream& out, MySexagesimalAngle<ty> const data) {
      return out << std::setw(3) << std::right << data.Degrees() << "° "
                 << std::setw(2) << std::right << data.Minutes() << "\' "
                 << std::setw(2) << std::right << data.Seconds() << "\"";
      }
      
   public:
      MySexagesimalAngle(void) = default;
      MySexagesimalAngle(MySexagesimalAngle const&) = default;
      MySexagesimalAngle(MySexagesimalAngle&&) noexcept = default;

      MySexagesimalAngle(ty const& deg, ty const& min, ty const& sec) {
         std::get<0>(*this) = deg;
         std::get<1>(*this) = min;
         std::get<2>(*this) = sec;
         }

      MySexagesimalAngle(std::string const& input) { Parse(input);  }

      template <std::floating_point para_ty>
      MySexagesimalAngle(para_ty const& deg_dec) : MySexagesimalAngle<ty>() {
         DegreesAsDecimal(deg_dec);
         }

      ~MySexagesimalAngle(void) = default;

      MySexagesimalAngle& operator = (MySexagesimalAngle const&) = default;
      MySexagesimalAngle& operator = (MySexagesimalAngle&&) noexcept = default;

      template <std::floating_point param_ty = double>
      operator param_ty () const { return DegreesAsDecimal(); }
      operator std::string() const { return std::format("{:3d}° {:2d}\' {:3d}\"", Degrees(), Minutes(), Seconds()); }

      auto operator <=> (MySexagesimalAngle const&) const = default;

      ty const& Degrees(void) const { return std::get<0>(*this); }
      ty const& Minutes(void) const { return std::get<1>(*this); }
      ty const& Seconds(void) const { return std::get<2>(*this); }

      template <std::floating_point param_ty = double>
      param_ty const& DegreesAsDecimal(void) const { return static_cast<param_ty>((Seconds() / 60.0 + Minutes()) / 60.0 + Degrees()); }

      void Degrees(ty const& val) { std::get<0>(*this) = val;  }
      void Minutes(ty const& val) { std::get<1>(*this) = val; }
      void Seconds(ty const& val) { std::get<2>(*this) = val; }

      template <std::floating_point param_ty = double>
      void DegreesAsDecimal(param_ty const& val)  {
         param_ty tmp { val };
         Degrees(std::floor(tmp));
         tmp -= Degrees();
         tmp *= 60;
         Minutes(std::floor(tmp));
         (tmp -= Minutes()) *= 60;
         Seconds(std::round(tmp));
         }
   private:
      void Parse(std::string const& para) { 
         static auto constexpr special_char = [](char c) { return c == '°' || c == '"' || c == '\''; };

         std::string input { para };
         //std::replace_if(input.begin(), input.end(), special_char, ' ');
         std::ranges::replace_if(input, special_char, ' ');
         
         *this = { 0, 0, 0 };
         switch (auto ret = std::string_view{ input.begin(), input.end() } 
                                  | std::views::split(' ')
                                  | std::views::filter([](auto token) { return !token.empty(); })
                                  | std::views::transform([](auto token)  {
                                            ty num = 0;
                                            for (char c : token) { if (std::isdigit(c)) { (num *= 10) += (c - '0'); } }
                                            return num; 
                                            } )
                                  | std::ranges::to<std::vector>(); ret.size()) {
            case 3: Seconds(ret[2]);
                    [[fallthrough]];
            case 2: Minutes(ret[1]);
                    [[fallthrough]];
            case 1: Degrees(ret[0]);
                    break;
            default: throw std::invalid_argument("invalid argument for parse Angle "s + para + "."s);
            }
         
         }


   };

template <std::floating_point ty>
class MyWGS84 : std::pair<ty, ty> {
   public:
      MyWGS84(void) = default;
      MyWGS84(MyWGS84 const&) = default;
      MyWGS84(MyWGS84&&) noexcept = default;
      MyWGS84(ty const& lon, ty const& lat) : std::pair<ty, ty>(lon, lat) { }

      virtual ~MyWGS84(void) = default;

      MyWGS84& operator = (MyWGS84 const&) = default;
      MyWGS84& operator = (MyWGS84&&) noexcept = default;

      auto operator <=> (MyWGS84 const&) const = default;

      void Longitude(ty const& val) { std::get<0>(*this) = val;  }
      void Latitude(ty const& val) { std::get<1>(*this) = val; }

      ty const& Longitude(void) const { return std::get<0>(*this); }
      ty const& Latitude(void) const { return std::get<1>(*this); }
   };

template <std::floating_point ty>
class MyETRS89 : std::tuple<char, int, ty, ty> {
   public:
      MyETRS89(void) = default;
      MyETRS89(MyETRS89 const&) = default;
      MyETRS89(MyETRS89&&) noexcept = default;
      MyETRS89(char l, int z, ty const& east, ty const& north) : std::tuple<char, int, ty, ty>(l, z, east, north) { }

      virtual ~MyETRS89(void) = default;

      MyETRS89& operator = (MyETRS89 const&) = default;
      MyETRS89& operator = (MyETRS89&&) noexcept = default;

      auto operator <=> (MyETRS89 const&) const = default;

      void Letter(char val) const { std::get<0>(*this) = val; }
      void Zone(int val) const { std::get<1>(*this) = val; }
      void Easting(ty const& val) { std::get<2>(*this) = val; }
      void Northing(ty const& val) { std::get<3>(*this) = val; }

      char      Letter(void) const { return std::get<0>(*this);  }
      int       Zone(void) const { return std::get<1>(*this); }
      ty const& Easting(void) const { return std::get<2>(*this); }
      ty const& Northing(void) const { return std::get<3>(*this); }

   };


template <std::floating_point ty>
class MyMercatorETRS89 : public MyPoint<ty> {// std::pair<ty, ty> {
   public:
      constexpr MyMercatorETRS89(void) = default;
      MyMercatorETRS89(MyMercatorETRS89 const&) = default;
      MyMercatorETRS89(MyMercatorETRS89&&) noexcept = default;
      MyMercatorETRS89(ty const& x, ty const& y) : MyPoint<ty>(x, y) { } //std::pair<ty, ty>(x, y) { }

      virtual ~MyMercatorETRS89(void) = default;

      MyMercatorETRS89& operator = (MyMercatorETRS89 const&) = default;
      MyMercatorETRS89& operator = (MyMercatorETRS89&&) noexcept = default;

      auto operator <=> (MyMercatorETRS89 const&) const = default;

   };


template <std::floating_point ty = double, MyGeodeticRefType _kind = MyGeodeticRefType::WGS84>
using GeoBase = std::conditional_t<(_kind == MyGeodeticRefType::WGS84), MyWGS84<ty>,
                std::conditional_t<(_kind == MyGeodeticRefType::ETRS89), MyETRS89<ty>,
                std::conditional_t<(_kind == MyGeodeticRefType::MercatorETRS89), MyMercatorETRS89<ty>, void>>>;

template <typename T>
struct is_floating_point : std::is_floating_point<T> {};

template <std::floating_point ty = double, MyGeodeticRefType _kind = MyGeodeticRefType::WGS84>
class MyGeoLocation : public GeoBase<ty, _kind> {
   public:
      MyGeoLocation(void) = default;
      MyGeoLocation(MyGeoLocation&&) noexcept = default;
   
      template <typename... Args, typename = std::enable_if_t<(std::conjunction_v<is_floating_point<Args>...>) &&
                                                               (_kind == MyGeodeticRefType::WGS84 || 
                                                                _kind == MyGeodeticRefType::MercatorETRS89) && 
                                                               sizeof...(Args) == 2>>
      MyGeoLocation(Args... args) : GeoBase<ty, _kind>(std::forward<Args>(args)...) {}



      template <typename Arg1, typename Arg2, typename... Rest, 
                typename = std::enable_if_t<std::is_same_v<Arg1, char> && std::is_same_v<Arg2, int> &&
                                            (_kind == MyGeodeticRefType::ETRS89) && sizeof...(Rest) == 2 &&
                                            (std::conjunction_v<is_floating_point<Rest>...>), void>>
      MyGeoLocation(Arg1 arg1, Arg2 arg2, Rest... rest) : GeoBase<ty, _kind>(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), std::forward<Rest>(rest)...) { }


      template <std::floating_point other_ty, MyGeodeticRefType other_kind>
      MyGeoLocation(const MyGeoLocation<other_ty, other_kind>& other) {
         *this = std::move(convert_from(other));
         }

      MyGeoLocation& operator = (MyGeoLocation&&) noexcept = default;

      template <std::floating_point other_ty, MyGeodeticRefType other_kind>
      MyGeoLocation& operator = (const MyGeoLocation<other_ty, other_kind>& other) {
         *this = std::move(convert_from(other));
         return *this;
         }


      template <std::floating_point oth_ty, MyGeodeticRefType oth_knd>
      operator std::conditional_t<std::is_same_v<ty, oth_ty>&& _kind == oth_knd, const MyGeoLocation&, MyGeoLocation<oth_ty, oth_knd>>() const {
         if constexpr (std::is_same_v<ty, oth_ty> && _kind == oth_knd) {
            return *this;
         }
         else {
            return convert_to<oth_ty, oth_knd>(*this);
            }
         }


      double Test(MyGeoLocation<ty, MyGeodeticRefType::WGS84> const& other) {
         return 1.0;
         }

      template <std::floating_point oth_ty, MyGeodeticRefType oth_knd>
      auto convert_from(const MyGeoLocation<oth_ty, oth_knd>& other) 
         -> std::conditional_t<std::is_same_v<ty, oth_ty>&& _kind == oth_knd, MyGeoLocation const&, MyGeoLocation> {
         if constexpr (std::is_same_v<ty, oth_ty> && _kind == oth_knd) { return static_cast<MyGeoLocation const&>(other); }
         else {
            return MyGeoLocation<ty, _kind>();
            }
         /*
         if constexpr (std::is_same_v<ty, double> && std::is_same_v<other_ty, float>) {
            // Konvertierung von float auf double
            return MyGeoLocation(static_cast<ty>(other.Longitude()), static_cast<ty>(other.Latitude()));
            }
         else if constexpr (std::is_same_v<ty, float> && std::is_same_v<other_ty, double>) {
            // Konvertierung von double auf float
            return MyGeoLocation(static_cast<ty>(other.Longitude()), static_cast<ty>(other.Latitude()));
            }
         else {
            // Fehlerfall oder nicht unterstützte Konvertierung
            // Hier können Sie entsprechende Maßnahmen ergreifen, z. B. eine Ausnahme werfen.
            throw std::runtime_error("Ungültige Konvertierung");
            }
         */
         }

      template <std::floating_point other_ty, MyGeodeticRefType other_kind>
      MyGeoLocation<other_ty, other_kind> convert_to() const {
         /*
         if constexpr (std::is_same_v<ty, double> && std::is_same_v<other_ty, float>) {
            // Konvertierung von double auf float
            return MyGeoLocation<other_ty, other_kind>(static_cast<other_ty>(this->Longitude()), static_cast<other_ty>(this->Latitude()));
         }
         else if constexpr (std::is_same_v<ty, float> && std::is_same_v<other_ty, double>) {
            // Konvertierung von float auf double
            return MyGeoLocation<other_ty, other_kind>(static_cast<other_ty>(this->Longitude()), static_cast<other_ty>(this->Latitude()));
         }
         else {
            // Fehlerfall oder nicht unterstützte Konvertierung
            // Hier können Sie entsprechende Maßnahmen ergreifen, z. B. eine Ausnahme werfen.
            throw std::runtime_error("Ungültige Konvertierung");
         }
         */
         return MyGeoLocation<other_ty, other_kind>();
         }
};


// Wir beginnen mit einer einfachen Klasse 

void Conv_test(MyDistance<double, MyDistanceKind::meter> const& val) {
   std::cout << val << "\n";
   }

int main() {
   std::cout << "Systemschicht, entwickeln eigener allgemeiner Klassen!\n";

   MySexagesimalAngle<int> xx("12° 30' 30\""s);
   std::cout << xx << "\n";
   //MySexagesimalAngle<int> xx(std::string("12 30 30"));
   MyAngle<double, MyAngleKind::degree> y(xx.DegreesAsDecimal());
   MyAngle<double, MyAngleKind::degree> x = 0.250_rad;
   std::cout << x << " = " << x.toRadians() << "rad" << "\n";

   MyAngle<double, MyAngleKind::radian> comp_x = x;
   //bool test = (x == static_cast<MyAngle<double, MyAngleKind::degree>>(comp_x));
   bool test = (x == comp_x);
   test = (x == x);

   //MyDistance<double, MyDistanceKind::meter> testX(1000);
   auto testMeter = 1000._meter;
   MyDistance<double, MyDistanceKind::yards> testYard;
   testYard = testMeter;
   
   test = (testMeter == testYard);

   std::cout << testYard << (test ? " = " : " <> ") << testMeter << "\nSize of the Yard elements is: " << sizeof(testYard) << "\n";

   Conv_test(testYard);


   MyGeoLocation<double, MyGeodeticRefType::WGS84> pos1(52.0, 13.0);
   MyGeoLocation<double, MyGeodeticRefType::WGS84> pos2(52.0, 13.5);

   MyGeoLocation<double, MyGeodeticRefType::ETRS89> pos3('E', 5, 52.0, 13.0);
   MyGeoLocation<double, MyGeodeticRefType::MercatorETRS89> pos4(pos3);

   if (pos1 < pos2) std::cout << "yeah\n";
   pos4.Test(pos3);

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
   }
