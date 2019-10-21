
namespace BoardGamesCore
{

  class Tile final
  {
  public:
    constexpr inline Tile(const Location& l, const CRect& r, const TileColor* f) noexcept : location_{ l }, tilecolor_{ f }, rect_{ r } {}
    Tile& operator=(const Tile&) = delete;
    inline bool InRect(const CPoint& p) const noexcept { return rect_.PtInRect(p) != 0; }
    constexpr inline const CRect& GetRect(void) const noexcept { return rect_; }
    constexpr inline const Location GetLocation(void) const noexcept { return location_; }
    inline void Draw(CDC* pDC, const Piece& p) const { p.Draw(pDC, rect_, tilecolor_); }

  private:
    const Location location_;
    const TileColor* tilecolor_;
    CRect rect_;
  };
  

  class Layout  // a layout contains the physical layout of the tiles of the playing board
  {
  public:
    enum class LayoutType { Light, Dark, Alternating, Small, None };
  protected:
    Layout(void) = delete;
  public:
    Layout(const Dimension& d, const BoardPart b, const LayoutType lt) noexcept;
    virtual inline ~Layout() noexcept { for (auto& t : tiles_) delete t; }
    void Draw(CDC* pDC, const Position* pos, _Mode mode) const;
    virtual void DrawSelected(CDC* pDC, const Location& l) const;
    virtual bool GetLocation(const CPoint& p, Location& l) const noexcept;

  protected:
    const Dimension dim_;
    std::vector<Tile*> tiles_;
  private:
    const LayoutType ltype_;
  };

  class MainLayout : public Layout
  {
  public:
    inline MainLayout(const Dimensions& d, LayoutType lt = LayoutType::Alternating) noexcept :
      Layout(d[0], BoardPart::Main, lt), stock_(d[1], BoardPart::Stock, LayoutType::Light), taken_(d[2], BoardPart::Taken, LayoutType::Small) {}
    virtual inline ~MainLayout() noexcept {}
    virtual inline bool GetLocation(const CPoint& p, Location& l) const noexcept override { return Layout::GetLocation(p, l) || taken_.GetLocation(p, l) || stock_.GetLocation(p, l); }
    virtual void Draw(CDC* pDC, const MainPosition* pos, _Mode mode) const;

  protected:
    Layout stock_;
    Layout taken_;
  };

}
