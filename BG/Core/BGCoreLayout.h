
namespace BoardGamesCore
{

  class Tile final
  {
  public:
    constexpr inline Tile(const Location& l, const CRect& r, const TileColor* f) noexcept : location{ l }, tilecolor{ f }, rect{ r } {}
    Tile& operator=(const Tile&) = delete;
    inline bool InRect(const CPoint& p) const noexcept { return rect.PtInRect(p) != 0; }
    constexpr inline const CRect& GetRect(void) const noexcept { return rect; }
    constexpr inline const Location GetLocation(void) const noexcept { return location; }
    inline void Draw(CDC* pDC, const Piece* p) const { p->Draw(pDC, rect, tilecolor); }

  private:
    const Location location;
    const TileColor* tilecolor;
    CRect rect;
  };
  

  class Layout  // a layout contains the physical layout of the tiles of the playing board
  {
  public:
    enum class LayoutType { Light, Dark, Alternating, Small };
  protected:
    Layout(void) = delete;
  public:
    Layout(const Dimension& d, const BoardPart b, const LayoutType lt) noexcept;
    virtual inline ~Layout() noexcept { for (auto& t : tiles) delete t; }
    void Draw(CDC* pDC, const Position* pos, _Mode mode) const;
    virtual void DrawSelected(CDC* pDC, const Location& l) const;
    virtual bool GetLocation(const CPoint& p, Location& l) const noexcept;

  protected:
    const Dimension dim;
    std::vector<Tile*> tiles;
  private:
    const LayoutType ltype;
  };

  class MainLayout : public Layout
  {
  public:
    inline MainLayout(const Dimensions& d, LayoutType lt = LayoutType::Alternating) noexcept :
      Layout(d[0], BoardPart::Main, lt), _stock(d[1], BoardPart::Stock, LayoutType::Light), _taken(d[2], BoardPart::Taken, LayoutType::Small) {}
    virtual inline ~MainLayout() noexcept {}
    virtual void Draw(CDC* pDC, const MainPosition* pos, _Mode mode) const;

  protected:
    Layout _stock;
    Layout _taken;
  };

}
