
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
    Pawn(void) : Kind('p') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    unsigned int GetValue(void) const noexcept override { return 1; }

  public:
    inline const static Pawn ThePawn{};

  };

  class Knight : public Kind
  {
  private:
    Knight(void) : Kind('n') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 4; }

  public:
    inline const static Knight TheKnight{};

  };

  class Bishop : public Kind
  {
  private:
    Bishop(void) : Kind('b') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 8; }

  public:
    inline const static Bishop TheBishop{};
  };

  class Rook : public Kind
  {
  private:
    Rook(void) : Kind('r') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 10; }

  public:
    inline const static Rook TheRook{};
  };

  class Lance : public Kind
  {
  private:
    Lance(void) : Kind('l') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 3; }

  public:
    inline const static Lance TheLance{};
  };

  class Silver : public Kind
  {
  private:
    Silver(void) : Kind('s') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 5; }

  public:
    inline const static Silver TheSilver{};
  };

  class Gold : public Kind
  {
  private:
    Gold(void) : Kind('G') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 6; }
    static void CollectGoldMoves(const MainPosition&, const Location&, std::vector<Move>&); // enable reuse for other kinds

  public:
    inline const static Gold TheGold{};
  };

  class King : public Kind
  {
  private:
    King(void) : Kind('K') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 8; }

  public:
    inline const static King TheKing{};
  };

  class PPawn : public Kind
  {
  private:
    PPawn(void) : Kind('P') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 7; }

  public:
    inline const static PPawn ThePPawn{};
  };

  class PKnight : public Kind
  {
  private:
    PKnight(void) : Kind('N') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 6; }

  public:
    inline const static PKnight ThePKnight{};
  };

  class PBishop : public Kind
  {
  private:
    PBishop(void) : Kind('B') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 10; }

  public:
    inline const static PBishop ThePBishop{};
  };

  class PRook : public Kind
  {
  private:
    PRook(void) : Kind('R') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 12; }

  public:
    inline const static PRook ThePRook{};
  };

  class PLance : public Kind
  {
  private:
    PLance(void) : Kind('L') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 6; }

  public:
    inline const static PLance ThePLance{};
  };

  class PSilver : public Kind
  {
  private:
    PSilver(void) : Kind('S') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 6; }

  public:
    inline const static PSilver ThePSilver{};
  };


  class ShogiPiece : public Piece
  {
  private:
    ShogiPiece(const Kind* k, const Color* c, const ShogiPiece * u, const ShogiPiece * d, UINT l, UINT s) : Piece(k, c, l, l, s), up(u), down(d) {}
    ShogiPiece(const ShogiPiece&) = delete;
    ShogiPiece& operator=(const ShogiPiece&) = delete;
  public:
    virtual ~ShogiPiece(void) override {}

    virtual bool IsPromotable(void) const noexcept override { return up != this; }            // is this a promotable piece?
    virtual const Piece* Promote(bool u) const noexcept override { return u ? up : down; }    // promote this piece up/down

  private:
    const ShogiPiece * up;    // what this piece promotes up to
    const ShogiPiece * down;  // what this piece promotes down to

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
    ShogiPosition(unsigned int x, unsigned int y) : MainPosition(x, y) {}
    virtual ~ShogiPosition() override {}
    virtual MainPosition* Clone(void) const override = 0;
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const;
    virtual Move::PositionValue EvaluateStatically(void) override;

  public: // extensions to base class
    virtual bool CanPromote(const Location &l) const = 0;
  };


  class ShogiLayout : public MainLayout
  {
  public:
    ShogiLayout(unsigned int x, unsigned int y) :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY), LayoutType::Light) {}
    virtual ~ShogiLayout() {}
  };

  class ShogiTakenLayout : public TakenLayout
  {
  public:
    ShogiTakenLayout(unsigned int x, unsigned int /*y*/) :
      TakenLayout(Dimension(3 * x, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * x - FieldSizeSY * 4)) { }
    virtual ~ShogiTakenLayout() {}
  };

  class ShogiStockLayout : public StockLayout
  {
  public:
    ShogiStockLayout(unsigned int x, unsigned int y) :
      StockLayout(Dimension(15, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY)) {}
    virtual ~ShogiStockLayout() {}
  };


  class ShogiGame : public Game
  {
  protected:
    ShogiGame(void) = delete;
    ShogiGame(ShogiPosition* p, TakenPosition* t, StockPosition* s, ShogiLayout* l, ShogiTakenLayout* tl, ShogiStockLayout* sl);
  public:
    virtual ~ShogiGame(void) override {}
  };

  class MiniShogiPosition : public ShogiPosition
  {
  public:
    MiniShogiPosition(unsigned int x, unsigned int y);
    virtual MainPosition* Clone(void) const override { return new MiniShogiPosition(*this); }
    virtual bool CanPromote(const Location &l) const;
  };

  class FullShogiPosition : public ShogiPosition
  {
  public:
    FullShogiPosition(unsigned int x, unsigned int y);
    virtual MainPosition* Clone(void) const override { return new FullShogiPosition(*this); }
    virtual bool CanPromote(const Location &l) const;
  };

  class MiniShogiGame : public ShogiGame
  {
  public:
    MiniShogiGame(void) :
      ShogiGame(
        new MiniShogiPosition(5, 5), new TakenPosition(15, 2), new StockPosition(15, 2),
        new ShogiLayout(5, 5), new ShogiTakenLayout(5, 5), new ShogiStockLayout(5, 5)) {}
  };

  class FullShogiGame : public ShogiGame
  {
  public:
    FullShogiGame(void) :
      ShogiGame(
        new FullShogiPosition(9, 9), new TakenPosition(27, 2), new StockPosition(15, 2),
        new ShogiLayout(9, 9), new ShogiTakenLayout(9, 9), new ShogiStockLayout(9, 9)) {}
  };

}
