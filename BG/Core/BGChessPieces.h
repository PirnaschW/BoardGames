
namespace BoardGamesChessPieces
{

  using namespace BoardGamesCore;

  class Pawn : public Kind
  {
  private:
    Pawn(void) : Kind('P') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const override { return 100; }

  public:
    inline const static Pawn ThePawn{};
  };

  class Knight : public Kind
  {
  private:
    Knight(void) : Kind('N') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const override { return 300; }

  public:
    inline const static Knight TheKnight{};
  };

  class Bishop : public Kind
  {
  private:
    Bishop(void) : Kind('B') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const override { return 300; }

  public:
    inline const static Bishop TheBishop{};
  };

  class Rook : public Kind
  {
  private:
    Rook(void) : Kind('R') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const override { return 500; }

  public:
    inline const static Rook TheRook{};
  };

  class Queen : public Kind
  {
  private:
    Queen(void) : Kind('Q') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const override { return 900; }

  public:
    inline const static Queen TheQueen{};
  };

  class King : public Kind
  {
  private:
    King(void) : Kind('K') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const override { return 500; }

  public:
    inline const static King TheKing{};
  };

  class ChessPiece : public Piece
  {
  private:
    ChessPiece(const Kind* k, const Color* c, UINT l, UINT d, UINT s) : Piece(k, c, l, d, s) {}
    ChessPiece(const ChessPiece&) = delete;
    ChessPiece& operator=(const ChessPiece&) = delete;

  public:
    virtual ~ChessPiece(void) override {}

  public:
    const static ChessPiece WP;
    const static ChessPiece WB;
    const static ChessPiece WN;
    const static ChessPiece WR;
    const static ChessPiece WQ;
    const static ChessPiece WK;
    const static ChessPiece BP;
    const static ChessPiece BB;
    const static ChessPiece BN;
    const static ChessPiece BR;
    const static ChessPiece BQ;
    const static ChessPiece BK;
  };

}
