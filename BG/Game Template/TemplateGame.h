
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
    inline TemplatePosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d) {}
    virtual inline MainPosition* Clone(void) const noexcept override { return new TemplatePosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const noexcept override;
    virtual void EvaluateStatically(void) noexcept override;
  };


  class TemplateGame : public Game
  {
  private:
    TemplateGame(void) = delete;

  public:
    inline TemplateGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new TemplatePosition(m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
