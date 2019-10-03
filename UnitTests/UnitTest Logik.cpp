#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BoardGamesCore;

#include "../BG/Game Logik/LogikGame.h"
using namespace Logik;

namespace UnitTestLogik
{

  TEST_CLASS(UnitTestLogik)
  {
  public:
    Plays plays;

    TEST_METHOD(TestLogikPlay)
    {
      for (PlayCode c = 0; c < Math::ipow(MaxColors, MaxPegs); ++c)
      {
//        const Play p1{ c };                                               // converts code to Play
        Assert::IsTrue(c == plays[c]);                                      // code was stored correctly?

        PlayCfg pegs{};
        for (unsigned int z = 0; z < MaxPegs; ++z)
        {
          pegs[z] = plays[c][z];                                                // retrieve Peg list one by one
        }

//        const Play p2{ pegs };                                            // convert Play back into code
        Assert::IsTrue(plays[c] == plays[pegs]);
        Assert::IsTrue(c == plays[pegs]);
      }
    }

    TEST_METHOD(TestLogikResult)
    {
      for (MarkerCount b = 0; b <= MaxPegs; ++b)
      {
        for (MarkerCount w = 0; w <= MaxPegs - b; ++w)
        {
          if (b == MaxPegs - 1 && w == 1) continue;                       // if all are black except one, can't have a white

          Result r1{ b, w };                                              // converts b, w to Result
          Assert::IsTrue(r1.GetMarker(true) == b);                        // b was stored correctly?
          Assert::IsTrue(r1.GetMarker(false) == w);                       // w was stored correctly?

          unsigned int c{ r1 };
          Assert::IsTrue(c < Result::RMax());                             // code is valid?
        }
      }

      constexpr const static PlayCode max = Math::ipow(MaxColors, MaxPegs);

      for (PlayCode c1 = 0; c1 < max/64; ++c1)
      {
        for (PlayCode c2 = 0; c2 < max/64; ++c2)
        {
          Result r{ plays[c1], plays[c2] };
        }
      }

    }



  };

}
