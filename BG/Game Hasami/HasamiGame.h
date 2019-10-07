
namespace Hasami
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 27;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 27;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class Checker : public Kind
  {
  private:
    constexpr inline Checker(void) noexcept : Kind('0') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 1000; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;

  public:
    static const Checker TheChecker;
  };

  class HasamiPiece : public Piece
  {
  private:
    inline HasamiPiece(const Kind* k, const Color* c, UINT l, UINT s) noexcept : Piece(k, c, l, l, s) {}
    HasamiPiece(const HasamiPiece&) = delete;
    HasamiPiece& operator=(const HasamiPiece&) = delete;

  public:  // the pieces
    static const HasamiPiece HasamiPieceB;
    static const HasamiPiece HasamiPieceW;
  };


  class HasamiPosition : public MainPosition
  {
  public:
    HasamiPosition(const PieceMapP& p, const Dimensions& d) noexcept;
    virtual inline MainPosition* Clone(void) const noexcept override { return new HasamiPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const noexcept override;
    virtual void EvaluateStatically(void) noexcept override;

  protected:
    inline unsigned int GetChainValue(unsigned int z) const noexcept;
  };

  class HasamiGame : public Game
  {
  private:
    HasamiGame(void) = delete;
  public:
    inline HasamiGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new HasamiPosition(m,d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
