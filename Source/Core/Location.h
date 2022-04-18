// dependent on Offset

namespace BoardGamesCore
{

  class Location final
  {
  public:
    enum class BoardPartID : unsigned char {
      Stage = 0x01,  // main playing board
      Taken = 0x02,  // taken pieces
      Stock = 0x03,  // piece stock
    };

  public:
    constexpr Location(BoardPartID b, Coordinate x, Coordinate y) noexcept : b_{ b }, x_{ x }, y_{ y } {}
    constexpr ~Location() = default;
    constexpr Location(const Location& l) = default;              // allow copy construction
    constexpr Location(Location&& l) = default;                   // allow move construction
    constexpr Location& operator = (const Location& l) = default; // allow copy assignments
    constexpr Location& operator = (Location&& l) = default;      // allow move assignments

    constexpr bool operator == (const Location& l) const noexcept { return l.x_ == x_ && l.y_ == y_ && l.b_ == b_; }
    constexpr bool operator != (const Location& l) const noexcept { return !(l == *this); }
    constexpr Location operator + (const Offset & o) const noexcept { Location l(*this); return l += o; }
    constexpr Location& operator += (const Offset& o) noexcept
    {
      x_ = x_ + o.dx_;
      y_ = y_ + o.dy_;
      return *this;
    }

  public:
    // can't be protected, as the values are used in many places (too cumbersome to use GetX)
    // can't be const, or operator += wouldn't work.
    Coordinate x_;
    Coordinate y_;
    BoardPartID b_;
  };
  using BoardPartID = Location::BoardPartID;

// can only be defined after Location is fully known:
  constexpr inline Offset::Offset(const Location& l1, const Location& l2) noexcept : dx_{ l2.x_ - l1.x_ }, dy_{ l2.y_ - l1.y_ } {}

  static_assert(!std::is_abstract<Location>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Location>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class Location, int, int>::value, "must not be 'int' constructible");
  static_assert(std::is_constructible<class Location, BoardPartID, Coordinate, Coordinate>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Location, BoardPartID, Coordinate, Coordinate>::value, "is not nothrow constructible");
  static_assert(Location(BoardPartID::Stage,0,0) == Location(BoardPartID::Stage, 0, 0), "Locations can't be constexpr");

}