
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

  enum AtaxxVariant : VariantCode   // recognized variants of Ataxx
  {
    Ataxx = 0x00,
    KnightJumps = 0x01,
    Assimilation = Ataxx | KnightJumps,
  };

  class Checker : public BoardGamesCore::Checker
  {
  private:
    constexpr Checker(void) noexcept {}
  public:
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
    AtaxxPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept;
    virtual MainPosition* Clone(void) const noexcept override { return new AtaxxPosition(*this); }
  };


  class AtaxxGame : public Game
  {
  private:
    AtaxxGame(void) = delete;

  public:
    AtaxxGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, new AtaxxPosition(v, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
