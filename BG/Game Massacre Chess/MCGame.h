

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
    MCPosition(unsigned int x, unsigned int y);
    virtual MainPosition* Clone(void) const override { return new MCPosition(*this); }
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;

  private:
    virtual bool PlaceRandomly(const Piece* p);
  };


  class MCLayout : public MainLayout
  {
  public:
    MCLayout(unsigned int x, unsigned int y) noexcept : MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
  };


  class MCTakenLayout : public TakenLayout
  {
  public:
    MCTakenLayout(unsigned int x, unsigned int y)  noexcept :
      TakenLayout(Dimension(x*y / 2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  };


  class MCStockLayout : public StockLayout
  {
  public:
    MCStockLayout(unsigned int /*x*/, unsigned int y)  noexcept :
      StockLayout(Dimension(5, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (y + 1), FieldSizeX, FieldSizeY)) {}
  };


  class MCGame : public Game
  {
  private:
    MCGame(MCPosition* p, TakenPosition* t, StockPosition* s, MCLayout* l, MCTakenLayout* tl, MCStockLayout* sl);

  public:
    MCGame(unsigned int x, unsigned int y);
    virtual unsigned int Plies(unsigned int z) const noexcept override { return Game::Plies(0) + static_cast<unsigned int>(11.0 / log(static_cast<double>(max(z, 5) - 3))) - 3; }
  };

}
