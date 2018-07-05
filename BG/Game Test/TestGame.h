
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
    TestPosition(unsigned int x, unsigned int y);
    ~TestPosition() override {}
    virtual MainPosition* Clone(void) const override { return new TestPosition(*this); }
    virtual bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;
  };


  class TestLayout : public MainLayout
  {
  public:
    TestLayout(unsigned int x, unsigned int y) : MainLayout(Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY)) {}
    ~TestLayout() {}
  };

  class TestTakenLayout : public TakenLayout
  {
  public:
    TestTakenLayout(unsigned int x, unsigned int y) :
      TakenLayout(Dimension(x*y / 2, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4)) {}
    ~TestTakenLayout() {}
  };

  class TestStockLayout : public StockLayout
  {
  public:
    TestStockLayout(unsigned int /*x*/, unsigned int y) :
      StockLayout(Dimension(5, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (y + 1), FieldSizeX, FieldSizeY)) {}
    ~TestStockLayout() {}
  };


  class TestGame : public Game
  {
  private:
    TestGame(TestPosition* p, TakenPosition* t, StockPosition* s, TestLayout* l, TestTakenLayout* tl, TestStockLayout* sl);

  public:
    TestGame(unsigned int x, unsigned int y);
    MainPosition* GetPosition(void) { return pos; }
    ~TestGame(void) override {};
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
    static bool TestPList(const MainPosition::PList& plist);
    static bool TestValue(const MainPosition* pos);
  };


}
