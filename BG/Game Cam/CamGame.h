
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
    constexpr inline Pawn(void) noexcept : Kind('P') {}

  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 100; };
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    
  public:
    inline const static Pawn ThePawn{};
  };

  class Knight : public Kind
  {
  private:
    constexpr inline Knight(void) noexcept : Kind('N') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 400; }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    
  public:
    inline const static Knight TheKnight{};
  };


  class CamPiece : public Piece
  {
  private:
    inline CamPiece(const Kind* k, const Color* c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
    CamPiece(const CamPiece&) = delete;
    CamPiece& operator=(const CamPiece&) = delete;
    virtual unsigned int GetValue(const MainPosition& p, const Location l) const noexcept override;

  public:
    inline static const CamPiece WP{ &Pawn::ThePawn,     &Color::White, IDB_WPL, IDB_WPD, IDB_WPS };
    inline static const CamPiece WN{ &Knight::TheKnight, &Color::White, IDB_WNL, IDB_WND, IDB_WNS };
    inline static const CamPiece BP{ &Pawn::ThePawn,     &Color::Black, IDB_BPL, IDB_BPD, IDB_BPS };
    inline static const CamPiece BN{ &Knight::TheKnight, &Color::Black, IDB_BNL, IDB_BND, IDB_BNS };
  };


  class CamPosition : public MainPosition
  {
  public:
    CamPosition(Coordinate x, Coordinate y) noexcept;
    virtual inline MainPosition* Clone(void) const override { return new CamPosition(*this); }
    virtual void GetAllMoves(void) override;
 
    // extensions:
  public:
    bool CollectJumps(const Location& fr, const std::vector<Step>& s, bool charge, const Color* c, std::vector<Move>& m) const;
  };

  class CamTakenPosition : public TakenPosition
  {
  public:
    inline CamTakenPosition(Coordinate x, Coordinate /*y*/) noexcept : TakenPosition(x == 12 ? 24 : 14, 2) {}
  };

  
  class CamLayout : public MainLayout
  {
  public:
    inline CamLayout(Coordinate x, Coordinate y) noexcept :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
  };

  class CamTakenLayout : public TakenLayout
  {
  public:
    inline CamTakenLayout(Coordinate x, Coordinate y) noexcept :
      TakenLayout(Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  };

  class CamStockLayout : public StockLayout
  {
  public:
    inline CamStockLayout(Coordinate /*x*/, Coordinate y) noexcept :
      StockLayout(Dimension(3, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
  };


  class CamGame : public Game
  {
  private:
    CamGame(void) = delete;
    CamGame(CamPosition* p, TakenPosition* t, StockPosition* s, CamLayout* l, CamTakenLayout* tl, CamStockLayout* sl) noexcept;
  public:
    CamGame(Coordinate x, Coordinate y) noexcept;
    static const VariantList& GetVariants(void) noexcept;
  };

}