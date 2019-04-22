

namespace MassacreChess
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;  // pixels per tile
  constexpr unsigned int FieldSizeY = 50;  // pixels per tile

  constexpr unsigned int FieldSizeSX = 18;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 20;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;
  using namespace BoardGamesChessPieces;

  class MCPosition : public MainPosition
  {
  public:
    MCPosition(Coordinate x, Coordinate y);
    virtual inline MainPosition* Clone(void) const override { return new MCPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const override;
    virtual inline unsigned int GetMoveCountFactor(void) const noexcept override { return 1000; }

// extensions
  private:
    virtual bool PlaceRandomly(const Piece* p);
  };


  class MCLayout : public MainLayout
  {
  public:
    inline MCLayout(Coordinate x, Coordinate y) noexcept :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
  };


  class MCTakenLayout : public TakenLayout
  {
  public:
    inline MCTakenLayout(Coordinate x, Coordinate y)  noexcept :
      TakenLayout(Dimension(x*y / 2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  };


  class MCStockLayout : public StockLayout
  {
  public:
    inline MCStockLayout(Coordinate /*x*/, Coordinate y)  noexcept :
      StockLayout(Dimension(5, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (y + 1), FieldSizeX, FieldSizeY)) {}
  };


  class MCGame : public Game
  {
  private:
    MCGame(void) = delete;
    MCGame(MCPosition* p, TakenPosition* t, StockPosition* s, MCLayout* l, MCTakenLayout* tl, MCStockLayout* sl) noexcept;

  public:
    MCGame(Coordinate x, Coordinate y) noexcept;
    static const VariantList& GetVariants(void) noexcept;
  };

}
