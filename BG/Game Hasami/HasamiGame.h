
namespace Hasami
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 27;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 27;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class Checker : public Kind
  {
  private:
    Checker(void) : Kind('0') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const override { return 1; }
  
  public:
    inline const static Checker TheChecker{};
  };

  class HasamiPiece : public Piece
  {
  private:
    HasamiPiece(const Kind* k, const Color* c, UINT l, UINT s) : Piece(k, c, l, l, s) {}
    HasamiPiece(const HasamiPiece&) = delete;
    HasamiPiece& operator=(const HasamiPiece&) = delete;

  public:
    virtual ~HasamiPiece(void) override {}

  public:  // the pieces
    static const HasamiPiece HasamiPieceB;
    static const HasamiPiece HasamiPieceW;
  };


  class HasamiPosition : public MainPosition
  {
  public:
    HasamiPosition(unsigned int x, unsigned int y);
    virtual ~HasamiPosition() override {}
    virtual MainPosition* Clone(void) const override { return new HasamiPosition(*this); }
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;
    virtual Move::PositionValue EvaluateStatically(void) override;

  protected:
    inline unsigned int GetValue(unsigned int z) const;
  };


  class HasamiLayout : public MainLayout
  {
  public:
    HasamiLayout(unsigned int x, unsigned int y) :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY), LayoutType::Light) {}
    virtual ~HasamiLayout() {}
  };

  class HasamiTakenLayout : public TakenLayout
  {
  public:
    HasamiTakenLayout(unsigned int x, unsigned int y) :
      TakenLayout(Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
    virtual ~HasamiTakenLayout() {}
  };

  class HasamiStockLayout : public StockLayout
  {
  public:
    HasamiStockLayout(unsigned int x, unsigned int y) :
      StockLayout(Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
    virtual ~HasamiStockLayout() {}
  };


  class HasamiGame : public Game
  {
  private:
    HasamiGame(HasamiPosition* p, TakenPosition* t, StockPosition* s, HasamiLayout* l, HasamiTakenLayout* tl, HasamiStockLayout* sl);
  public:
    HasamiGame(unsigned int x, unsigned int y) : HasamiGame(
      new HasamiPosition(x, y), new TakenPosition(2 * x, 2), new StockPosition(3, 1),
      new HasamiLayout(x, y), new HasamiTakenLayout(x, y), new HasamiStockLayout(x, y)) {}
    virtual ~HasamiGame(void) override {};
  };

}
