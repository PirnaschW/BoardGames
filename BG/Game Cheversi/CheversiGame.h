
namespace Cheversi
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
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Checker TheChecker;
  };

  class CheversiPiece : public Piece
  {
  private:
    inline CheversiPiece(const Kind& k, const Color& c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
    CheversiPiece(const CheversiPiece&) = delete;
    CheversiPiece& operator=(const CheversiPiece&) = delete;
  public:
    ~CheversiPiece(void) override {}

  public:
    static const CheversiPiece CheversiPieceW;
    static const CheversiPiece CheversiPieceB;
  };


  class CheversiPosition : public MainPosition
  {
  public:
    inline CheversiPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d) {}
    virtual inline MainPosition* Clone(void) const noexcept override { return new CheversiPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
  };


  class CheversiGame : public Game
  {
  private:
    CheversiGame(void) = delete;

  public:
    inline CheversiGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new CheversiPosition(m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
