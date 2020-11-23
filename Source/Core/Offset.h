
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
    const Offset operator*(int i) const noexcept;
    constexpr bool IsParallel(const Offset& o) const noexcept { return dx_ * o.dy_ == o.dx_ * dy_; }  // 2-dim cross product is 0 when parallel

  private:
    const int dx_;
    const int dy_;

  public:
    static const Offset Rdirection[4];   // standard 4 'Rook'   directions
    static const Offset Bdirection[4];   // standard 4 'Bishop' directions
    static const Offset Qdirection[8];   // standard 8 'Queen'  directions
  };

}
