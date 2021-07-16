
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

  class Checker final : public Kind
  {
  private:
    constexpr inline Checker(void) noexcept : Kind('0') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const Checker TheChecker;
  };

  class King final : public Kind
  {
  private:
    constexpr inline King(void) noexcept : Kind('K') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 500; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const King TheKing;
  };

  class Queen final : public Kind
  {
  private:
    constexpr inline Queen(void) noexcept : Kind('K') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 800; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const Queen TheQueen;
  };

  class CheckersPiece final : public Piece
  {
  private:
    inline CheckersPiece(const Kind& k, const PieceColor& c, const CheckersPiece* u, unsigned int ID) noexcept : Piece(k, c, ID), up_(u) {}
    CheckersPiece(const CheckersPiece&) = delete;
    CheckersPiece& operator=(const CheckersPiece&) = delete;
  public:
    virtual inline bool IsPromotable(void) const noexcept override { return up_ != this; }          // is this a promotable piece?
    virtual inline const Piece& Promote(bool /*u*/) const noexcept override { return *up_; }         // promote this piece up/down
  private:
    const CheckersPiece* up_;                                                                       // what this piece promotes up to

  public:
    static const CheckersPiece CheckersPieceW;
    static const CheckersPiece CheckersPieceB;
    static const CheckersPiece CheckersKingW;
    static const CheckersPiece CheckersKingB;
    static const CheckersPiece CheckersQueenW;
    static const CheckersPiece CheckersQueenB;
  };


  class CheckersPosition final : public MainPosition
  {
  public:
    CheckersPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept;
    virtual inline MainPosition* Clone(void) const noexcept override { return new CheckersPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual void GetAllMoves(void) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
// extensions
  public:
    bool AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, const Piece& p, const Location& fr) const noexcept;
  private:
    inline bool CanPromote(const Location& l, const Piece& p) const noexcept;
  };


  class CheckersGame final : public Game
  {
  private:
    CheckersGame(void) = delete;

  public:
    inline CheckersGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, new CheckersPosition(v.c, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
