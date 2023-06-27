

namespace Chess
{
  using namespace BoardGamesCore;

  class Pawn : public Kind
  {
  private:
    Pawn() noexcept : Kind('P') {}
  public:
    void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Pawn ThePawn;
  };

  class Knight : public Kind
  {
  private:
    Knight() noexcept : Kind('N') {}
  public:
    void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 300; }

  public:
    static const Knight TheKnight;
  };

  class Bishop : public Kind
  {
  private:
    Bishop() noexcept : Kind('B') {}
  public:
    void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 300; }

  public:
    static const Bishop TheBishop;
  };

  class Rook : public Kind
  {
  private:
    Rook() noexcept : Kind('R') {}
  public:
    void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 500; }

  public:
    static const Rook TheRook;
  };

  class Cardinal : public Kind  // Bishop + Knight (also: Archbishop, Princess, Janus)
  {
  private:
    Cardinal() noexcept : Kind('C') {}
  public:
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Cardinal TheCardinal;
  };

  class Marshall : public Kind // Rook + Knight
  {
  private:
    Marshall() noexcept : Kind('M') {}
  public:
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Marshall TheMarshall;
  };

  class Queen : public Kind
  {
  private:
    Queen() noexcept : Kind('Q') {}
  public:
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Queen TheQueen;
  };

  class Amazon : public Kind // Queen + Knight
  {
  private:
    Amazon() noexcept : Kind('A') {}
  public:
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 900; }

  public:
    static const Amazon TheAmazon;
  };

  class King : public Kind
  {
  private:
    King() noexcept : Kind('K') {}
  public:
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 500; }

  public:
    static const King TheKing;
  };

  class Behem : public Kind // Behemoth - has to be named different than its variant code
  {
  private:
    Behem() noexcept : Kind('b') {}
  public:
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override {};
//    void GetMotions(Motions& m, const PieceColor& c) const noexcept override;
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 0; }
  public:
    static const Behem TheBehemoth;
  };

  class Ice : public Kind
  {
  private:
    Ice() noexcept : Kind('i') {}
  public:
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override {};
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 0; }

  public:
    static const Ice TheIce;
  };

  class ChessPiece : public Piece
  {
  private:
    inline ChessPiece(const Kind& k, const PieceColor& c, unsigned int ID) : Piece(k, c, ID) {}
    ChessPiece(const ChessPiece&) = delete;
    ChessPiece& operator=(const ChessPiece&) = delete;

  public:
    static const ChessPiece WP;  // White Pawn
    static const ChessPiece WN;  // White Knight
    static const ChessPiece WB;  // White Bishop
    static const ChessPiece WR;  // White Rook 
    static const ChessPiece WC;  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    static const ChessPiece WM;  // White Marshall = Rook + Knight
    static const ChessPiece WQ;  // White Queen
    static const ChessPiece WA;  // White Amazon   = Queen + Knight
    static const ChessPiece WK;  // White King 

    static const ChessPiece BP;  // Black Pawn 
    static const ChessPiece BN;  // Black Knight
    static const ChessPiece BB;  // Black Bishop
    static const ChessPiece BR;  // Black Rook 
    static const ChessPiece BC;  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    static const ChessPiece BM;  // Black Marshall = Rook + Knight
    static const ChessPiece BQ;  // Black Queen
    static const ChessPiece BA;  // Black Amazon   = Queen + Knight
    static const ChessPiece BK;  // Black King 

    static const ChessPiece RB;  // (Red) Behemoth 
    static const ChessPiece GI;  // (Gray) Ice
  };

}
