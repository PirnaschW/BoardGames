
export module BG.Chess.Pieces;

namespace BoardGamesChessPieces
{

  using namespace BoardGamesCore;

  export class Pawn : public Kind
  {
  private:
    Pawn(void) noexcept : Kind('P') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Pawn ThePawn;
  };

  export class Knight : public Kind
  {
  private:
    Knight(void) noexcept : Kind('N') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 300; }

  public:
    static const Knight TheKnight;
  };

  export class Bishop : public Kind
  {
  private:
    Bishop(void) noexcept : Kind('B') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 300; }

  public:
    static const Bishop TheBishop;
  };

  export class Rook : public Kind
  {
  private:
    Rook(void) noexcept : Kind('R') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 500; }

  public:
    static const Rook TheRook;
  };

  export class Queen : public Kind
  {
  private:
    Queen(void) noexcept : Kind('Q') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Queen TheQueen;
  };

  export class King : public Kind
  {
  private:
    King(void) noexcept : Kind('K') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 500; }

  public:
    static const King TheKing;
  };

  export class ChessPiece : public Piece
  {
  private:
    ChessPiece(const Kind& k, const PieceColor& c, UINT ID) : Piece(k, c, ID) {}
    ChessPiece(const ChessPiece&) = delete;
    ChessPiece& operator=(const ChessPiece&) = delete;

  public:
    virtual ~ChessPiece(void) override {}

  public:
    static const ChessPiece WP;
    static const ChessPiece WB;
    static const ChessPiece WN;
    static const ChessPiece WR;
    static const ChessPiece WQ;
    static const ChessPiece WK;
    static const ChessPiece BP;
    static const ChessPiece BB;
    static const ChessPiece BN;
    static const ChessPiece BR;
    static const ChessPiece BQ;
    static const ChessPiece BK;
  };

}
