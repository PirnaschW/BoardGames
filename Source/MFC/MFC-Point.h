
namespace BoardGamesMFC
{

  class Point
  {
  public:
    constexpr Point(long x, long y) noexcept : x_(static_cast<Pixel>(x)), y_(static_cast<Pixel>(y)) {}
    constexpr Point(int x, int y) noexcept : x_(static_cast<Pixel>(x)), y_(static_cast<Pixel>(y)) {}
    constexpr Point(Pixel x = 0, Pixel y = 0) noexcept : x_(x), y_(y) {}

  public:
    // cannot be const, or assignment doesn't work
    Pixel x_;
    Pixel y_;
  };

}
