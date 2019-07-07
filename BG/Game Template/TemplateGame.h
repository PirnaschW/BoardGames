
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
    static const Checker TheChecker;
  };

  class TemplatePiece : public Piece
  {
  private:
    inline TemplatePiece(const Kind* k, const Color* c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
    TemplatePiece(const TemplatePiece&) = delete;
    TemplatePiece& operator=(const TemplatePiece&) = delete;
  public:
    ~TemplatePiece(void) override {}

  public:
    static const TemplatePiece TemplatePieceW;
    static const TemplatePiece TemplatePieceB;
  };


  class TemplatePosition : public MainPosition
  {
  public:
    inline TemplatePosition(const PieceMapP& p, Coordinate x, Coordinate y) noexcept : MainPosition(p, x, y) {}
    virtual inline MainPosition* Clone(void) const override { return new TemplatePosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const override;
    virtual void EvaluateStatically(void) override;
  };


  class TemplateLayout : public MainLayout
  {
  public:
    inline TemplateLayout(Coordinate x, Coordinate y) noexcept : 
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
  };

  class TemplateGame : public Game
  {
  private:
    TemplateGame(void) = delete;
    TemplateGame(const PieceMapP& m, TemplatePosition* p, TemplateLayout* l) noexcept;

  public:
    TemplateGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept;
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
  };

}
