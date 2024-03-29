
namespace Shogi
{
  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 27;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 27;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class Pawn : public Kind
  {
  private:
    constexpr Pawn() noexcept : Kind('p') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 1000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
    virtual bool CanDrop(const Piece& p, const Location& l, const Board* board_) const noexcept override;

  public:
    static const Pawn ThePawn;
  };

  class Knight : public Kind
  {
  private:
    constexpr Knight() noexcept : Kind('n') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 4000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
    virtual bool CanDrop(const Piece& p, const Location& l, const Board* board_) const noexcept override { return CanMove(p, l, board_); }
    virtual bool CanMove(const Piece& p, const Location& l, const Board* board_) const noexcept override;

  public:
    static const Knight TheKnight;
  };

  class Bishop : public Kind
  {
  private:
    constexpr Bishop() noexcept : Kind('b') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 8000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
    virtual bool CanDrop(const Piece&, const Location&, const Board*) const noexcept override { return true; }  // can drop anywhere

  public:
    static const Bishop TheBishop;
  };

  class Rook : public Kind
  {
  private:
    constexpr Rook() noexcept : Kind('r') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 10000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
    virtual bool CanDrop(const Piece&, const Location&, const Board*) const noexcept override { return true; }  // can drop anywhere

  public:
    static const Rook TheRook;
  };

  class Lance : public Kind
  {
  private:
    constexpr Lance() noexcept : Kind('l') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 3000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
    virtual bool CanDrop(const Piece& p, const Location& l, const Board* board_) const noexcept override { return CanMove(p, l, board_); }
    virtual bool CanMove(const Piece& p, const Location& l, const Board* board_) const noexcept override;

  public:
    static const Lance TheLance;
  };

  class Silver : public Kind
  {
  private:
    constexpr Silver() noexcept : Kind('s') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 5000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
    virtual bool CanDrop(const Piece&, const Location&, const Board*) const noexcept override { return true; }  // can drop anywhere

  public:
    static const Silver TheSilver;
  };

  class Gold : public Kind
  {
  private:
    constexpr Gold() noexcept : Kind('G') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;
    static void CollectGoldMoves(const Board&, const Location&, Moves&) noexcept; // enable reuse for other kinds
    bool CanDrop(const Piece&, const Location&, const Board*) const noexcept override { return true; }  // can drop anywhere

  public:
    static const Gold TheGold;
  };

  class King : public Kind
  {
  private:
    constexpr King() noexcept : Kind('K') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 8000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

  public:
    static const King TheKing;
  };

  class PPawn : public Kind
  {
  private:
    constexpr PPawn() noexcept : Kind('P') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 7000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

  public:
    static const PPawn ThePPawn;
  };

  class PKnight : public Kind
  {
  private:
    constexpr PKnight() noexcept : Kind('N') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

  public:
    static const PKnight ThePKnight;
  };

  class PBishop : public Kind
  {
  private:
    constexpr PBishop() noexcept : Kind('B') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 10000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

  public:
    static const PBishop ThePBishop;
  };

  class PRook : public Kind
  {
  private:
    constexpr PRook() noexcept : Kind('R') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 12000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

  public:
    static const PRook ThePRook;
  };

  class PLance : public Kind
  {
  private:
    constexpr PLance() noexcept : Kind('L') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

  public:
    static const PLance ThePLance;
  };

  class PSilver : public Kind
  {
  private:
    constexpr PSilver() noexcept : Kind('S') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

  public:
    static const PSilver ThePSilver;
  };


  class ShogiPiece : public Piece
  {
  private:
    ShogiPiece(const Kind& k, const PieceColor& c, const ShogiPiece& u, const ShogiPiece& d, unsigned int ID) noexcept
      : Piece(k, c, ID), up(u), down(d) {}
    ShogiPiece(const ShogiPiece&) = delete;
    ShogiPiece& operator=(const ShogiPiece&) = delete;

  public:
    virtual bool IsPromotable() const noexcept override { return up != *this; }            // is this a promotable piece?
    virtual const Piece& Promote(bool u) const noexcept override { return u ? up : down; }    // promote this piece up/down
    bool CanDrop(const Board* board_, const Location& l) const noexcept { return kind_.CanDrop(*this, l, board_); };
    bool CanMove(const Board* board_, const Location& l) const noexcept { return kind_.CanMove(*this, l, board_); };

  private:
    const ShogiPiece& up;    // what this piece promotes up to
    const ShogiPiece& down;  // what this piece becomes when taken

  public:  // the pieces
    // S = Standard pieces, White
    static const ShogiPiece ShogiWK;   // White King 
    static const ShogiPiece ShogiWG;   // White Gold 
    static const ShogiPiece ShogiWS;   // White Silver 
    static const ShogiPiece ShogiWB;   // White Bishop 
    static const ShogiPiece ShogiWR;   // White Rook 
    static const ShogiPiece ShogiWN;   // White Knight 
    static const ShogiPiece ShogiWL;   // White Lance 
    static const ShogiPiece ShogiWP;   // White Pawn 
    // P = Promoted pieces, White      
    static const ShogiPiece ShogiWSP;  // White Silver (Promoted)
    static const ShogiPiece ShogiWBP;  // White Bishop (Promoted)
    static const ShogiPiece ShogiWRP;  // White Rook   (Promoted)
    static const ShogiPiece ShogiWNP;  // White Knight (Promoted)
    static const ShogiPiece ShogiWLP;  // White Lance  (Promoted)
    static const ShogiPiece ShogiWPP;  // White Pawn   (Promoted)
                                       
    // S = Standard pieces, Black 
    static const ShogiPiece ShogiBK;   // Black King  
    static const ShogiPiece ShogiBG;   // Black Gold  
    static const ShogiPiece ShogiBS;   // Black Silver 
    static const ShogiPiece ShogiBB;   // Black Bishop 
    static const ShogiPiece ShogiBR;   // Black Rook  
    static const ShogiPiece ShogiBN;   // Black Knight 
    static const ShogiPiece ShogiBL;   // Black Lance  
    static const ShogiPiece ShogiBP;   // Black Pawn  
    // P = Promoted pieces, Black
    static const ShogiPiece ShogiBSP;  // Black Silver (Promoted)
    static const ShogiPiece ShogiBBP;  // Black Bishop (Promoted)
    static const ShogiPiece ShogiBRP;  // Black Rook   (Promoted)
    static const ShogiPiece ShogiBNP;  // Black Knight (Promoted)
    static const ShogiPiece ShogiBLP;  // Black Lance  (Promoted)
    static const ShogiPiece ShogiBPP;  // Black Pawn   (Promoted)
  };


  class ShogiBoard : public Board
  {
  public:
    ShogiBoard(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : Board(v, p, d) {}
    virtual Board* Clone() const noexcept override { return new ShogiBoard(*this); };
    virtual void SetStartingBoard() noexcept override;
    virtual void GetAllMoves() const noexcept;                              // generate all moves and save list
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically() const noexcept override;              // calculate position value and save

  // extensions to base class
  public:
  private:
    bool CanPromote(const PieceColor& c, const Location &l) const noexcept;

  private:
  };


  class ShogiGame : public Game
  {
  protected:
    ShogiGame() = delete;

  public:
    ShogiGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, new ShogiBoard(v, m, d), new MainLayout(d, Layout::LayoutType::Light)) {}
    constexpr static bool IsFull(Coordinate x, Coordinate /*y*/) noexcept { return x == 9; } //only check for x == 9 -> full Shogi game, all others are Mini
    static const VariantList& GetVariants() noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
