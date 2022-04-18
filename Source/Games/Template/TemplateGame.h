
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
    constexpr Checker() noexcept : Kind('0') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Checker TheChecker;
  };

  class TemplatePiece : public Piece
  {
  private:
    TemplatePiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    TemplatePiece(const TemplatePiece&) = delete;
    TemplatePiece& operator=(const TemplatePiece&) = delete;
  public:
    ~TemplatePiece() override {}

  public:
    static const TemplatePiece TemplatePieceW;
    static const TemplatePiece TemplatePieceB;
  };


  class TemplateBoard : public Board
  {
  public:
    TemplateBoard(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : Board(v, p, d) {}
    virtual Board* Clone() const noexcept override { return new TemplateBoard(*this); }
    virtual void SetStartingBoard() noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically() const noexcept override;
  };


  class TemplateGame : public Game
  {
  private:
    TemplateGame() = delete;

  public:
    TemplateGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, new TemplateBoard(v, m, d)) {}
    static const VariantList& GetVariants() noexcept;
    static const PieceMapP& GetPieces() noexcept;
    static const Dimensions GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
