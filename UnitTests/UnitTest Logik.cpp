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

    TEST_METHOD(TestLogikPlay)
    {
      for (PlayCode c = 0; c < Math::ipow(MaxColors, MaxPegs); ++c)
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
      }
    }


    TEST_METHOD(TestLogikResult)
    {
      for (unsigned int b = 0; b < MaxPegs; ++b)
      {
        for (unsigned int w = 0; w < MaxPegs-b; ++w)
        {
          if (b == MaxPegs - 1 && w == 1) continue;                      // if all are black except one, can't have a white

          Result r1{ b, w };                                             // converts b, w to Result
          Assert::IsTrue(r1.GetMarker(true) == b);                       // b was stored correctly?
          Assert::IsTrue(r1.GetMarker(false) == w);                      // w was stored correctly?

          ResultCode c{ r1 };
          Assert::IsTrue(c < Result::RN());                              // code is valid?
        }
      }
    }



  };

}
