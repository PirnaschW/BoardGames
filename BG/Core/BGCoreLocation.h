
namespace BoardGamesCore
{

  typedef unsigned int Coordinate;

  class Location final
  {
  public:
    constexpr inline Location(Coordinate x, Coordinate y) noexcept : _x{ x }, _y{ y } {}
    Location(int, int) noexcept = delete;

    constexpr inline bool operator==(const Location& l) const noexcept { return l._x == _x && l._y == _y; }
    constexpr inline bool operator!=(const Location& l) const noexcept { return !(l == *this); }
    constexpr inline Location operator+(const Offset & o) const noexcept { Location l(*this); return l += o; }
    constexpr inline Location& operator+=(const Offset & o) noexcept { _x += o._dx, _y += o._dy; return *this; }

    constexpr inline bool Valid(Coordinate sizeX, Coordinate sizeY) const noexcept { return _x >= 0 && _x < sizeX && _y >= 0 && _y < sizeY; }
    constexpr inline Coordinate Index(Coordinate sizeX, Coordinate /*sizeY*/) const noexcept { return _y * sizeX + _x; }

  public:
    // can't be protected, as the values need to be used in many places
    // can't be const, or assignments between Locations wouldn't work.
    Coordinate _x;
    Coordinate _y;
  };

  static_assert(!std::is_abstract<Location>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Location>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class Location, int, int>::value, "must not be 'int' constructible");
  static_assert(std::is_constructible<class Location, Coordinate, Coordinate>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Offset, Coordinate, Coordinate>::value, "is not nothrow constructible");

}