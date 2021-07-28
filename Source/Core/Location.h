
namespace BoardGamesCore
{

  class Location final
  {
  public:
    enum class BoardPart : unsigned char {
      Main  = 0x01,  // main playing board
      Taken = 0x02,  // taken pieces
      Stock = 0x03,  // piece stock
    };

  public:
    constexpr Location(BoardPart b, Coordinate x, Coordinate y) noexcept : b_{ b }, x_{ x }, y_{ y } {}
    //Location(unsigned int, unsigned int) noexcept = delete;
//    Location(int, int) noexcept = delete;
    //Location(char, char) noexcept = delete;

    constexpr bool operator==(const Location& l) const noexcept { return l.b_ == b_ && l.x_ == x_ && l.y_ == y_; }
    constexpr bool operator!=(const Location& l) const noexcept { return !(l == *this); }
    constexpr Location operator+(const Offset & o) const noexcept { Location l(*this); return l += o; }
    constexpr Location& operator+=(const Offset& o) noexcept
    {
//      assert(o.dx_ >= 0 || -o.dx_ <= x_);
      x_ += o.dx_;
//      assert(o.dy_ >= 0 || -o.dy_ <= y_);
      y_ += o.dy_;
      return *this;
    }

    constexpr bool Valid(Coordinate sizeX_, Coordinate sizeY_) const noexcept { return x_ >= 0 && x_ < sizeX_&& y_ >= 0 && y_ < sizeY_; }
    constexpr unsigned int Index(Coordinate sizeX_, Coordinate /*sizeY_*/) const noexcept { return y_ * sizeX_ + x_; }

  public:
    // can't be protected, as the values need to be used in many places
    // can't be const, or assignments between Locations wouldn't work.
    Coordinate x_;
    Coordinate y_;
    BoardPart  b_;
  };
  using BoardPart = Location::BoardPart;

}