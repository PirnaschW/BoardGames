
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
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const override;

  public:
    static const Checker TheChecker;
  };

  class King : public Kind
  {
  private:
    constexpr inline King(void) noexcept : Kind('K') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const override;

  public:
    static const King TheKing;
  };

  class Queen : public Kind
  {
  private:
    constexpr inline Queen(void) noexcept : Kind('K') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const override;

  public:
    static const Queen TheQueen;
  };

  class CheckersPiece : public Piece
  {
  private:
    inline CheckersPiece(const Kind* k, const Color* c, const CheckersPiece* u, UINT l, UINT s) noexcept : Piece(k, c, l, l, s), up(u) {}
    CheckersPiece(const CheckersPiece&) = delete;
    CheckersPiece& operator=(const CheckersPiece&) = delete;
  public:
    virtual inline bool IsPromotable(void) const noexcept override { return up != this; } // is this a promotable piece?
    virtual inline const Piece* Promote(bool /*u*/) const noexcept override { return up; }    // promote this piece up/down
  private:
    const CheckersPiece* up;    // what this piece promotes up to

  public:
    static const CheckersPiece CheckersPieceW;
    static const CheckersPiece CheckersPieceB;
    static const CheckersPiece CheckersKingW;
    static const CheckersPiece CheckersKingB;
    static const CheckersPiece CheckersQueenW;
    static const CheckersPiece CheckersQueenB;
  };


  class CheckersPosition : public MainPosition
  {
  public:
    CheckersPosition(const PieceMapP& p, Coordinate x, Coordinate y) noexcept;
    virtual inline MainPosition* Clone(void) const override { return new CheckersPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const override;
    virtual void GetAllMoves(void) override;
    virtual void EvaluateStatically(void) override;
// extensions
  public:
    bool AddIfLegalJump(Moves& m, bool longjumps, const Steps& s, const Location fr) const;
  private:
    inline bool CanPromote(const Location &l) const noexcept;
  };


  class CheckersLayout : public MainLayout
  {
  public:
    inline CheckersLayout(Coordinate x, Coordinate y) noexcept : 
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
  };

  //class CheckersTakenLayout : public TakenLayout
  //{
  //public:
  //  inline CheckersTakenLayout(Coordinate x, Coordinate y) noexcept :
  //    TakenLayout(Dimension(x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  //};

  //class CheckersStockLayout : public StockLayout
  //{
  //public:
  //  inline CheckersStockLayout(Coordinate x, Coordinate y) noexcept :
  //    StockLayout(Dimension(4, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
  //};


  class CheckersGame : public Game
  {
  private:
    CheckersGame(void) = delete;
    CheckersGame(const PieceMapP& m, CheckersPosition* p, CheckersLayout* l) noexcept;

  public:
    CheckersGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept;
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
  };

}
