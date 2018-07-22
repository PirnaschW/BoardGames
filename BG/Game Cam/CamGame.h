
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
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override;
    void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    
  public:
    inline const static Pawn ThePawn{};
  };

  class Knight : public Kind
  {
  private:
    constexpr inline Knight(void) noexcept : Kind('N') {}
  public:
    unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override;
    void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    
  public:
    inline const static Knight TheKnight{};
  };


  class CamPiece : public Piece
  {
  private:
    inline CamPiece(const Kind* k, const Color* c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
    CamPiece(const CamPiece&) = delete;
    CamPiece& operator=(const CamPiece&) = delete;

  public:
    inline const static CamPiece WP{ &Pawn::ThePawn,     &Color::White, IDB_WPL, IDB_WPD, IDB_WPS };
    inline const static CamPiece WN{ &Knight::TheKnight, &Color::White, IDB_WNL, IDB_WND, IDB_WNS };
    inline const static CamPiece BP{ &Pawn::ThePawn,     &Color::Black, IDB_BPL, IDB_BPD, IDB_BPS };
    inline const static CamPiece BN{ &Knight::TheKnight, &Color::Black, IDB_BNL, IDB_BND, IDB_BNS };
  };


  class CamPosition : public MainPosition
  {
  public:
    CamPosition(unsigned int x, unsigned int y) noexcept;
    inline virtual MainPosition* Clone(void) const override { return new CamPosition(*this); }
    virtual void GetAllMoves(void) override;
    // extensions:
  public:
    bool CollectJumps(const Location& fr, const std::vector<Step>& s, bool charge, const Color* c, std::vector<Move>& m) const;
    std::vector<Move> EnforceJumps(std::vector<Move>& moves) const;
  };

  class CamTakenPosition : public TakenPosition
  {
  public:
    inline CamTakenPosition(unsigned int x, unsigned int /*y*/) noexcept :
      TakenPosition(x == 12 ? 24 : 14, 2) {}
  };

  
  class CamLayout : public MainLayout
  {
  public:
    inline CamLayout(unsigned int x, unsigned int y) noexcept :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
  };

  class CamTakenLayout : public TakenLayout
  {
  public:
    inline CamTakenLayout(unsigned int x, unsigned int y) noexcept :
      TakenLayout(Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  };

  class CamStockLayout : public StockLayout
  {
  public:
    inline CamStockLayout(unsigned int /*x*/, unsigned int y) noexcept :
      StockLayout(Dimension(3, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
  };


  class CamGame : public Game
  {
  protected:
    CamGame(void) = delete;
    CamGame(CamPosition* p, TakenPosition* t, StockPosition* s, CamLayout* l, CamTakenLayout* tl, CamStockLayout* sl) noexcept;
  public:
    CamGame(unsigned int x, unsigned int y) noexcept;
    inline static const VariantList& GetVariants(void) noexcept { static VariantList v{ { Variant{ 12, 16, "Camelot" },{ Variant{ 7, 13, "Cam" }}} }; return v; }
  };

}