
using namespace BoardGamesCore;

#ifdef TESTING
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


  class TestGame : public Game
  {
  private:
    TestGame(const PieceMapP& m, TestPosition* p, TestLayout* l);

  public:
    TestGame(const PieceMapP& m, Coordinate x, Coordinate y);
    MainPosition* GetPosition(void) { return pos; }
    inline static const VariantList& GetVariants(void) noexcept { static VariantList v{ { Variant{ 2, 4, nullptr, 2, 20 } } }; return v; }
    static const PieceMapP& GetPieces(void) noexcept;
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
#endif
