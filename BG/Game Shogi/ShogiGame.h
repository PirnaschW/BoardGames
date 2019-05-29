
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
    virtual bool CanDrop(const MainPosition* pos, const Location& l) const noexcept override;

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
    virtual bool CanDrop(const MainPosition* pos, const Location& l) const noexcept override;

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
    virtual inline bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept override { return true; }  // can drop anywhere

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
    virtual inline bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept override { return true; }  // can drop anywhere

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
    virtual bool CanDrop(const MainPosition* pos, const Location& l) const noexcept override;

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
    virtual inline bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept override { return true; }  // can drop anywhere

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
    inline bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept override { return true; }  // can drop anywhere

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
    inline bool CanDrop(const MainPosition* pos, const Location& l) const noexcept { return kind->CanDrop(pos,l); };

  private:
    const ShogiPiece* up;    // what this piece promotes up to
    const ShogiPiece* down;  // what this piece promotes down to

  public:  // the pieces
    // S = Standard pieces, White
    static const ShogiPiece ShogiSKW;
    static const ShogiPiece ShogiSGW;
    static const ShogiPiece ShogiSSW;
    static const ShogiPiece ShogiSBW;
    static const ShogiPiece ShogiSRW;
    static const ShogiPiece ShogiSNW;
    static const ShogiPiece ShogiSLW;
    static const ShogiPiece ShogiSPW;
    // P = Promoted pieces, White   
    static const ShogiPiece ShogiPSW;
    static const ShogiPiece ShogiPBW;
    static const ShogiPiece ShogiPRW;
    static const ShogiPiece ShogiPNW;
    static const ShogiPiece ShogiPLW;
    static const ShogiPiece ShogiPPW;
                                    
    // S = Standard pieces, Black
    static const ShogiPiece ShogiSKB;
    static const ShogiPiece ShogiSGB;
    static const ShogiPiece ShogiSSB;
    static const ShogiPiece ShogiSBB;
    static const ShogiPiece ShogiSRB;
    static const ShogiPiece ShogiSNB;
    static const ShogiPiece ShogiSLB;
    static const ShogiPiece ShogiSPB;
    // P = Promoted pieces, Black   
    static const ShogiPiece ShogiPSB;
    static const ShogiPiece ShogiPBB;
    static const ShogiPiece ShogiPRB;
    static const ShogiPiece ShogiPNB;
    static const ShogiPiece ShogiPLB;
    static const ShogiPiece ShogiPPB;
  };


  class ShogiPosition : public MainPosition
  {
  public:
    ShogiPosition(const PieceMapP& p, Coordinate x, Coordinate y) noexcept;
    virtual inline MainPosition* Clone(void) const override { return new ShogiPosition(*this); };
    virtual void GetAllMoves(void);              // generate all moves and save list
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const override;

  // extensions to base class
  public:
    inline void SetTPos(TakenPosition* t) noexcept { tpos = t; }
  private:
    inline bool CanPromote(const Location &l) const noexcept;

  private:
    TakenPosition* tpos{ nullptr };
  };


  class ShogiTakenPosition : public TakenPosition
  {
  public:
    inline ShogiTakenPosition(const PieceMapP& p, Coordinate x, Coordinate /*y*/) noexcept : TakenPosition(p, x == 9 ? 27 : 15, 2) {}
  };


  class ShogiLayout : public MainLayout
  {
  public:
    inline ShogiLayout(Coordinate x, Coordinate y) noexcept :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY), LayoutType::Light) {}
  };

  class ShogiTakenLayout : public TakenLayout
  {
  public:
    inline ShogiTakenLayout(Coordinate x, Coordinate /*y*/) noexcept :
      TakenLayout(Dimension(3 * x, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * x - FieldSizeSY * 4)) { }
  };

  class ShogiStockLayout : public StockLayout
  {
  public:
    inline ShogiStockLayout(Coordinate x, Coordinate y) noexcept:
      StockLayout(Dimension(15, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY)) {}
  };


  class ShogiGame : public Game
  {
  protected:
    ShogiGame(void) = delete;
    ShogiGame(const PieceMapP& m, ShogiPosition* p, TakenPosition* t, StockPosition* s, ShogiLayout* l, ShogiTakenLayout* tl, ShogiStockLayout* sl) noexcept;
  public:
    ShogiGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept;
    inline constexpr static bool IsFull(Coordinate x, Coordinate /*y*/) noexcept { return x == 9; } //only check for x == 9 -> full Shogi game, all others are Mini
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
  };

}
