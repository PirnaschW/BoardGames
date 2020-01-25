
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
    constexpr inline Checker(void) noexcept : Kind('0') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;

  public:
    static const Checker TheChecker;
  };

  class AtaxxPiece : public Piece
  {
  private:
    inline AtaxxPiece(const Kind& k, const Color& c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
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
    AtaxxPosition(const PieceMapP& p, const Dimensions& d) noexcept;
    virtual inline MainPosition* Clone(void) const noexcept override { return new AtaxxPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
  };


  class AtaxxGame : public Game
  {
  private:
    AtaxxGame(void) = delete;

  public:
    inline AtaxxGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new AtaxxPosition(m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
