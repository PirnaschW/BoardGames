
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

  class Checker : public Kind
  {
  private:
    constexpr Checker(void) noexcept : Kind('0') {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 0; }

  public:
    static const Checker TheChecker;
  };

  class TicTacToePiece : public Piece
  {
  private:
    TicTacToePiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    TicTacToePiece(const TicTacToePiece&) = delete;
    TicTacToePiece& operator=(const TicTacToePiece&) = delete;

  public:
    static const TicTacToePiece TicTacToePieceB;
    static const TicTacToePiece TicTacToePieceW;
  };


  class TicTacToePosition : public MainPosition
  {
  public:
    TicTacToePosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(c, p, d) {}

    virtual MainPosition* Clone(void) const noexcept override { return new TicTacToePosition(*this); }
    virtual void GetAllMoves(void) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;

  protected:
    virtual unsigned int GetChainValue(unsigned int z) const noexcept override;
  };


  class TicTacToeGame : public Game
  {
  private:
    TicTacToeGame(void) = delete;

  public:
    TicTacToeGame(VariantCode c, const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new TicTacToePosition(c, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(VariantCode c) noexcept;
    static const Dimensions& GetDimensions(const VariantChosen& v) noexcept;
  };

}
