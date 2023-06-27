namespace Checkers
{
  using namespace BoardGamesCore;

  class Checker final : public Kind
  {
  private:
    constexpr inline Checker() noexcept : Kind('0') {}
  public:
    virtual inline PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const Board& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const Checker TheChecker;
  };

  class King final : public Kind
  {
  private:
    constexpr inline King() noexcept : Kind('K') {}
  public:
    virtual inline PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 500; }
    virtual void CollectMoves(const Board& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const King TheKing;
  };

  class Queen final : public Kind
  {
  private:
    constexpr inline Queen() noexcept : Kind('Q') {}
  public:
    virtual inline PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 800; }
    virtual void CollectMoves(const Board& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const Queen TheQueen;
  };

  class Para final : public Kind
  {
  private:
    constexpr inline Para() noexcept : Kind('P') {}
  public:
    virtual inline PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const Board& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const Para ThePara;
  };

  class TurkChecker final : public Kind
  {
  private:
    constexpr inline TurkChecker() noexcept : Kind('T') {}
  public:
    virtual inline PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const Board& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const TurkChecker TheTurkChecker;
  };


  class CheckersPiece final : public Piece
  {
  private:
    constexpr CheckersPiece(const Kind& k, const PieceColor& c, const CheckersPiece* u, unsigned int ID) noexcept : Piece(k, c, ID), up_(u) {}
    CheckersPiece(const CheckersPiece&) = delete;
    CheckersPiece& operator=(const CheckersPiece&) = delete;
  public:
    virtual inline bool IsPromotable() const noexcept override { return up_ != this; }          // is this a promotable piece?
    virtual inline const Piece& Promote(bool /*u*/) const noexcept override { return *up_; }        // promote this piece up/down
  private:
    const CheckersPiece* up_;                                                                       // what this piece promotes up to

  public:
    static const CheckersPiece CheckersPieceW;
    static const CheckersPiece CheckersPieceB;
    static const CheckersPiece CheckersKingW;
    static const CheckersPiece CheckersKingB;
    static const CheckersPiece CheckersQueenW;
    static const CheckersPiece CheckersQueenB;
    static const CheckersPiece CheckersParaW;
    static const CheckersPiece CheckersParaB;
    static const CheckersPiece TurkCheckersPieceW;
    static const CheckersPiece TurkCheckersPieceB;
  };

  constexpr inline const CheckersPiece CheckersPiece::CheckersPieceW    { Checker    ::TheChecker,     PieceColor::White, &CheckersQueenW, IDB_WPL };
  constexpr inline const CheckersPiece CheckersPiece::CheckersPieceB    { Checker    ::TheChecker,     PieceColor::Black, &CheckersQueenB, IDB_BPL };
  constexpr inline const CheckersPiece CheckersPiece::CheckersKingW     { King       ::TheKing,        PieceColor::White, &CheckersKingW,  IDB_WKL };
  constexpr inline const CheckersPiece CheckersPiece::CheckersKingB     { King       ::TheKing,        PieceColor::Black, &CheckersKingB,  IDB_BKL };
  constexpr inline const CheckersPiece CheckersPiece::CheckersQueenW    { Queen      ::TheQueen,       PieceColor::White, &CheckersQueenW, IDB_WQL };
  constexpr inline const CheckersPiece CheckersPiece::CheckersQueenB    { Queen      ::TheQueen,       PieceColor::Black, &CheckersQueenB, IDB_BQL };
  constexpr inline const CheckersPiece CheckersPiece::CheckersParaW     { Para       ::ThePara,        PieceColor::White, &CheckersPieceW, IDB_CHECKERSWPARA };
  constexpr inline const CheckersPiece CheckersPiece::CheckersParaB     { Para       ::ThePara,        PieceColor::Black, &CheckersPieceB, IDB_CHECKERSBPARA };
  constexpr inline const CheckersPiece CheckersPiece::TurkCheckersPieceW{ TurkChecker::TheTurkChecker, PieceColor::White, &CheckersQueenW, IDB_WPL };
  constexpr inline const CheckersPiece CheckersPiece::TurkCheckersPieceB{ TurkChecker::TheTurkChecker, PieceColor::Black, &CheckersQueenB, IDB_BPL };

}
