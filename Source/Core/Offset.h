
namespace BoardGamesCore
{

  class Location;

  class Offset final
  {
    friend class BoardGamesCore::Location;
  public:
    constexpr Offset(int dx, int dy) noexcept : dx_{ dx }, dy_{ dy } {}
    Offset(const Location& l1, const Location& l2) noexcept;
    constexpr bool operator==(const Offset& o) const noexcept { return o.dx_ == dx_ && o.dy_ == dy_; }
    constexpr Offset operator*(int i) const noexcept { return Offset(dx_ * i, dy_ * i); }
    constexpr Offset rotate(bool p) const noexcept { return p ? Offset(-dy_, dx_) : Offset(dy_, -dx_); } // rotate mathematically positive (p=true) or negative (p=false)
    constexpr bool IsParallel(const Offset& o) const noexcept { return dx_ * o.dy_ == o.dx_ * dy_; }  // 2-dim cross product is 0 when parallel

  private:
    const int dx_;
    const int dy_;

  public:
    static const Offset RDirection[4];   // standard 4 'Rook'   directions
    static const Offset BDirection[4];   // standard 4 'Bishop' directions
    static const Offset QDirection[8];   // standard 8 'Queen'  directions
    static const Offset NJumps[8];       // standard 8 'Night'  jumps
  };

}
