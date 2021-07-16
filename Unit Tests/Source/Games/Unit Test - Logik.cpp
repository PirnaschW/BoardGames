#include "Unit Test - Games.h"

#include "../../../Source/Games/Logik/LogikGame.h"
using namespace Logik;

namespace UnitTestLogik
{

  TEST_CLASS(UnitTestLogik)
  {
  public:
    Plays plays_;

    TEST_METHOD(TestLogikPlay)
    {
      for (PlayCode c = 0; c < Plays::Max; ++c)
      {
        Assert::IsTrue(c == plays_[c]);                                   // code was stored correctly?

        PlayCfg pegs{};
        for (unsigned int z = 0; z < MaxPegs; ++z)
        {
          pegs[z] = plays_[c][z];                                         // retrieve Peg list one by one
        }

        Assert::IsTrue(plays_[c] == plays_[pegs]);
        Assert::IsTrue(c == plays_[pegs]);
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

      for (PlayCode c1 = 0; c1 < Plays::Max /64; ++c1)
      {
        for (PlayCode c2 = 0; c2 < Plays::Max; c2+=511)
        {
          Result r1{ plays_[c1], plays_[c2] };
          Result r2{ plays_[c2], plays_[c1] };
          Assert::IsTrue(r1 == r2);
        }
      }

    }

    TEST_METHOD(TestLogikLayout)
    {
      for (unsigned int z = 0; z < 10; ++z)  // make sure creating a layout works multiple times
      {
        LogikLayout l{ LogikGame::GetDimensions(VariantChosen{0, '\0', 5U, 7U}) };
      }
    }
    
    TEST_METHOD(TestLogikPositionClone)
    {
      const VariantChosen v{ 0,'\0', MaxPegs, MaxTries };
      LogikPosition l1{ 0, Variants<Logik::LogikGame>::GetPieces(v), LogikGame::GetDimensions(v) };
      MainPosition* p2 = l1.Clone();
      Assert::IsTrue(p2 != nullptr);

      LogikPosition* pl2{ dynamic_cast<LogikPosition*>(p2) };
      Assert::IsTrue(pl2 != nullptr);
      delete p2;
    }

    TEST_METHOD(TestLogikPositionPegsMarkers)
    {
      const VariantChosen v{ 0,'\0', MaxPegs, MaxTries };
      LogikPosition l1{ 0, Variants<Logik::LogikGame>::GetPieces(v), LogikGame::GetDimensions(v) };
      MainPosition* pm = &l1;
      Assert::IsTrue(pm != nullptr);

      LogikPosition* pl1{ dynamic_cast<LogikPosition*>(pm) };
      Assert::IsTrue(pl1 != nullptr);

      bool set{ false };
      std::array<const LogikPiece*, MaxPegs * MaxTries> p{ nullptr };
      std::array<const LogikPiece*, MaxPegs * MaxTries> m{ nullptr };

      for (unsigned int i = 0; i < MaxPegs * MaxTries - 1; ++i)
      {
        p[i] = &LogikPiece::GetPiece(rand() % MaxColors);
        set = pl1->SetFirstFreePeg(*p[i]);
        Assert::IsTrue(set);
        m[i] = rand() % 2 ? &LogikPiece::LPieceB : &LogikPiece::LPieceW;
        set = pl1->SetFirstFreeMarker(*m[i]);
        Assert::IsTrue(set);
      }

      pl1->ReadPosition();

      // verify that 'Peg full' is recognized correctly
      set = pl1->SetFirstFreePeg(LogikPiece::GetPiece(rand() % MaxColors));
      Assert::IsTrue(set);
      set = pl1->SetFirstFreePeg(LogikPiece::GetPiece(rand() % MaxColors));
      Assert::IsFalse(set);
      // verify that 'Marker full' is recognized correctly
      set = pl1->SetFirstFreeMarker(rand() % 2 ? LogikPiece::LPieceB : LogikPiece::LPieceW);
      Assert::IsTrue(set);
      set = pl1->SetFirstFreeMarker(rand() % 2 ? LogikPiece::LPieceB : LogikPiece::LPieceW);
      Assert::IsFalse(set);

      for (unsigned char i = 0; i < MaxPegs * MaxTries - 1; ++i)
      {
        const unsigned char i21 = i % MaxPegs;
        const unsigned char i31 = i / MaxPegs;
        const unsigned char i22 = i21 + MaxPegs;
        // verify Pegs come back as set
        const Piece& pp2 = pl1->GetPiece(Location{ BoardPart::Main, i22, i31 });
        Assert::IsTrue(pp2 == *p[i]);
        // verify Markers come back as set
        const Piece& pm2 = pl1->GetPiece(Location{ BoardPart::Main, i21, i31 });
        Assert::IsTrue(pm2 == *m[i]);
      }
    }


    TEST_METHOD(TestLogikPositionExecute)
    {
      std::unique_ptr<const Plays> plays = std::make_unique<const Plays>();
      for (unsigned int z = 0; z < 10; ++z)  // make sure creating a position works multiple times
      {
        const VariantChosen v{ 0,'\0', MaxPegs, MaxTries };
        LogikPosition l{ 0, Variants<Logik::LogikGame>::GetPieces(v), LogikGame::GetDimensions(v) };
        for (unsigned char j = 0; j < MaxTries; ++j)
        {
          PlayCode c = rand() % Plays::Max;
          l.Execute(c);

          for (unsigned char i = MaxPegs; i < 2 * MaxPegs; ++i)
          {
            // verify Pegs come back as set
            const Piece& pp = l.GetPiece(Location{ BoardPart::Main, i, j });
            Assert::IsTrue(pp == LogikPiece::GetPiece((*plays)[c][i - MaxPegs]));
          }
        }
      }
    }

    TEST_METHOD(TestLogikGame)
    {
      for (unsigned int z = 0; z < 10; ++z)  // make sure creating a game works multiple times
      {
        const VariantChosen v{ 0,'\0', 5, 7 };
        LogikGame g{ v, Variants<Logik::LogikGame>::GetPieces(v), Logik::LogikGame::GetDimensions(v) };

        bool set{ false };
        std::array<unsigned char, MaxPegs * MaxTries> p{};
        std::array<unsigned char, MaxPegs * MaxTries> m{};

        for (unsigned char i = 0; i < MaxPegs * MaxTries; ++i)
        {
          p[i] = '0' + rand() % MaxColors;
          m[i] = rand() % 2 ? 'B' : 'W';
          g.React(m[i], 0U, 0U);
          g.React(p[i], 0U, 0U);
        }
      }
    }

  };

}
