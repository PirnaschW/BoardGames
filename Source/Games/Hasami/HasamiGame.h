
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
    constexpr Checker() noexcept : Kind('0') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 1000; }
    virtual void CollectMoves(const Board&, const Location&, Moves&) const noexcept override;

  public:
    static const Checker TheChecker;
  };

  class HasamiPiece : public Piece
  {
  private:
    HasamiPiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    HasamiPiece(const HasamiPiece&) = delete;
    HasamiPiece& operator=(const HasamiPiece&) = delete;

  public:  // the pieces
    static const HasamiPiece HasamiPieceB;
    static const HasamiPiece HasamiPieceW;
  };


  class HasamiBoard : public Board
  {
  public:
    HasamiBoard(const VariantChosen& v, const PieceMapP& p, const BoardPartDimensions& d) noexcept : Board(v, p, d) {};
    virtual Board* Clone() const noexcept override { return new HasamiBoard(*this); }
    virtual void SetStartingBoard() noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically() const noexcept override;
  };

  class HasamiGame : public Game
  {
  private:
    HasamiGame() = delete;
  public:
    HasamiGame(const VariantChosen& v, const PieceMapP& m, const BoardPartDimensions& d) noexcept : Game(v, m, new HasamiBoard(v, m,d)) {}
    static const VariantList& GetVariants() noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
