
namespace BoardGamesCore
{

  class Mode;

  class Tile final
  {
  public:
    constexpr Tile(const Location l, const Rect& r, const TileColor& f) noexcept : location_{ l }, tilecolor_{ f }, rect_{ r } {}
    Tile& operator=(const Tile&) = delete;
    bool IsInRect(const Point& p) const noexcept { return rect_.PtInRect(p) != 0; }
    constexpr const Rect& GetRect() const noexcept { return rect_; }
    constexpr const Location& GetLocation() const noexcept { return location_; }
    void Draw(DC* pDC, const Piece& p) const;

  private:
    const Location location_;
    const TileColor& tilecolor_;
    const Rect rect_;
  };
  

  class Layout  // a layout contains the physical layout of the tiles of a board part
  {
  public:
    enum class LayoutType { Light, Dark, Alternating, Small, None };
  protected:
    Layout() = delete;
  public:
    Layout(const BoardPartDimension& d, const BoardPartID b, const LayoutType lt) noexcept;
    virtual ~Layout() noexcept { for (auto& t : tiles_) delete t; }
    void Draw(DC* pDC, const BoardPart* board_, const PieceMapP& pMap, Mode mode_) const;
    virtual void DrawSelected(DC* pDC, const Location& l) const;
    virtual bool GetLocationFromPoint(const Point& p, Location& l) const noexcept;

  protected:
    const BoardPartDimension dim_;
    std::vector<Tile*> tiles_;
  private:
    const LayoutType ltype_;
  };

  class MainLayout : public Layout
  {
  public:
    MainLayout(const BoardPartDimensions& d, LayoutType lt = LayoutType::Alternating) noexcept :
      Layout(d[0], BoardPartID::Stage, lt), stock_(d[1], BoardPartID::Stock, LayoutType::Light), taken_(d[2], BoardPartID::Taken, LayoutType::Small) {}
    virtual ~MainLayout() noexcept {}
    virtual bool GetLocationFromPoint(const Point& p, Location& l) const noexcept override { return Layout::GetLocationFromPoint(p, l) || taken_.GetLocationFromPoint(p, l) || stock_.GetLocationFromPoint(p, l); }
    virtual void Draw(DC* pDC, const Board* board_, Mode mode_) const;

  protected:
    Layout stock_;
    Layout taken_;
  };

}
