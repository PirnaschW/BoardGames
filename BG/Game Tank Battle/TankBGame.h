
namespace TankB
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

  class TankBPiece : public Piece
  {
  private:
    inline TankBPiece(const Kind& k, const Color& c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
    TankBPiece(const TankBPiece&) = delete;
    TankBPiece& operator=(const TankBPiece&) = delete;
  public:
    ~TankBPiece(void) override {}

  public:
    static const TankBPiece TankBPieceW;
    static const TankBPiece TankBPieceB;
  };


  class TankBPosition : public MainPosition
  {
  public:
    inline TankBPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d) {}
    virtual inline MainPosition* Clone(void) const noexcept override { return new TankBPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
  };


  class TankBGame : public Game
  {
  private:
    TankBGame(void) = delete;

  public:
    inline TankBGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new TankBPosition(m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
