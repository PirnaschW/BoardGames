
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
    constexpr inline Checker(void) noexcept : Kind('0') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 0; }

  public:
    static const Checker TheChecker;
  };

  class TicTacToePiece : public Piece
  {
  private:
    inline TicTacToePiece(const Kind* k, const Color* c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
    TicTacToePiece(const TicTacToePiece&) = delete;
    TicTacToePiece& operator=(const TicTacToePiece&) = delete;

  public:
    static const TicTacToePiece TicTacToePieceB;
    static const TicTacToePiece TicTacToePieceW;
  };


  class TicTacToePosition : public MainPosition
  {
  public:
    inline TicTacToePosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d) {}

    virtual inline MainPosition* Clone(void) const noexcept override { return new TicTacToePosition(*this); }
    virtual void GetAllMoves(void) noexcept override;
    virtual void EvaluateStatically(void) noexcept override;

  protected:
    inline unsigned int GetChainValue(unsigned int z) const noexcept;
  };


  class TicTacToeGame : public Game
  {
  private:
    TicTacToeGame(void) = delete;

  public:
    inline TicTacToeGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new TicTacToePosition(m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
