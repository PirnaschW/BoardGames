
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
    Pawn(void) : Kind('P') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const override { return 20; }
    
  public:
    inline const static Pawn ThePawn{};
  };

  class Knight : public Kind
  {
  private:
    Knight(void) : Kind('N') {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    virtual unsigned int GetValue(void) const override { return 100; }
    
  public:
    inline const static Knight TheKnight{};
  };


  class CamPiece : public Piece
  {
  private:
    CamPiece(const Kind* k, const Color* c, UINT l, UINT d, UINT s) : Piece(k, c, l, d, s) {}
    CamPiece(const CamPiece&) = delete;
    CamPiece& operator=(const CamPiece&) = delete;

  public:
    virtual ~CamPiece(void) override {}
    const static CamPiece WP;
    const static CamPiece WN;
    const static CamPiece BP;
    const static CamPiece BN;
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
    CamPosition(unsigned int x, unsigned int y) : MainPosition(x, y) {}
    virtual ~CamPosition(void) override {}
    virtual MainPosition* Clone(void) const override = 0;
    virtual void GetAllMoves(void) override;
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;
    virtual Move::PositionValue EvaluateStatically(void) override;
  };



  class CamLayout : public MainLayout
  {
  public:
    CamLayout(unsigned int x, unsigned int y) :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
    virtual ~CamLayout() {}
  };


  class CamTakenLayout : public TakenLayout
  {
  public:
    CamTakenLayout(unsigned int x, unsigned int y) :
      TakenLayout(Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
    virtual ~CamTakenLayout() {}
  };

  class CamStockLayout : public StockLayout
  {
  public:
    CamStockLayout(unsigned int /*x*/, unsigned int y) :
      StockLayout(Dimension(3, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
    virtual ~CamStockLayout() {}
  };


  class CamGame : public Game
  {
  protected:
    CamGame(void) = delete;
    CamGame(CamPosition* p, TakenPosition* t, StockPosition* s, CamLayout* l, CamTakenLayout* tl, CamStockLayout* sl);
  public:
    virtual ~CamGame(void) override {}
  };


  class MiniCamPosition : public CamPosition
  { 
  public: 
    MiniCamPosition(unsigned int x, unsigned int y);
    virtual MainPosition* Clone(void) const override { return new MiniCamPosition(*this); }

  };
  class FullCamPosition : public CamPosition 
  
  { 
  public: 
    FullCamPosition(unsigned int x, unsigned int y); 
    virtual MainPosition* Clone(void) const override { return new FullCamPosition(*this); }
  };

  class MiniCamGame : public CamGame
  {
  public:
    MiniCamGame(void) :
      CamGame(
        new MiniCamPosition(7, 13), new TakenPosition(14, 2), new StockPosition(3, 2),
        new CamLayout(7, 13), new CamTakenLayout(7, 13), new CamStockLayout(7, 13)) {}
  };
  class FullCamGame : public CamGame
  {
  public:
    FullCamGame(void) :
      CamGame(
        new FullCamPosition(12, 16), new TakenPosition(24, 2), new StockPosition(3, 2),
        new CamLayout(12, 16), new CamTakenLayout(12, 16), new CamStockLayout(12, 16)) {}
  };

}