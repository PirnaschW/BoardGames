
namespace BoardGamesCore
{

  class Offset final
  {
    friend class Location;
  public:
    constexpr Offset(int dx, int dy) noexcept : dx_{ dx }, dy_{ dy } {}
    Offset(const Location& l1, const Location& l2) noexcept;
    constexpr inline bool operator==(const Offset& o) const noexcept { return o.dx_ == dx_ && o.dy_ == dy_; }
    constexpr inline const Offset operator*(int i) const noexcept { return Offset(dx_ * i, dy_ * i); }
    constexpr inline bool IsParallel(const Offset& o) const noexcept { return dx_ * o.dy_ - o.dx_ * dy_ == 0; }    // 2-dim cross product is 0 when parallel

  private:
    const int dx_;
    const int dy_;

  public:
    static const Offset Rdirection[4];   // standard 4 'Rook'   directions
    static const Offset Bdirection[4];   // standard 4 'Bishop' directions
    static const Offset Qdirection[8];   // standard 8 'Queen'  directions
  };
  static_assert(!std::is_abstract<Offset>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Offset>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Offset, int, int>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Offset, int, int>::value, "is not nothrow constructible");

}
