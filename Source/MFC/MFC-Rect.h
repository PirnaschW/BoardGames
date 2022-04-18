
namespace BoardGamesMFC
{
  class Point;

  class Rect
  {
  public:
    constexpr Rect(Pixel l = 0, Pixel t = 0, Pixel r = 0, Pixel b = 0) noexcept : left_(l), top_(t), right_(r), bottom_(b) {}
    constexpr Pixel Width() const noexcept { return right_ - left_; }
    constexpr Pixel Height() const noexcept { return bottom_ - top_; }
    constexpr bool PtInRect(const Point& p) const noexcept { return p.x_ >= left_ && p.x_ < right_&& p.y_ >= top_ && p.y_ < bottom_; }
    constexpr void DeflateRect(Pixel l = 0, Pixel t = 0, Pixel r = 0, Pixel b = 0) noexcept { left_ += l; top_ += t; right_ -= r; bottom_ -= b; }

  public:
    Pixel left_;
    Pixel top_;
    Pixel right_;
    Pixel bottom_;
  };

}
