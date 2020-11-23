
namespace BoardGamesMFC
{
  class Point;

  class Rect
  {
  public:
    constexpr Rect(int l = 0, int t = 0, int r = 0, int b = 0) noexcept : left_(l), top_(t), right_(r), bottom_(b) {}
    constexpr int Width(void) const noexcept { return right_ - left_; }
    constexpr int Height(void) const noexcept { return bottom_ - top_; }
    constexpr bool PtInRect(const Point& p) const noexcept { return p.x_ >= left_ && p.x_ < right_&& p.y_ >= top_ && p.y_ < bottom_; }
    constexpr void DeflateRect(int l = 0, int t = 0, int r = 0, int b = 0) noexcept { left_ += l; top_ += t; right_ -= r; bottom_ -= b; }

  public:
    int left_;
    int top_;
    int right_;
    int bottom_;
  };

}
