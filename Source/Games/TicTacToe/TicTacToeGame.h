
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

  class TicTacToePosition : public MainPosition
  {
  public:
    TicTacToePosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(v, p, d) {}
    virtual MainPosition* Clone(void) const noexcept override { return new TicTacToePosition(*this); }
    virtual void SetStartingPosition() noexcept override {}
    virtual void GetAllMoves(void) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
  };


  class TicTacToeGame : public Game
  {
  private:
    TicTacToeGame(void) = delete;

  public:
    TicTacToeGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, new TicTacToePosition(v, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
