
namespace BoardGamesMFC
{

  class Point
  {
  public:
    constexpr Point(int x=0, int y=0) noexcept : x_(x), y_(y) {}

  public:
    // cannot be const, or assignment doesn't work
    int x_;
    int y_;
  };

}
