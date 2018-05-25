
namespace Template
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class Checker : public Kind
  {
  private:
    constexpr Checker(void) noexcept : Kind('0') {}
  public:
    void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const override;
    unsigned int GetValue(void) const noexcept override { return 8; }

  public:
    inline const static Checker TheChecker{};
  };

  class TemplatePiece : public Piece
  {
  private:
    TemplatePiece(const Kind* k, const Color* c, UINT l, UINT s) noexcept : Piece(k, c, l, l, s) {}
    TemplatePiece(const TemplatePiece&) = delete;
    TemplatePiece& operator=(const TemplatePiece&) = delete;
  public:
    virtual ~TemplatePiece(void) override {}

  public:
    static const TemplatePiece TemplatePieceB;
    static const TemplatePiece TemplatePieceW;
  };


  class TemplatePosition : public MainPosition
  {
  public:
    TemplatePosition(unsigned int x, unsigned int y) noexcept;
    virtual ~TemplatePosition() override {}
    virtual MainPosition* Clone(void) const override { return new TemplatePosition(*this); }
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;
    virtual Move::PositionValue EvaluateStatically(void) override;
  };


  class TemplateLayout : public MainLayout
  {
  public:
    TemplateLayout(unsigned int x, unsigned int y) noexcept : MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
    virtual ~TemplateLayout() {}
  };

  class TemplateTakenLayout : public TakenLayout
  {
  public:
    TemplateTakenLayout(unsigned int x, unsigned int y) noexcept :
      TakenLayout(Dimension(x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
    virtual ~TemplateTakenLayout() {}
  };

  class TemplateStockLayout : public StockLayout
  {
  public:
    TemplateStockLayout(unsigned int x, unsigned int y) noexcept :
      StockLayout(Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
    virtual ~TemplateStockLayout() {}
  };


  class TemplateGame : public Game
  {
  private:
    TemplateGame(TemplatePosition* p, TakenPosition* t, StockPosition* s,
      TemplateLayout* l, TemplateTakenLayout* tl, TemplateStockLayout* sl) noexcept : Game{p,t,s,l,tl,sl}
    {
      AddToStock(Location(0, 0), &TemplatePiece::TemplatePieceW);
      AddToStock(Location(1, 0), &TemplatePiece::TemplatePieceB);
    }
  public:
    TemplateGame(unsigned int x, unsigned int y) noexcept : TemplateGame(
      new TemplatePosition(x, y), new TakenPosition(x, 2), new StockPosition(3, 1),
      new TemplateLayout(x, y), new TemplateTakenLayout(x, y), new TemplateStockLayout(x, y)) {}
    virtual ~TemplateGame(void) override {};
  };

}
