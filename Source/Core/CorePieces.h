
namespace BoardGamesCore
{

  class Checker : public Kind
  {
  private:
    constexpr inline Checker(void) noexcept : Kind('C') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;

  public:
    static const Checker TheChecker;
  };

  class Pawn : public Kind
  {
  private:
    Pawn(void) noexcept : Kind('P') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Pawn ThePawn;
  };

  class Knight : public Kind
  {
  private:
    Knight(void) noexcept : Kind('N') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 300; }

  public:
    static const Knight TheKnight;
  };

  class Bishop : public Kind
  {
  private:
    Bishop(void) noexcept : Kind('B') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 300; }

  public:
    static const Bishop TheBishop;
  };

  class Rook : public Kind
  {
  private:
    Rook(void) noexcept : Kind('R') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 500; }

  public:
    static const Rook TheRook;
  };

  class Queen : public Kind
  {
  private:
    Queen(void) noexcept : Kind('Q') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Queen TheQueen;
  };

  class King : public Kind
  {
  private:
    King(void) noexcept : Kind('K') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 500; }

  public:
    static const King TheKing;
  };

  class CorePiece : public Piece
  {
  private:
    CorePiece(const Kind& k, const PieceColor& c, unsigned int ID) : Piece(k, c, ID) {}
    CorePiece(const CorePiece&) = delete;
    CorePiece& operator=(const CorePiece&) = delete;

  public:
    virtual ~CorePiece(void) override {}

  public:
    static const CorePiece WC;  // White Checker
    static const CorePiece BC;  // Black Checker

    static const CorePiece WP;  // White Pawn
    static const CorePiece WB;  // White Bishop
    static const CorePiece WN;  // White Knight
    static const CorePiece WR;  // White Rook 
    static const CorePiece WQ;  // White Queen
    static const CorePiece WK;  // White King 

    static const CorePiece BP;  // Black Pawn 
    static const CorePiece BB;  // Black Bishop
    static const CorePiece BN;  // Black Knight
    static const CorePiece BR;  // Black Rook 
    static const CorePiece BQ;  // Black Queen
    static const CorePiece BK;  // Black King 
  };

}
