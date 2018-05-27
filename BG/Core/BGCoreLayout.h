
namespace BoardGamesCore
{

  class Tile
  {
  public:
    Tile(const Location& l, const CRect& r, const TileColor* f) : location{l}, rect{r}, tilecolor{f} {}
    ~Tile(void) {}
    Tile& operator=(const Tile&) = delete;
    bool InRect(const CPoint &p) const { return rect.PtInRect(p) != 0; }
    const CRect& GetRect(void) const { return rect; }
    const Location GetLocation(void) const { return location; }
    void Draw(CDC* pDC, const Piece* p) const { p->Draw(pDC, rect, tilecolor); }

  private:
    const Location location;
    const TileColor* tilecolor;
    CRect rect;
  };


  struct Dimension
  {
  public:
    constexpr Dimension(
      unsigned int xc, unsigned int yc,                     // tile count in x and y directions
      unsigned int le, unsigned int te,                     // starting edge in x and y directions
      unsigned int xd, unsigned int yd,                     // tile size in x and y directions
      unsigned int xs = 0, unsigned int ys = 0) noexcept :  // extra distance between tiles in x and y directions
      xCount(xc), yCount(yc),
      lEdge(le), tEdge(te),
      xDim(xd), yDim(yd),
      xSkip(xs), ySkip(ys),
      rEdge(lEdge + xCount * xDim), bEdge(tEdge + yCount * yDim) {}

  public:
    const unsigned int xCount;
    const unsigned int yCount;
    const unsigned int xDim;
    const unsigned int yDim;
    const unsigned int xSkip;
    const unsigned int ySkip;
    const unsigned int lEdge;
    const unsigned int rEdge;
    const unsigned int tEdge;
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
    ~Layout() { for (auto& t : tiles) delete t; }
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
    constexpr MainLayout(const Dimension& d, LayoutType lt = LayoutType::Alternating) noexcept : Layout(d, lt) {}
    ~MainLayout() {}
    virtual void Draw(CDC* pDC, const Position* pos) const override;
  };

  class TakenLayout : public Layout
  {
  public:
    constexpr TakenLayout(const Dimension& d, LayoutType lt = LayoutType::Small) noexcept : Layout(d, lt) {}
    ~TakenLayout() {}
  };

  class StockLayout : public Layout
  {
  public:
    constexpr StockLayout(const Dimension& d, LayoutType lt = LayoutType::Light) noexcept : Layout(d, lt) {}
    ~StockLayout() {}
  };

}
