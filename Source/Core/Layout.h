
namespace BoardGamesCore
{
  //using BoardGamesMFC::Point;
  //using BoardGamesMFC::Rect;
  //using BoardGamesMFC::DC;

  //export class Location;
  //export class TileColor;
  //export class Piece;
  class _Mode;

  class Tile final
  {
  public:
    constexpr Tile(const Location l, const Rect& r, const TileColor& f) noexcept : location_{ l }, tilecolor_{ f }, rect_{ r } {}
    Tile& operator=(const Tile&) = delete;
    bool InRect(const Point& p) const noexcept { return rect_.PtInRect(p) != 0; }
    constexpr const Rect& GetRect(void) const noexcept { return rect_; }
    constexpr const Location& GetLocation(void) const noexcept { return location_; }
    void Draw(DC* pDC, const Piece& p) const;

  private:
    const Location location_;
    const TileColor& tilecolor_;
    Rect rect_;
  };
  

  class Layout  // a layout contains the physical layout of the tiles of the playing board
  {
  public:
    enum class LayoutType { Light, Dark, Alternating, Small, None };
  protected:
    Layout(void) = delete;
  public:
    Layout(const Dimension& d, const BoardPart b, const LayoutType lt) noexcept;
    virtual ~Layout() noexcept { for (auto& t : tiles_) delete t; }
    void Draw(DC* pDC, const Position* pos, _Mode mode) const;
    virtual void DrawSelected(DC* pDC, const Location& l) const;
    virtual bool GetLocation(const Point& p, Location& l) const noexcept;

  protected:
    const Dimension dim_;
    std::vector<Tile*> tiles_;
  private:
    const LayoutType ltype_;
  };

  class MainLayout : public Layout
  {
  public:
    MainLayout(const Dimensions& d, LayoutType lt = LayoutType::Alternating) noexcept :
      Layout(d[0], BoardPart::Main, lt), stock_(d[1], BoardPart::Stock, LayoutType::Light), taken_(d[2], BoardPart::Taken, LayoutType::Small) {}
    virtual ~MainLayout() noexcept {}
    virtual bool GetLocation(const Point& p, Location& l) const noexcept override { return Layout::GetLocation(p, l) || taken_.GetLocation(p, l) || stock_.GetLocation(p, l); }
    virtual void Draw(DC* pDC, const MainPosition* pos, _Mode mode) const;

  protected:
    Layout stock_;
    Layout taken_;
  };

}
