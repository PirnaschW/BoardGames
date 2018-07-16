
namespace Cam
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class Pawn : public Kind
  {
  private:
    constexpr Pawn(void) noexcept : Kind('P') {}

  public:
    void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const noexcept override { return 20; }
    
  public:
    inline const static Pawn ThePawn{};
  };

  class Knight : public Kind
  {
  private:
    constexpr Knight(void) noexcept : Kind('N') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    unsigned int GetValue(void) const noexcept override { return 100; }
    
  public:
    inline const static Knight TheKnight{};
  };


  class CamPiece : public Piece
  {
  private:
    CamPiece(const Kind* k, const Color* c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
    CamPiece(const CamPiece&) = delete;
    CamPiece& operator=(const CamPiece&) = delete;

  public:
    inline const static CamPiece WP{ &Pawn::ThePawn,     &Color::White, IDB_WPL, IDB_WPD, IDB_WPS };
    inline const static CamPiece WN{ &Knight::TheKnight, &Color::White, IDB_WNL, IDB_WND, IDB_WNS };
    inline const static CamPiece BP{ &Pawn::ThePawn,     &Color::Black, IDB_BPL, IDB_BPD, IDB_BPS };
    inline const static CamPiece BN{ &Knight::TheKnight, &Color::Black, IDB_BNL, IDB_BND, IDB_BNS };
  };


  // class to handle multi-jumps specific to Cam
  class CollectMoves
  {
  public:
    static bool CollectJumps(const MainPosition& pos, const Location& fr, const std::vector<Step>& s, bool canter, const Color* c, std::vector<Move>& m);
    static void CollectSlides(const MainPosition& pos, const Location& fr, std::vector<Move>& m);
    static std::vector<Move> EnforceJumps(std::vector<Move>& moves);
  };


  class CamPosition : public MainPosition
  {
  public:
    CamPosition(unsigned int x, unsigned int y) noexcept;
    inline virtual MainPosition* Clone(void) const override { return new CamPosition(*this); }
    virtual void GetAllMoves(void) override;
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;
    virtual void EvaluateStatically(void) override;
  };

  
  class CamLayout : public MainLayout
  {
  public:
    CamLayout(unsigned int x, unsigned int y) noexcept :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
    ~CamLayout() {}
  };

  class CamTakenLayout : public TakenLayout
  {
  public:
    CamTakenLayout(unsigned int x, unsigned int y) noexcept :
      TakenLayout(Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
    ~CamTakenLayout() {}
  };

  class CamStockLayout : public StockLayout
  {
  public:
    CamStockLayout(unsigned int /*x*/, unsigned int y) noexcept :
      StockLayout(Dimension(3, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
  };


  class CamGame : public Game
  {
  public:
    CamGame(unsigned int x, unsigned int y) noexcept;
    inline constexpr static bool IsFull(unsigned int x, unsigned int /*y*/) noexcept { return x == 12; } //only check for x == 12 -> full Camelot game, all others are Cam.
  protected:
    CamGame(void) = delete;
    CamGame(CamPosition* p, TakenPosition* t, StockPosition* s, CamLayout* l, CamTakenLayout* tl, CamStockLayout* sl) noexcept;
  };

}