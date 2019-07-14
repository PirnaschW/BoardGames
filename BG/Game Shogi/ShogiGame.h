
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
    constexpr inline Pawn(void) noexcept : Kind('p') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 1000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;
    virtual bool CanDrop(const Piece* p, const Location& l, const MainPosition* pos) const noexcept override;

  public:
    static const Pawn ThePawn;
  };

  class Knight : public Kind
  {
  private:
    constexpr inline Knight(void) noexcept : Kind('n') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 4000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;
    virtual inline bool CanDrop(const Piece* p, const Location& l, const MainPosition* pos) const noexcept override { return CanMove(p, l, pos); }
    virtual bool CanMove(const Piece* p, const Location& l, const MainPosition* pos) const noexcept override;

  public:
    static const Knight TheKnight;
  };

  class Bishop : public Kind
  {
  private:
    constexpr inline Bishop(void) noexcept : Kind('b') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 8000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;
    virtual inline bool CanDrop(const Piece*, const Location&, const MainPosition*) const noexcept override { return true; }  // can drop anywhere

  public:
    static const Bishop TheBishop;
  };

  class Rook : public Kind
  {
  private:
    constexpr inline Rook(void) noexcept : Kind('r') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 10000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;
    virtual inline bool CanDrop(const Piece*, const Location&, const MainPosition*) const noexcept override { return true; }  // can drop anywhere

  public:
    static const Rook TheRook;
  };

  class Lance : public Kind
  {
  private:
    constexpr inline Lance(void) noexcept : Kind('l') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 3000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;
    virtual inline bool CanDrop(const Piece* p, const Location& l, const MainPosition* pos) const noexcept override { return CanMove(p, l, pos); }
    virtual bool CanMove(const Piece* p, const Location& l, const MainPosition* pos) const noexcept override;

  public:
    static const Lance TheLance;
  };

  class Silver : public Kind
  {
  private:
    constexpr inline Silver(void) noexcept : Kind('s') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 5000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;
    virtual inline bool CanDrop(const Piece*, const Location&, const MainPosition*) const noexcept override { return true; }  // can drop anywhere

  public:
    static const Silver TheSilver;
  };

  class Gold : public Kind
  {
  private:
    constexpr inline Gold(void) noexcept : Kind('G') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;
    static void CollectGoldMoves(const MainPosition&, const Location&, Moves&); // enable reuse for other kinds
    inline bool CanDrop(const Piece*, const Location&, const MainPosition*) const noexcept override { return true; }  // can drop anywhere

  public:
    static const Gold TheGold;
  };

  class King : public Kind
  {
  private:
    constexpr inline King(void) noexcept : Kind('K') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 8000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const King TheKing;
  };

  class PPawn : public Kind
  {
  private:
    constexpr inline PPawn(void) noexcept : Kind('P') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 7000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const PPawn ThePPawn;
  };

  class PKnight : public Kind
  {
  private:
    constexpr inline PKnight(void) noexcept : Kind('N') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const PKnight ThePKnight;
  };

  class PBishop : public Kind
  {
  private:
    constexpr inline PBishop(void) noexcept : Kind('B') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 10000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const PBishop ThePBishop;
  };

  class PRook : public Kind
  {
  private:
    constexpr inline PRook(void) noexcept : Kind('R') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 12000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const PRook ThePRook;
  };

  class PLance : public Kind
  {
  private:
    constexpr inline PLance(void) noexcept : Kind('L') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const PLance ThePLance;
  };

  class PSilver : public Kind
  {
  private:
    constexpr inline PSilver(void) noexcept : Kind('S') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const PSilver ThePSilver;
  };


  class ShogiPiece : public Piece
  {
  private:
    inline ShogiPiece(const Kind* k, const Color* c, const ShogiPiece* u, const ShogiPiece* d, UINT l, UINT s) noexcept
      : Piece(k, c, l, l, s), up(u), down(d) {}
    ShogiPiece(const ShogiPiece&) = delete;
    ShogiPiece& operator=(const ShogiPiece&) = delete;
  public:
    virtual inline bool IsPromotable(void) const noexcept override { return up != this; }            // is this a promotable piece?
    virtual inline const Piece* Promote(bool u) const noexcept override { return u ? up : down; }    // promote this piece up/down
    inline bool CanDrop(const MainPosition* pos, const Location& l) const noexcept { return kind->CanDrop(this, l, pos); };
    inline bool CanMove(const MainPosition* pos, const Location& l) const noexcept { return kind->CanMove(this, l, pos); };

  private:
    const ShogiPiece* up;    // what this piece promotes up to
    const ShogiPiece* down;  // what this piece becomes when taken

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


  class ShogiPosition : public MainPosition
  {
  public:
    ShogiPosition(const PieceMapP& p, const Dimensions& d) noexcept;
    virtual inline MainPosition* Clone(void) const override { return new ShogiPosition(*this); };
    virtual void GetAllMoves(void);                                       // generate all moves and save list
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const override;
    virtual void EvaluateStatically(void) noexcept override;              // calculate position value and save

  // extensions to base class
  public:
  private:
    inline bool CanPromote(const Color* c, const Location &l) const noexcept;

  private:
  };


  class ShogiGame : public Game
  {
  protected:
    ShogiGame(void) = delete;

  public:
    inline ShogiGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new ShogiPosition(m, d), new MainLayout(d, Layout::LayoutType::Light)) {}
    inline constexpr static bool IsFull(Coordinate x, Coordinate /*y*/) noexcept { return x == 9; } //only check for x == 9 -> full Shogi game, all others are Mini
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
