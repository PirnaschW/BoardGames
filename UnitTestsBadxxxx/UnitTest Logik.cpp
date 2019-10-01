#include "stdafx.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BoardGamesCore;

#include "../BG/Game Logik/LogikGame.h"
using namespace Logik;

namespace UnitTestLogik
{

  TEST_CLASS(UnitTestLogik)
  {
  public:

    TEST_METHOD(TestLogikPlay)
    {
      for (PlayCode c = 0; c < Math::ipow(MaxColors, MaxPegs); c++)
      {
        Play p1{ c };                                                     // converts code to Peg list
        Assert::IsTrue(c == p1);                                          // code was stored correctly?

        std::array<unsigned int, MaxPegs> pegs{};
        for (unsigned int z = 0; z < MaxPegs; ++z)
        {
          pegs[z] = p1[z];                                                // retrieve Peg list one by one
        }

        Play p2{ pegs };                                                  // convert Peg list back into code
        Assert::IsTrue(c == p2);


        Play p3{ p1.peg };
        Assert::IsTrue(c == p3);

      }
    }


  };

}
