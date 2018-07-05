
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
    inline const static Checker TheChecker{};
  };

  class TicTacToePiece : public Piece
  {
  private:
    TicTacToePiece(const Kind* k, const Color* c, UINT l, UINT s) noexcept : Piece(k, c, l, l, s) {}
    TicTacToePiece(const TicTacToePiece&) = delete;
    TicTacToePiece& operator=(const TicTacToePiece&) = delete;

  public:
    static const TicTacToePiece TicTacToePieceB;
    static const TicTacToePiece TicTacToePieceW;
  };


  class TicTacToePosition : public MainPosition
  {
  public:
    TicTacToePosition(unsigned int x, unsigned int y) noexcept;
    ~TicTacToePosition() override {}
    virtual MainPosition* Clone(void) const override { return new TicTacToePosition(*this); }
    virtual void GetAllMoves(void) override;                                                            // collect all moves
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;
    virtual void EvaluateStatically(void) override;
    virtual unsigned int GetValue(unsigned int z) const noexcept;
  };


  class TicTacToeLayout : public MainLayout
  {
  public:
    TicTacToeLayout(unsigned int x, unsigned int y) noexcept;
  };

  class TicTacToeTakenLayout : public TakenLayout
  {
  public:
    TicTacToeTakenLayout(unsigned int x, unsigned int y) noexcept :
      TakenLayout(Dimension(x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  };

  class TicTacToeStockLayout : public StockLayout
  {
  public:
    TicTacToeStockLayout(unsigned int x, unsigned int y) noexcept :
      StockLayout(Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
  };


  class TicTacToeGame : public Game
  {
  private:
    TicTacToeGame(TicTacToePosition* p, TakenPosition* t, StockPosition* s,
      TicTacToeLayout* l, TicTacToeTakenLayout* tl, TicTacToeStockLayout* sl) noexcept : Game{ p,t,s,l,tl,sl,true }
    {
      AddToStock(Location(0, 0), &TicTacToePiece::TicTacToePieceW);
      AddToStock(Location(1, 0), &TicTacToePiece::TicTacToePieceB);
    }
  public:
    TicTacToeGame(unsigned int x, unsigned int y) noexcept : TicTacToeGame(
      new TicTacToePosition(x, y), new TakenPosition(x, 2), new StockPosition(3, 1),
      new TicTacToeLayout(x, y), new TicTacToeTakenLayout(x, y), new TicTacToeStockLayout(x, y)) {}
  };

}
