#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BoardGamesCore;

#include "../BG/Game CFour/CFourGame.h"
using namespace CFour;

namespace UnitTestCFour
{
  constexpr Coordinate MaxX{ 8U };
  constexpr Coordinate MaxY{ 8U };

  TEST_CLASS(UnitTestCFour)
  {
  public:

    TEST_METHOD(TestCFourPieces)
    {
      Assert::IsFalse(CFourPiece::CFourPieceW.IsPromotable());
      Assert::IsFalse(CFourPiece::CFourPieceB.IsPromotable());
    }

    TEST_METHOD(TestCFourPositionClone)
    {
      CFourPosition p1{ Variants<CFour::CFourGame>::GetPieces(), CFourGame::GetDimensions(7U, 6U) };
      MainPosition* pm = p1.Clone();
      Assert::IsTrue(pm != nullptr);

      CFourPosition* p2{ dynamic_cast<CFourPosition*>(pm) };
      Assert::IsTrue(p2 != nullptr);
      Assert::IsTrue(p1 == *p2);
    }

    TEST_METHOD(TestCFourPositionEvaluateChain)
    {
      CFourPosition pos{ Variants<CFour::CFourGame>::GetPieces(), CFourGame::GetDimensions(7U, 6U) };
      pos.SetPiece(Location{ BoardPart::Stock, 0, 0 }, CFourPiece::CFourPieceW);
      pos.SetPiece(Location{ BoardPart::Stock, 0, 1 }, CFourPiece::CFourPieceB);

      auto Put = [&pos](int x, int y, const CFourPiece& p)
      {
        const Location& lf_ = p == CFourPiece::CFourPieceW ? Location{ BoardPart::Stock, 0, 0 } : Location{ BoardPart::Stock, 0, 1 };
        const Location& lt_{ BoardPart::Main, static_cast<Coordinate>(x), static_cast<Coordinate>(y) };
        Actions a_{};
        a_.push_back(std::make_shared<ActionLift>(lf_, p));   // pick fresh Piece up
        a_.push_back(std::make_shared<ActionDrop>(lt_, p));   // and place it on target
        pos.Execute(Move{ a_ });                              // create Move and execute it
        pos.EvaluateChainLengths(4);
      };

      Put(2, 5, CFourPiece::CFourPieceW);
      Assert::IsTrue(pos.GetValue(true) == PositionValue(1800));  // 6 directions * (100 + 100) + 2 directions * (100 + 100 + 100)

      Put(3, 5, CFourPiece::CFourPieceW);
      Assert::IsTrue(pos.GetValue(true) == PositionValue(4400));  // 2 Pieces * 6 directions * (100 + 100) + 2 pieces * 1 directions * (100 + 800 + 100)

      Put(4, 5, CFourPiece::CFourPieceW);
      Assert::IsTrue(pos.GetValue(true) == PositionValue(22000));  // 3 Pieces * 6 directions * (100 + 100) + 2 pieces * 1 directions * (100 + 9000 + 100)

      Put(4, 4, CFourPiece::CFourPieceB);
      //  1 Pieces * 6 directions * (100 + 100) + 1 direction  * (100 + 9000 + 100) + 1 direction  * (  0) +
      //  1 pieces * 4 directions * (100 + 100) + 2 directions * (100)              + 2 directions * (  0) +
      //  1 pieces * 4 directions * (100 + 100) + 1 direction  * (100 + 9000 + 100) + 2 directions * (100) +
      //-(1 pieces * 4 directions * (100 + 100) + 4 direction  * (100 +  100 + 100))
      Assert::IsTrue(pos.GetValue(true) == PositionValue(19600)); // 104 + 10 + 102 - 20

      Put(3, 4, CFourPiece::CFourPieceB);
      //  1 Pieces * ( 4 directions * (100 + 100) + 2 directions * (100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      //  1 pieces * ( 2 directions * (100 + 100) + 4 directions * (100)                                     + 2 directions * (0) ) +
      //  1 Pieces * ( 2 directions * (100 + 100) + 4 directions * (100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      // -1 pieces * ( 6 directions * (100 + 100) + 0 directions * (100 + 100 + 100) + 1 directions * (100 + 800 + 100) + 1 directions * (0) ) +
      // -1 pieces * ( 4 directions * (100 + 100) + 2 directions * (100 + 100 + 100) + 1 directions * (100 + 800 + 100) + 1 directions * (0) )
      Assert::IsTrue(pos.GetValue(true) == PositionValue(16400)); // 102 + 8 + 100 - 22 - 24

      Put(2, 4, CFourPiece::CFourPieceB);
      //  1 Pieces * ( 2 directions * (100 + 100) + 4 directions * (100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      //  1 pieces * ( 0 directions * (100 + 100) + 6 directions * (100)                                     + 2 directions * (0) ) +
      //  1 Pieces * ( 2 directions * (100 + 100) + 4 directions * (100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      // -1 pieces * ( 4 directions * (100 + 100) + 2 directions * (100 + 100 + 100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      // -1 pieces * ( 6 directions * (100 + 100) +                                  +                                     2 directions * (0) ) +
      // -1 pieces * ( 4 directions * (100 + 100) + 2 directions * (100 + 100 + 100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) )
      Assert::IsTrue(pos.GetValue(true) == PositionValue(-1800)); // 100 + 6 + 100 - 106 - 12 - 106

      Put(5, 5, CFourPiece::CFourPieceW);
      Assert::IsTrue(pos.GetValue(true) == PositionValue::Won);  // four in a row
    }

    TEST_METHOD(TestCFourPosition)
    {
      for (unsigned int z = 0; z < 10; ++z)  // make sure creating a game works multiple times
      {
        CFourGame g{ Variants<CFourGame>::GetPieces(), CFourGame::GetDimensions(7U, 6U) };
      }
    }

    TEST_METHOD(TestCFourGame)
    {
      for (unsigned int z = 0; z < 10; ++z)  // make sure creating a game works multiple times
      {
        CFourGame g{ Variants<CFourGame>::GetPieces(), CFourGame::GetDimensions(7U, 6U) };
      }
    }

  };

}
