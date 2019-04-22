
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
    constexpr inline Checker(void) noexcept : Kind('0') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 1000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const Checker TheChecker;
  };

  class HasamiPiece : public Piece
  {
  private:
    inline HasamiPiece(const Kind* k, const Color* c, UINT l, UINT s) noexcept : Piece(k, c, l, l, s) {}
    HasamiPiece(const HasamiPiece&) = delete;
    HasamiPiece& operator=(const HasamiPiece&) = delete;

  public:  // the pieces
    static const HasamiPiece HasamiPieceB;
    static const HasamiPiece HasamiPieceW;
  };


  class HasamiPosition : public MainPosition
  {
  public:
    HasamiPosition(Coordinate x, Coordinate y) noexcept;
    virtual inline MainPosition* Clone(void) const override { return new HasamiPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const override;
    virtual void EvaluateStatically(void) override;

  protected:
    inline unsigned int GetChainValue(unsigned int z) const noexcept;
  };

  class HasamiTakenPosition : public TakenPosition
  {
  public:
    inline HasamiTakenPosition(Coordinate x, Coordinate /*y*/) noexcept : TakenPosition(2 * x, 2) {}
  };


  class HasamiLayout : public MainLayout
  {
  public:
    HasamiLayout(Coordinate x, Coordinate y) noexcept :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY), LayoutType::Light) {}
  };

  class HasamiTakenLayout : public TakenLayout
  {
  public:
    HasamiTakenLayout(Coordinate x, Coordinate y) noexcept :
      TakenLayout(Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  };

  class HasamiStockLayout : public StockLayout
  {
  public:
    HasamiStockLayout(Coordinate x, Coordinate y) noexcept :
      StockLayout(Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
  };


  class HasamiGame : public Game
  {
  private:
    HasamiGame(void) = delete;
    HasamiGame(HasamiPosition* p, TakenPosition* t, StockPosition* s, HasamiLayout* l, HasamiTakenLayout* tl, HasamiStockLayout* sl) noexcept;
  public:
    HasamiGame(Coordinate x, Coordinate y) noexcept;
    static const VariantList& GetVariants(void) noexcept;
  };

}
