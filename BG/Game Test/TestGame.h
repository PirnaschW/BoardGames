
using namespace BoardGamesCore;


namespace Test
{
  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;
  using namespace BoardGamesChessPieces;

  class TestPosition : public MainPosition
  {
  public:
    TestPosition(const PieceMapP& m, Coordinate x, Coordinate y);
    ~TestPosition() override {}
    virtual MainPosition* Clone(void) const override { return new TestPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const override;
  };


  class TestLayout : public MainLayout
  {
  public:
    TestLayout(Coordinate x, Coordinate y) : MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
    ~TestLayout() {}
  };

  class TestTakenLayout : public TakenLayout
  {
  public:
    TestTakenLayout(Coordinate x, Coordinate y) :
      TakenLayout(Dimension(x*y / 2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
    ~TestTakenLayout() {}
  };

  class TestStockLayout : public StockLayout
  {
  public:
    TestStockLayout(Coordinate /*x*/, Coordinate y) :
      StockLayout(Dimension(5, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (y + 1), FieldSizeX, FieldSizeY)) {}
    ~TestStockLayout() {}
  };


  class TestGame : public Game
  {
  private:
    TestGame(const PieceMapP& m, TestPosition* p, TakenPosition* t, StockPosition* s, TestLayout* l, TestTakenLayout* tl, TestStockLayout* sl);

  public:
    TestGame(const PieceMapP& m, Coordinate x, Coordinate y);
    MainPosition* GetPosition(void) { return pos; }
    inline static const VariantList& GetVariants(void) noexcept { static VariantList v{ { Variant{ 2, 4, nullptr, 2, 20 } } }; return v; }
  };


  class Test
  {
  public:

    static bool TestAll(void);
    static bool TestOffset(void);
    static bool TestEvaluate(void);
    static bool TestMoveUndo(const MainPosition* pos);
    static bool TestTaken(const MainPosition* pos);
    static bool TestPosition(const MainPosition* pos);
    static bool TestPList(const AIContext& plist);
    static bool TestValue(const MainPosition* pos);
  };


}
