
namespace Checkers
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class Checker : public Kind
  {
  private:
    constexpr inline Checker(void) noexcept : Kind('0') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, std::vector<Move>& /*m*/) const override;

  public:
    inline const static Checker TheChecker{};
  };

  class King : public Kind
  {
  private:
    constexpr inline King(void) noexcept : Kind('K') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, std::vector<Move>& /*m*/) const override;

  public:
    inline const static King TheKing{};
  };

  class Queen : public Kind
  {
  private:
    constexpr inline Queen(void) noexcept : Kind('K') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, std::vector<Move>& /*m*/) const override;

  public:
    inline const static Queen TheQueen{};
  };

  class CheckersPiece : public Piece
  {
  private:
    inline CheckersPiece(const Kind* k, const Color* c, UINT l, UINT s) noexcept : Piece(k, c, l, l, s) {}
    CheckersPiece(const CheckersPiece&) = delete;
    CheckersPiece& operator=(const CheckersPiece&) = delete;
  public:
    ~CheckersPiece(void) override {}

  public:
    inline static const CheckersPiece CheckersPieceB{ &Checker::TheChecker, &Color::Black, IDB_LOAPEGB, IDB_LOAPEGBF };
    inline static const CheckersPiece CheckersPieceW{ &Checker::TheChecker, &Color::White, IDB_LOAPEGW, IDB_LOAPEGWF };
    inline static const CheckersPiece CheckersKingB{  &King::TheKing, &Color::Black, IDB_LOAPEGB, IDB_LOAPEGBF };
    inline static const CheckersPiece CheckersKingW{  &King::TheKing, &Color::White, IDB_LOAPEGW, IDB_LOAPEGWF };
    inline static const CheckersPiece CheckersQueenB{ &Queen::TheQueen, &Color::Black, IDB_LOAPEGB, IDB_LOAPEGBF };
    inline static const CheckersPiece CheckersQueenW{ &Queen::TheQueen, &Color::White, IDB_LOAPEGW, IDB_LOAPEGWF };
  };


  class CheckersPosition : public MainPosition
  {
  public:
    CheckersPosition(Coordinate x, Coordinate y) noexcept;
    virtual inline MainPosition* Clone(void) const override { return new CheckersPosition(*this); }
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;
    virtual void EvaluateStatically(void) override;
// extensions
  public:
    bool AddIfLegalJump(std::vector<Move>& m, const Location fr, const Offset d) const;
  };


  class CheckersLayout : public MainLayout
  {
  public:
    inline CheckersLayout(Coordinate x, Coordinate y) noexcept : 
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
  };

  class CheckersTakenLayout : public TakenLayout
  {
  public:
    inline CheckersTakenLayout(Coordinate x, Coordinate y) noexcept :
      TakenLayout(Dimension(x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  };

  class CheckersStockLayout : public StockLayout
  {
  public:
    inline CheckersStockLayout(Coordinate x, Coordinate y) noexcept :
      StockLayout(Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
  };


  class CheckersGame : public Game
  {
  private:
    CheckersGame(void) = delete;
    CheckersGame(CheckersPosition* p, TakenPosition* t, StockPosition* s, CheckersLayout* l, CheckersTakenLayout* tl, CheckersStockLayout* sl) noexcept;

  public:
    CheckersGame(Coordinate x, Coordinate y) noexcept;
    static const VariantList& GetVariants(void) noexcept;
  };

}
