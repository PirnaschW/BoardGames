#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BoardGamesCore;

#include "../BG/Game Checkers/CheckersGame.h"
using namespace Checkers;

namespace UnitTestCheckers
{

  TEST_CLASS(UnitTestCheckers)
  {
  public:

    TEST_METHOD(TestCheckersPlay)
    {
      Assert::IsTrue(1 == 1);
    }

    TEST_METHOD(TestCheckersPositionClone)
    {
      CheckersPosition l1{ Variants<Checkers::CheckersGame>::GetPieces(), CheckersGame::GetDimensions(8U, 8U) };
      MainPosition* p2 = l1.Clone();
      Assert::IsTrue(p2 != nullptr);

      CheckersPosition* pl2{ dynamic_cast<CheckersPosition*>(p2) };
      Assert::IsTrue(pl2 != nullptr);
    }

    TEST_METHOD(TestCheckersGame)
    {
      for (unsigned int z = 0; z < 10; ++z)  // make sure creating a game works multiple times
      {
        CheckersGame g{ Variants<CheckersGame>::GetPieces(), CheckersGame::GetDimensions(8U, 8U) };
      }
    }

  };

}
