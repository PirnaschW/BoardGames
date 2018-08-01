
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
    constexpr inline Checker(void) noexcept : Kind('0') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 100; }

  public:
    inline const static Checker TheChecker{};
  };

  class TemplatePiece : public Piece
  {
  private:
    inline TemplatePiece(const Kind* k, const Color* c, UINT l, UINT s) noexcept : Piece(k, c, l, l, s) {}
    TemplatePiece(const TemplatePiece&) = delete;
    TemplatePiece& operator=(const TemplatePiece&) = delete;
  public:
    ~TemplatePiece(void) override {}

  public:
    inline static const TemplatePiece TemplatePieceB{ &Checker::TheChecker, &Color::Black, IDB_LOAPEGB, IDB_LOAPEGBF };
    inline static const TemplatePiece TemplatePieceW{ &Checker::TheChecker, &Color::White, IDB_LOAPEGW, IDB_LOAPEGWF };
  };


  class TemplatePosition : public MainPosition
  {
  public:
    inline TemplatePosition(Coordinate x, Coordinate y) noexcept : MainPosition(x, y) {}
    virtual inline MainPosition* Clone(void) const override { return new TemplatePosition(*this); }
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;
    virtual void EvaluateStatically(void) override;
  };


  class TemplateLayout : public MainLayout
  {
  public:
    inline TemplateLayout(Coordinate x, Coordinate y) noexcept : 
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
  };

  class TemplateTakenLayout : public TakenLayout
  {
  public:
    inline TemplateTakenLayout(Coordinate x, Coordinate y) noexcept :
      TakenLayout(Dimension(x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  };

  class TemplateStockLayout : public StockLayout
  {
  public:
    inline TemplateStockLayout(Coordinate x, Coordinate y) noexcept :
      StockLayout(Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
  };


  class TemplateGame : public Game
  {
  private:
    TemplateGame(void) = delete;
    TemplateGame(TemplatePosition* p, TakenPosition* t, StockPosition* s, TemplateLayout* l, TemplateTakenLayout* tl, TemplateStockLayout* sl) noexcept;

  public:
    TemplateGame(Coordinate x, Coordinate y) noexcept;
    static const VariantList& GetVariants(void) noexcept;
  };

}
