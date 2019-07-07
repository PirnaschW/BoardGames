
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
    inline TicTacToePosition(const PieceMapP& p, Coordinate x, Coordinate y) noexcept : MainPosition(p, x, y) {}

    virtual inline MainPosition* Clone(void) const override { return new TicTacToePosition(*this); }
    virtual void GetAllMoves(void) override;
//    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const override;
    virtual void EvaluateStatically(void) override;

  protected:
    inline unsigned int GetChainValue(unsigned int z) const noexcept;
  };


  class TicTacToeLayout : public MainLayout
  {
  public:
    inline TicTacToeLayout(Coordinate x, Coordinate y) noexcept :
      MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1), Layout::LayoutType::Light) {}
  };

  //class TicTacToeTakenLayout : public TakenLayout
  //{
  //public:
  //  inline TicTacToeTakenLayout(Coordinate x, Coordinate y) noexcept :
  //    TakenLayout(Dimension(0, 0, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
  //};

  //class TicTacToeStockLayout : public StockLayout
  //{
  //public:
  //  inline TicTacToeStockLayout(Coordinate x, Coordinate y) noexcept :
  //    StockLayout(Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY)) {}
  //};


  class TicTacToeGame : public Game
  {
  private:
    TicTacToeGame(void) = delete;
    TicTacToeGame(const PieceMapP& m, TicTacToePosition* p, TicTacToeLayout* l) noexcept;

  public:
    TicTacToeGame(const PieceMapP& m, Coordinate x, Coordinate y) noexcept;
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
  };

}
