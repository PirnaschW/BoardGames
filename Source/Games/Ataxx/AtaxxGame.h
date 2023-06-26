
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
    constexpr Checker() noexcept {}
  public:
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

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
    ~AtaxxPiece() override {}

  public:
    static const AtaxxPiece AtaxxPieceW;
    static const AtaxxPiece AtaxxPieceB;
  };


  class AtaxxBoard : public Board
  {
  public:
    AtaxxBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept;
    virtual Board* Clone() const noexcept override { return new AtaxxBoard(*this); }
    virtual void SetStartingBoard() noexcept override;
  };


  class AtaxxGame : public Game
  {
  private:
    AtaxxGame() = delete;

  public:
    AtaxxGame(const VariantChosen& v, const std::vector<PieceIndex>& list, const BoardPartDimensions& d) noexcept : Game(v,list,new AtaxxBoard(v, d)) {}
    static void Register() noexcept;
    static const VariantList& GetVariants() noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
