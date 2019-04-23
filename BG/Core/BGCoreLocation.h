
namespace BoardGamesCore
{

  class Offset final
  {
    friend class Location;
  public:
    constexpr Offset(int dx, int dy) noexcept : _dx{ dx }, _dy{ dy } {}
    constexpr inline bool operator==(const Offset& o) const noexcept { return o._dx == _dx && o._dy == _dy; }
    constexpr inline const Offset operator*(int i) const noexcept { return Offset(_dx * i, _dy * i); }

  private:
    const int _dx;
    const int _dy;

  public:
    static const Offset Rdirection[4];   // standard 4 'Rook'   directions
    static const Offset Bdirection[4];   // standard 4 'Bishop' directions
    static const Offset Qdirection[8];   // standard 8 'Queen'  directions
  };
  static_assert(!std::is_trivially_constructible<class Offset>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Offset, int, int>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Offset, int, int>::value, "is not nothrow constructible");

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

}