
namespace BoardGamesCore
{

  class Location;

  class Offset final
  {
    friend class BoardGamesCore::Location;
  private:
    constexpr Offset() noexcept = delete;

  public:
    constexpr Offset(Coordinate dx, Coordinate dy) noexcept : dx_{ dx }, dy_{ dy } {}
    constexpr Offset(const Location& l1, const Location& l2) noexcept;  // not constexpr as we can't access Location yet
    constexpr bool operator==(const Offset& o) const noexcept { return o.dx_ == dx_ && o.dy_ == dy_; }
    constexpr bool operator!=(const Offset& o) const noexcept { return !(*this == o); }
    constexpr Offset operator*(int i) const noexcept { return Offset(dx_ * i, dy_ * i); }
    constexpr Offset Rotate(bool p = true) const noexcept { return p ? Offset(-dy_, dx_) : Offset(dy_, -dx_); } // rotate mathematically positive (p=true) or negative (p=false)
    constexpr bool IsParallel(const Offset& o) const noexcept { return dx_ * o.dy_ == o.dx_ * dy_; }  // 2-dim cross product is 0 when parallel

  private:
    const Coordinate dx_;
    const Coordinate dy_;

  public:
    static const Offset RDirection[4];  // standard 4 'Rook'   directions
    static const Offset BDirection[4];  // standard 4 'Bishop' directions
    static const Offset QDirection[8];  // standard 8 'Queen'  directions
    static const Offset NJumps    [8];  // standard 8 'Night'  jumps

  };

                                        // standard 4 'Rook'   directions
  constexpr inline const Offset Offset::RDirection[4]{ Offset(+1, +0), Offset(+0, +1),
                                                       Offset(+0, -1), Offset(-1, +0) };
                                        // standard 4 'Bishop' directions
  constexpr inline const Offset Offset::BDirection[4]{ Offset(+1, +1), Offset(+1, -1),
                                                       Offset(-1, -1), Offset(-1, +1) };
                                        // standard 8 'Queen'  directions
  constexpr inline const Offset Offset::QDirection[8]{ Offset(+1, +1), Offset(+1, +0), Offset(+1, -1),
                                                       Offset(+0, +1),                 Offset(+0, -1),
                                                       Offset(-1, +1), Offset(-1, +0), Offset(-1, -1) };
                                        // standard 8 'Knight' jumps
  constexpr inline const Offset Offset::NJumps    [8]{ Offset(+2, +1), Offset(+2, -1),
                                                       Offset(+1, +2), Offset(+1, -2),
                                                       Offset(-1, +2), Offset(-1, -2),
                                                       Offset(-2, +1), Offset(-2, -2) };

  static_assert(!std::is_abstract<Offset>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Offset>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Offset, Coordinate, Coordinate>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Offset, Coordinate, Coordinate>::value, "is not nothrow constructible");
  static_assert(Offset::RDirection[0] != Offset::BDirection[0], "Offset singletons are not constexpr");

}
