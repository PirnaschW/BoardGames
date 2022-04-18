
namespace TicTacToe
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class TicTacToeBoard : public Board
  {
  public:
    TicTacToeBoard(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : Board(v, p, d) {}
    virtual Board* Clone() const noexcept override { return new TicTacToeBoard(*this); }
    virtual void SetStartingBoard() noexcept override {}
    virtual void GetAllMoves() const noexcept override;
    virtual PositionValue EvaluateStatically() const noexcept override;
  };


  class TicTacToeGame : public Game
  {
  private:
    TicTacToeGame() = delete;

  public:
    TicTacToeGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, new TicTacToeBoard(v, m, d)) {}
    static const VariantList& GetVariants() noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
