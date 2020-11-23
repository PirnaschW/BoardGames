
namespace Ataxx
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
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;

  public:
    static const Checker TheChecker;
  };

  class AtaxxPiece : public Piece
  {
  private:
    AtaxxPiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    AtaxxPiece(const AtaxxPiece&) = delete;
    AtaxxPiece& operator=(const AtaxxPiece&) = delete;
  public:
    ~AtaxxPiece(void) override {}

  public:
    static const AtaxxPiece AtaxxPieceW;
    static const AtaxxPiece AtaxxPieceB;
  };


  class AtaxxPosition : public MainPosition
  {
  public:
    AtaxxPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept;
    virtual MainPosition* Clone(void) const noexcept override { return new AtaxxPosition(*this); }
  };


  class AtaxxGame : public Game
  {
  private:
    AtaxxGame(void) = delete;

  public:
    AtaxxGame(VariantCode c, const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new AtaxxPosition(c, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(VariantCode c) noexcept;
    static const Dimensions& GetDimensions(VariantCode c, Coordinate x, Coordinate y) noexcept;
  };

}
