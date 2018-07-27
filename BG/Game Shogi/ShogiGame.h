
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
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual bool CanDrop(const MainPosition* pos, const Location& l) const noexcept override;

  public:
    inline const static Pawn ThePawn{};

  };

  class Knight : public Kind
  {
  private:
    constexpr inline Knight(void) noexcept : Kind('n') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 4000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual bool CanDrop(const MainPosition* pos, const Location& l) const noexcept override;

  public:
    inline const static Knight TheKnight{};

  };

  class Bishop : public Kind
  {
  private:
    constexpr inline Bishop(void) noexcept : Kind('b') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 8000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual inline bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept override { return true; }  // can drop anywhere

  public:
    inline const static Bishop TheBishop{};
  };

  class Rook : public Kind
  {
  private:
    constexpr inline Rook(void) noexcept : Kind('r') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 10000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual inline bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept override { return true; }  // can drop anywhere

  public:
    inline const static Rook TheRook{};
  };

  class Lance : public Kind
  {
  private:
    constexpr inline Lance(void) noexcept : Kind('l') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 3000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual bool CanDrop(const MainPosition* pos, const Location& l) const noexcept override;

  public:
    inline const static Lance TheLance{};
  };

  class Silver : public Kind
  {
  private:
    constexpr inline Silver(void) noexcept : Kind('s') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 5000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual inline bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept override { return true; }  // can drop anywhere

  public:
    inline const static Silver TheSilver{};
  };

  class Gold : public Kind
  {
  private:
    constexpr inline Gold(void) noexcept : Kind('G') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    static void CollectGoldMoves(const MainPosition&, const Location&, std::vector<Move>&); // enable reuse for other kinds
    inline bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept override { return true; }  // can drop anywhere

  public:
    inline const static Gold TheGold{};
  };

  class King : public Kind
  {
  private:
    constexpr inline King(void) noexcept : Kind('K') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 8000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;

  public:
    inline const static King TheKing{};
  };

  class PPawn : public Kind
  {
  private:
    constexpr inline PPawn(void) noexcept : Kind('P') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 7000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;

  public:
    inline const static PPawn ThePPawn{};
  };

  class PKnight : public Kind
  {
  private:
    constexpr inline PKnight(void) noexcept : Kind('N') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;

  public:
    inline const static PKnight ThePKnight{};
  };

  class PBishop : public Kind
  {
  private:
    constexpr inline PBishop(void) noexcept : Kind('B') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 10000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;

  public:
    inline const static PBishop ThePBishop{};
  };

  class PRook : public Kind
  {
  private:
    constexpr inline PRook(void) noexcept : Kind('R') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 12000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;

  public:
    inline const static PRook ThePRook{};
  };

  class PLance : public Kind
  {
  private:
    constexpr inline PLance(void) noexcept : Kind('L') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;

  public:
    inline const static PLance ThePLance{};
  };

  class PSilver : public Kind
  {
  private:
    constexpr inline PSilver(void) noexcept : Kind('S') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 6000; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;

  public:
    inline const static PSilver ThePSilver{};
  };


  class ShogiPiece : public Piece
  {
  private:
    constexpr inline ShogiPiece(const Kind* k, const Color* c, const ShogiPiece* u, const ShogiPiece* d, UINT l, UINT s) noexcept
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
    inline static const ShogiPiece ShogiSKW{ &King::TheKing,       &Color::White, &ShogiPiece::ShogiSKW, &ShogiPiece::ShogiSKW, IDB_SHOGI_SK_W, IDB_SHOGI_SK_S };
    inline static const ShogiPiece ShogiSGW{ &Gold::TheGold,       &Color::White, &ShogiPiece::ShogiSGW, &ShogiPiece::ShogiSGW, IDB_SHOGI_SG_W, IDB_SHOGI_SG_S };
    inline static const ShogiPiece ShogiSSW{ &Silver::TheSilver,   &Color::White, &ShogiPiece::ShogiPSW, &ShogiPiece::ShogiSSW, IDB_SHOGI_SS_W, IDB_SHOGI_SS_S };
    inline static const ShogiPiece ShogiSBW{ &Bishop::TheBishop,   &Color::White, &ShogiPiece::ShogiPBW, &ShogiPiece::ShogiSBW, IDB_SHOGI_SB_W, IDB_SHOGI_SB_S };
    inline static const ShogiPiece ShogiSRW{ &Rook::TheRook,       &Color::White, &ShogiPiece::ShogiPRW, &ShogiPiece::ShogiSRW, IDB_SHOGI_SR_W, IDB_SHOGI_SR_S };
    inline static const ShogiPiece ShogiSNW{ &Knight::TheKnight,   &Color::White, &ShogiPiece::ShogiPNW, &ShogiPiece::ShogiSNW, IDB_SHOGI_SN_W, IDB_SHOGI_SN_S };
    inline static const ShogiPiece ShogiSLW{ &Lance::TheLance,     &Color::White, &ShogiPiece::ShogiPLW, &ShogiPiece::ShogiSLW, IDB_SHOGI_SL_W, IDB_SHOGI_SL_S };
    inline static const ShogiPiece ShogiSPW{ &Pawn::ThePawn,       &Color::White, &ShogiPiece::ShogiPPW, &ShogiPiece::ShogiSPW, IDB_SHOGI_SP_W, IDB_SHOGI_SP_S };
    // P = Promoted pieces, White
    inline static const ShogiPiece ShogiPSW{ &PSilver::ThePSilver, &Color::White, &ShogiPiece::ShogiPSW, &ShogiPiece::ShogiSSW, IDB_SHOGI_PS_W, IDB_SHOGI_PS_S };
    inline static const ShogiPiece ShogiPBW{ &PBishop::ThePBishop, &Color::White, &ShogiPiece::ShogiPBW, &ShogiPiece::ShogiSBW, IDB_SHOGI_PB_W, IDB_SHOGI_PB_S };
    inline static const ShogiPiece ShogiPRW{ &PRook::ThePRook,     &Color::White, &ShogiPiece::ShogiPRW, &ShogiPiece::ShogiSRW, IDB_SHOGI_PR_W, IDB_SHOGI_PR_S };
    inline static const ShogiPiece ShogiPNW{ &PKnight::ThePKnight, &Color::White, &ShogiPiece::ShogiPNW, &ShogiPiece::ShogiSNW, IDB_SHOGI_PN_W, IDB_SHOGI_PN_S };
    inline static const ShogiPiece ShogiPLW{ &PLance::ThePLance,   &Color::White, &ShogiPiece::ShogiPLW, &ShogiPiece::ShogiSLW, IDB_SHOGI_PL_W, IDB_SHOGI_PL_S };
    inline static const ShogiPiece ShogiPPW{ &PPawn::ThePPawn,     &Color::White, &ShogiPiece::ShogiPPW, &ShogiPiece::ShogiSPW, IDB_SHOGI_PP_W, IDB_SHOGI_PP_S };
    
    // S = Standard pieces, Black
    inline static const ShogiPiece ShogiSKB{ &King::TheKing,       &Color::Black, &ShogiPiece::ShogiSKB, &ShogiPiece::ShogiSKB, IDB_SHOGI_SK_B, IDB_SHOGI_SK_S };
    inline static const ShogiPiece ShogiSGB{ &Gold::TheGold,       &Color::Black, &ShogiPiece::ShogiSGB, &ShogiPiece::ShogiSGB, IDB_SHOGI_SG_B, IDB_SHOGI_SG_S };
    inline static const ShogiPiece ShogiSSB{ &Silver::TheSilver,   &Color::Black, &ShogiPiece::ShogiPSB, &ShogiPiece::ShogiSSB, IDB_SHOGI_SS_B, IDB_SHOGI_SS_S };
    inline static const ShogiPiece ShogiSBB{ &Bishop::TheBishop,   &Color::Black, &ShogiPiece::ShogiPBB, &ShogiPiece::ShogiSBB, IDB_SHOGI_SB_B, IDB_SHOGI_SB_S };
    inline static const ShogiPiece ShogiSRB{ &Rook::TheRook,       &Color::Black, &ShogiPiece::ShogiPRB, &ShogiPiece::ShogiSRB, IDB_SHOGI_SR_B, IDB_SHOGI_SR_S };
    inline static const ShogiPiece ShogiSNB{ &Knight::TheKnight,   &Color::Black, &ShogiPiece::ShogiPNB, &ShogiPiece::ShogiSNB, IDB_SHOGI_SN_B, IDB_SHOGI_SN_S };
    inline static const ShogiPiece ShogiSLB{ &Lance::TheLance,     &Color::Black, &ShogiPiece::ShogiPLB, &ShogiPiece::ShogiSLB, IDB_SHOGI_SL_B, IDB_SHOGI_SL_S };
    inline static const ShogiPiece ShogiSPB{ &Pawn::ThePawn,       &Color::Black, &ShogiPiece::ShogiPPB, &ShogiPiece::ShogiSPB, IDB_SHOGI_SP_B, IDB_SHOGI_SP_S };
    // P = Promoted pieces, Black
    inline static const ShogiPiece ShogiPSB{ &PSilver::ThePSilver, &Color::Black, &ShogiPiece::ShogiPSB, &ShogiPiece::ShogiSSB, IDB_SHOGI_PS_B, IDB_SHOGI_PS_S };
    inline static const ShogiPiece ShogiPBB{ &PBishop::ThePBishop, &Color::Black, &ShogiPiece::ShogiPBB, &ShogiPiece::ShogiSBB, IDB_SHOGI_PB_B, IDB_SHOGI_PB_S };
    inline static const ShogiPiece ShogiPRB{ &PRook::ThePRook,     &Color::Black, &ShogiPiece::ShogiPRB, &ShogiPiece::ShogiSRB, IDB_SHOGI_PR_B, IDB_SHOGI_PR_S };
    inline static const ShogiPiece ShogiPNB{ &PKnight::ThePKnight, &Color::Black, &ShogiPiece::ShogiPNB, &ShogiPiece::ShogiSNB, IDB_SHOGI_PN_B, IDB_SHOGI_PN_S };
    inline static const ShogiPiece ShogiPLB{ &PLance::ThePLance,   &Color::Black, &ShogiPiece::ShogiPLB, &ShogiPiece::ShogiSLB, IDB_SHOGI_PL_B, IDB_SHOGI_PL_S };
    inline static const ShogiPiece ShogiPPB{ &PPawn::ThePPawn,     &Color::Black, &ShogiPiece::ShogiPPB, &ShogiPiece::ShogiSPB, IDB_SHOGI_PP_B, IDB_SHOGI_PP_S };
  };


  class ShogiPosition : public MainPosition
  {
  public:
    ShogiPosition(Coordinate x, Coordinate y) noexcept;
    virtual inline MainPosition* Clone(void) const override { return new ShogiPosition(*this); };
    virtual void GetAllMoves(void);              // generate all moves and save list
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;

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
    inline ShogiTakenPosition(Coordinate x, Coordinate /*y*/) noexcept : TakenPosition(x == 9 ? 27 : 15, 2) {}
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
    ShogiGame(ShogiPosition* p, TakenPosition* t, StockPosition* s, ShogiLayout* l, ShogiTakenLayout* tl, ShogiStockLayout* sl) noexcept;
  public:
    ShogiGame(Coordinate x, Coordinate y) noexcept;
    inline constexpr static bool IsFull(Coordinate x, Coordinate /*y*/) noexcept { return x == 9; } //only check for x == 9 -> full Shogi game, all others are Mini
    static const VariantList& GetVariants(void) noexcept;
  };

}
