
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


  struct Dimension final  /// PoD structure to collect a game's layout dimensions
  {
  public:
    constexpr Dimension(
      Coordinate xc, Coordinate yc,                         // tile count in x and y directions
      unsigned int le, unsigned int te,                     // starting edge in x and y directions
      unsigned int xd, unsigned int yd,                     // tile size in x and y directions
      unsigned int xs = 0, unsigned int ys = 0) noexcept :  // extra distance between tiles in x and y directions
      xCount(xc), yCount(yc),
      xDim(xd), yDim(yd),
      xSkip(xs), ySkip(ys),
      lEdge(le), tEdge(te),
      rEdge(lEdge + xCount * xDim), bEdge(tEdge + yCount * yDim) {}

  public:
    const Coordinate xCount;
    const Coordinate yCount;
    const unsigned int xDim;
    const unsigned int yDim;
    const unsigned int xSkip;
    const unsigned int ySkip;
    const unsigned int lEdge;
    const unsigned int tEdge;
    const unsigned int rEdge;
    const unsigned int bEdge;
  };
  

  class Layout  // a layout contains the physical layout of the tiles of the playing board
  {
  public:
    enum class LayoutType { Light, Dark, Alternating, Small };
  protected:
    Layout(void) = delete;
  public:
    Layout(const Dimension& d, LayoutType lt);
    virtual inline ~Layout() noexcept { for (auto& t : tiles) delete t; }
    virtual void Draw(CDC* pDC, const Position* pos) const;
    virtual void DrawSelected(CDC* pDC, const Location& l) const;
    virtual bool GetLocation(const CPoint& p, Location& l) const;

  protected:
    const Dimension dim;
    std::vector<Tile*> tiles;
  private:
    const LayoutType ltype;
  };

  class MainLayout : public Layout
  {
  public:
    constexpr inline MainLayout(const Dimension& d, LayoutType lt = LayoutType::Alternating) : Layout(d, lt) {}
    virtual inline ~MainLayout() noexcept {}
    virtual void Draw(CDC* pDC, const Position* pos) const override;
  };

  class TakenLayout : public Layout
  {
  public:
    constexpr inline TakenLayout(const Dimension& d, LayoutType lt = LayoutType::Small) : Layout(d, lt) {}
    inline ~TakenLayout() {}
  };

  class StockLayout : public Layout
  {
  public:
    constexpr inline StockLayout(const Dimension& d, LayoutType lt = LayoutType::Light) : Layout(d, lt) {}
    inline ~StockLayout() {}
  };

}
