
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


  enum CheckerVariant : VariantCode   // recognized variants of Checkers
  {
    Standard      = '\0',
    International = '*',

    Brazilian     = 'B',
    Canadian      = 'C',
    Czech         = 'Z',
    Italian       = 'I',
    Portuguese    = 'P',
    Russian       = 'R',
    Thai          = 'H',
    Turkish       = 'T',

    Anti          = 'a',
    Corner        = 'c',
    Hawaiian      = 'h',
    OneWay        = 'o',
    Parachute     = 'p',
    Gothic        = 'g',
    Dameo         = 'd',
  };

  enum Rule : unsigned int // multi-use rule variants
  {
    None                  = 0x0000,                // none of these rules
    JumpFurther           = 0x0001,                // promoted pieces can jump further than behind opponent's piece
    BackJump              = 0x0002,                // checkers can jump backwards
    MaxCapture            = 0x0004,                // must capture the maximum possible number of pieces
    MaxPromotedCapture    = 0x0008,                // must capture the maximum possible number of promoted pieces
    PromotedJump          = 0x0010,                // must capture with promoted piece
    ContinueJumping       = 0x0020,                // checkers can continue jumping after being promoted
    NoPromotedCapture     = 0x0040,                // checkers cannot capture promoted pieces
    CapturePromotedFirst  = 0x0080,                // capture promoted pieces first
    PromoteToQueen        = 0x0100,                // pieces promote to Queens (not Kings)
  };
  inline constexpr Rule operator | (Rule a, Rule b) { return static_cast<Rule>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b)); }
  inline constexpr Rule operator & (Rule a, Rule b) { return static_cast<Rule>(static_cast<unsigned char>(a) & static_cast<unsigned char>(b)); }
  inline constexpr bool operator >= (Rule rule, Rule test) { return rule & test; }

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
    constexpr inline Queen(void) noexcept : Kind('Q') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 800; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const Queen TheQueen;
  };

  class Para final : public Kind
  {
  private:
    constexpr inline Para(void) noexcept : Kind('P') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const Para ThePara;
  };

  class CheckersPiece final : public Piece
  {
  private:
    inline CheckersPiece(const Kind& k, const PieceColor& c, const CheckersPiece* u, unsigned int ID) noexcept : Piece(k, c, ID), up_(u) {}
    CheckersPiece(const CheckersPiece&) = delete;
    CheckersPiece& operator=(const CheckersPiece&) = delete;
  public:
    virtual inline bool IsPromotable(void) const noexcept override { return up_ != this; }          // is this a promotable piece?
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
  };


  class CheckersPosition : public MainPosition
  {
  public:
    CheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(v, p, d) {}
    virtual inline MainPosition* Clone(void) const noexcept override { return new CheckersPosition(*this); }
    virtual void SetStartingPosition() noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual void GetAllMoves(void) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
// extensions
  public:
    bool AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, const Piece& p, const Location& fr) const noexcept;
  protected:
    virtual bool CanPromote(const Location& l, const Piece& p) const noexcept;
    virtual Rule GetRule() const noexcept { return PromotedJump; }
  };


  class CheckersGame final : public Game
  {
  private:
    CheckersGame(void) = delete;
    static MainPosition* GetNewPosition(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept;

  public:
    inline CheckersGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, GetNewPosition(v, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
