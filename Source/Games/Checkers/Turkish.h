namespace Checkers
{
  using namespace BoardGamesCore;

  class TurkChecker final : public Kind
  {
  private:
    constexpr inline TurkChecker(void) noexcept : Kind('T') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const TurkChecker TheTurkChecker;
  };

  class TurkCheckersPiece final : public Piece
  {
  private:
    inline TurkCheckersPiece(const Kind& k, const PieceColor& c, const Piece* u, unsigned int ID) noexcept : Piece(k, c, ID), up_(u) {}
    TurkCheckersPiece(const TurkCheckersPiece&) = delete;
    TurkCheckersPiece& operator=(const TurkCheckersPiece&) = delete;
  public:
    virtual inline bool IsPromotable(void) const noexcept override { return up_ != this; }          // is this a promotable piece?
    virtual inline const Piece& Promote(bool /*u*/) const noexcept override { return *up_; }        // promote this piece up/down
  private:
    const Piece* up_;                                                                       // what this piece promotes up to

  public:
    static const TurkCheckersPiece TurkCheckersPieceW;
    static const TurkCheckersPiece TurkCheckersPieceB;
  };

  class TurkCheckersPosition : public CheckersPosition
  {
  public:
    TurkCheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : CheckersPosition(v, p, d) {}
    virtual inline MainPosition* Clone(void) const noexcept override { return new TurkCheckersPosition(*this); }
    void SetStartingPosition() noexcept override;
    // extensions
  public:
    virtual bool AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, const Piece& p, const Location& fr) const noexcept override;
  protected:
    Rule GetRule() const noexcept override { return PromoteToQueen | JumpFurther; }
  };

  class TurkCheckersGame : public CheckersGame
  {
  private:
    TurkCheckersGame(void) = delete;
//    static MainPosition* GetNewPosition(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept;

  public:
    inline TurkCheckersGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : CheckersGame(v, m, d) {}
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
  };


}
