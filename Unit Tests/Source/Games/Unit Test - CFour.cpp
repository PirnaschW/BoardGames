#include "Unit Test - Games.h"

#include "../../../Source/Games/CFour/CFourGame.h"
using namespace CFour;

namespace UnitTestCFour
{
  constexpr Coordinate MaxX{ 8U };
  constexpr Coordinate MaxY{ 8U };

  TEST_CLASS(UnitTestCFour)
  {
  public:

    TEST_METHOD(Pieces)
    {
      Assert::IsFalse(CorePiece::WC.IsPromotable());
      Assert::IsFalse(CorePiece::WC.IsPromotable());
    }

    TEST_METHOD(Clone)
    {
      CFourPosition p1{ 0, Variants<CFour::CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0, '\0', 7U, 6U}) };
      MainPosition* pm = p1.Clone();
      Assert::IsTrue(pm != nullptr);

      CFourPosition* p2{ dynamic_cast<CFourPosition*>(pm) };
      Assert::IsTrue(p2 != nullptr);
      Assert::IsTrue(p1 == *p2);
      delete pm;
    }

    TEST_METHOD(EvaluateChain)
    {
      CFourPosition pos{ 0, Variants<CFour::CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0,'\0', 7U, 6U}) };
      pos.SetPiece(Location{ BoardPart::Stock, 0, 0 }, CorePiece::WC);
      pos.SetPiece(Location{ BoardPart::Stock, 0, 1 }, CorePiece::BC);

      auto Put = [&pos](int x, int y, const CorePiece& p)
      {
        const Location& lf_ = p == CorePiece::WC ? Location{ BoardPart::Stock, 0, 0 } : Location{ BoardPart::Stock, 0, 1 };
        const Location& lt_{ BoardPart::Main, static_cast<Coordinate>(x), static_cast<Coordinate>(y) };
        Actions a_{};
        a_.push_back(std::make_shared<ActionLift>(lf_, p));   // pick fresh Piece up
        a_.push_back(std::make_shared<ActionDrop>(lt_, p));   // and place it on target
        pos.Execute(Move{ a_ });                              // create Move and execute it
        pos.SetValue(true,pos.EvaluateChainLengths(4));
      };

      Put(2, 5, CorePiece::WC);
      Assert::IsTrue(pos.GetValue(true) == PositionValue(1800));  // 6 directions * (100 + 100) + 2 directions * (100 + 100 + 100)

      Put(3, 5, CorePiece::WC);
      Assert::IsTrue(pos.GetValue(true) == PositionValue(4400));  // 2 Pieces * 6 directions * (100 + 100) + 2 pieces * 1 directions * (100 + 800 + 100)

      Put(4, 5, CorePiece::WC);
      Assert::IsTrue(pos.GetValue(true) == PositionValue(22000));  // 3 Pieces * 6 directions * (100 + 100) + 2 pieces * 1 directions * (100 + 9000 + 100)

      Put(4, 4, CorePiece::WC);
      //  1 Pieces * 6 directions * (100 + 100) + 1 direction  * (100 + 9000 + 100) + 1 direction  * (  0) +
      //  1 pieces * 4 directions * (100 + 100) + 2 directions * (100)              + 2 directions * (  0) +
      //  1 pieces * 4 directions * (100 + 100) + 1 direction  * (100 + 9000 + 100) + 2 directions * (100) +
      //-(1 pieces * 4 directions * (100 + 100) + 4 direction  * (100 +  100 + 100))
      Assert::IsTrue(pos.GetValue(true) == PositionValue(19600)); // 104 + 10 + 102 - 20

      Put(3, 4, CorePiece::BC);
      //  1 Pieces * ( 4 directions * (100 + 100) + 2 directions * (100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      //  1 pieces * ( 2 directions * (100 + 100) + 4 directions * (100)                                     + 2 directions * (0) ) +
      //  1 Pieces * ( 2 directions * (100 + 100) + 4 directions * (100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      // -1 pieces * ( 6 directions * (100 + 100) + 0 directions * (100 + 100 + 100) + 1 directions * (100 + 800 + 100) + 1 directions * (0) ) +
      // -1 pieces * ( 4 directions * (100 + 100) + 2 directions * (100 + 100 + 100) + 1 directions * (100 + 800 + 100) + 1 directions * (0) )
      Assert::IsTrue(pos.GetValue(true) == PositionValue(16400)); // 102 + 8 + 100 - 22 - 24

      Put(2, 4, CorePiece::BC);
      //  1 Pieces * ( 2 directions * (100 + 100) + 4 directions * (100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      //  1 pieces * ( 0 directions * (100 + 100) + 6 directions * (100)                                     + 2 directions * (0) ) +
      //  1 Pieces * ( 2 directions * (100 + 100) + 4 directions * (100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      // -1 pieces * ( 4 directions * (100 + 100) + 2 directions * (100 + 100 + 100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) ) +
      // -1 pieces * ( 6 directions * (100 + 100) +                                  +                                     2 directions * (0) ) +
      // -1 pieces * ( 4 directions * (100 + 100) + 2 directions * (100 + 100 + 100) + 1 directions * (100 + 9000 + 100) + 1 directions * (0) )
      Assert::IsTrue(pos.GetValue(true) == PositionValue(-1800)); // 100 + 6 + 100 - 106 - 12 - 106

      Put(5, 5, CorePiece::WC);
      Assert::IsTrue(pos.GetValue(true) == PositionValue::PValueType::Won);  // four in a row
    }

    class TestCFourPosition : public CFourPosition
    {
    public:
      TestCFourPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept : CFourPosition(c, p, d) {}
      virtual MainPosition* Clone(void) const noexcept override { return new TestCFourPosition(*this); }
      void  Eval4inaRow(void)
      {
        using P = PositionValue::PValueType;

        auto Put = [this](int x, int y, const CorePiece& p)
        {
          const Location& lf_ = p == CorePiece::WC ? Location{ BoardPart::Stock, 0, 0 } : Location{ BoardPart::Stock, 0, 1 };
          const Location& lt_{ BoardPart::Main, static_cast<Coordinate>(x), static_cast<Coordinate>(y) };
          Actions a_{};
          a_.push_back(std::make_shared<ActionLift>(lf_, p));   // pick fresh Piece up
          a_.push_back(std::make_shared<ActionDrop>(lt_, p));   // and place it on target
          this->Execute(Move{ a_ });                            // create Move and execute it
          this->SetValue(true, this->EvaluateStatically());
        };

        Put(3, 5, CorePiece::WC);
        Put(3, 4, CorePiece::BC);
        Put(2, 5, CorePiece::WC);
        Put(2, 4, CorePiece::BC);
        Put(4, 5, CorePiece::WC);
        Put(4, 4, CorePiece::BC);
        Put(1, 5, CorePiece::WC);
        Assert::IsTrue(GetValue(true) == P::Won);
      }

      void BruteForceMove1(void)
      {
        constexpr static PositionValue val1[7]     { 1400, 1800 , 1800 , 1800 , 1800 , 1800 , 1400 };
        constexpr static PositionValue val2[7][7]{ { -400, -400 , -400 , -400 , -400 , -400 ,    0 },
                                                   {  400, -600 ,    0 ,    0 ,    0 ,    0 ,  400 },
                                                   {  400,    0 , -600 ,    0 ,    0 ,    0 ,  400 },
                                                   {  400,    0 ,    0 , -600 ,    0 ,    0 ,  400 },
                                                   {  400,    0 ,    0 ,    0 , -600 ,    0 ,  400 },
                                                   {  400,    0 ,    0 , -  0 ,    0 , -600 ,  400 },
                                                   {    0,  -400, -400 , -400 , -400 , -400 , -400 } };

        SetValue(true, EvaluateStatically());
        Assert::IsTrue(movesW_.size() == 7);

        for (int i1 = 0; i1 < 7; ++i1)
        {
          MainPosition* p1 = Clone();
          p1->Execute(*movesW_[i1]);
          p1->SetValue(true, p1->EvaluateStatically());
          Assert::IsTrue(p1->GetValue(true) == PositionValue(val1[i1]));

          for (int i2 = 0; i2 < 7; ++i2)
          {
            MainPosition* p2 = p1->Clone();
            p2->Execute(*(p1->GetMoveList(false)[i2]));
            p2->SetValue(true, p2->EvaluateStatically());

            wchar_t buffer[256];
            wsprintfW(buffer, L"%3d %3d: %10d\n", i1, i2, (int)p2->GetValue(true));
            OutputDebugString(buffer);
            Assert::IsTrue(p2->GetValue(true) == PositionValue(val2[i1][i2]));
            delete p2;
          }
          delete p1;
        }

        AIContext plist{};
        PositionValue valAB1 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 1);
        Assert::IsTrue(valAB1 == PositionValue(1800));

        PositionValue valAB2 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 2);
        Assert::IsTrue(valAB2 == PositionValue(-400));
      }
      void  BruteForceWinIn2(void)
      {
        using P = PositionValue::PValueType;
        constexpr static PositionValue val1[7]     {  23400, P::Won,  26000,  26800,  26000, P::Won,  23400 };
        constexpr static PositionValue val2[7][7]{ {  21600,  21600,  21000,  21000,  21000,  21600,  22000  },
                                                   { P::Won, P::Won, P::Won, P::Won, P::Won, P::Won, P::Won  },
                                                   {  24600,  24200,  23600,  23600,  23600,  24200,  24600  },
                                                   {  25400,  25000,  24400,  24400,  24400,  25000,  25400  },
                                                   {  24600,  24200,  23600,  23600,  23600,  24200,  24600  },
                                                   { P::Won, P::Won, P::Won, P::Won, P::Won, P::Won, P::Won  },
                                                   {  22000,  21600,  21000,  21000,  21000,  21600,  21600  } };

        auto Put = [this](int x, int y, const CorePiece& p)
        {
          const Location& lf_ = p == CorePiece::WC ? Location{ BoardPart::Stock, 0, 0 } : Location{ BoardPart::Stock, 0, 1 };
          const Location& lt_{ BoardPart::Main, static_cast<Coordinate>(x), static_cast<Coordinate>(y) };
          Actions a_{};
          a_.push_back(std::make_shared<ActionLift>(lf_, p));   // pick fresh Piece up
          a_.push_back(std::make_shared<ActionDrop>(lt_, p));   // and place it on target
          this->Execute(Move{ a_ });                              // create Move and execute it
          this->SetValue(true, this->EvaluateStatically());
        };

        Put(2, 5, CorePiece::WC);
        Assert::IsTrue(GetValue(true) == PositionValue(1800));  // 6 directions * (100 + 100) + 2 directions * (100 + 100 + 100)

        Put(3, 5, CorePiece::WC);
        Assert::IsTrue(GetValue(true) == PositionValue(4400));  // 2 Pieces * 6 directions * (100 + 100) + 2 pieces * 1 directions * (100 + 800 + 100)

        Put(4, 5, CorePiece::WC);
        Assert::IsTrue(GetValue(true) == PositionValue(22000));  // 3 Pieces * 6 directions * (100 + 100) + 2 pieces * 1 directions * (100 + 9000 + 100)

        Assert::IsTrue(movesW_.size() == 7);

        wchar_t buffer[256];
        for (int i1 = 0; i1 < 7; ++i1)
        {
          MainPosition* p1 = Clone();
          p1->Execute(*movesW_[i1]);
          p1->SetValue(true, p1->EvaluateStatically());
          wsprintfW(buffer, L"%3d: %10.10S\n", i1, (const char*)p1->GetValue(true));
          OutputDebugString(buffer);
          Assert::IsTrue(p1->GetValue(true) == val1[i1]);

          for (int i2 = 0; i2 < 7; ++i2)
          {
            MainPosition* p2 = p1->Clone();
            p2->Execute(*(p1->GetMoveList(false)[i2]));
            p2->SetValue(true, p2->EvaluateStatically());

            wsprintfW(buffer, L"%3d %3d: %10.10S\n", i1, i2, (const char*)p2->GetValue(true));
            OutputDebugString(buffer);
            Assert::IsTrue(p2->GetValue(true) == val2[i1][i2]);
            delete p2;
          }
          delete p1;
        }

        AIContext plist{};
        PositionValue valAB1 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 1);
        Assert::IsTrue(valAB1 == PositionValue::PValueType::Won);

        PositionValue valAB2 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 2);
        Assert::IsTrue(valAB2 == PositionValue::PValueType::Won);
      }
      void BruteForceWinIn3(void)
      {
        using P = PositionValue::PValueType;
        constexpr static PositionValue val1[7]      {  5800,  22000,   8400,   8400,  22000,   6200,   5800 };
        constexpr static PositionValue val2[7][7]{ {   4000,   4000,   3400,   3400,   4000,   4000,   4400  },
                                                   {  20600,  19600,  19600,  19600,  20200,  20200,  20600  },
                                                   {   7000,   6600,   6000,   6000,   6600,   6600,   7000  },
                                                   {   7000,   6600,   6000,   6000,   6600,   6600,   7000  },
                                                   {  20600,  20200,  19600,  19600,  19600,  20200,  20600  },
                                                   {   4800,   4400,   3800,   3800,   4400,   3800,   4800  },
                                                   {   4400,   4000,   3400,   3400,   4000,   4000,   4000  } };

        auto Put = [this](int x, int y, const CorePiece& p)
        {
          const Location& lf_ = p == CorePiece::WC ? Location{ BoardPart::Stock, 0, 0 } : Location{ BoardPart::Stock, 0, 1 };
          const Location& lt_{ BoardPart::Main, static_cast<Coordinate>(x), static_cast<Coordinate>(y) };
          Actions a_{};
          a_.push_back(std::make_shared<ActionLift>(lf_, p));   // pick fresh Piece up
          a_.push_back(std::make_shared<ActionDrop>(lt_, p));   // and place it on target
          this->Execute(Move{ a_ });                              // create Move and execute it
          this->SetValue(true, this->EvaluateStatically());
        };

        Put(2, 5, CorePiece::WC);
        Assert::IsTrue(GetValue(true) == PositionValue(1800));  // 6 directions * (100 + 100) + 2 directions * (100 + 100 + 100)

        Put(3, 5, CorePiece::WC);
        Assert::IsTrue(GetValue(true) == PositionValue(4400));  // 2 Pieces * 6 directions * (100 + 100) + 2 pieces * 1 directions * (100 + 800 + 100)

        Assert::IsTrue(movesW_.size() == 7);

        wchar_t buffer[256];
        for (int i1 = 0; i1 < 7; ++i1)
        {
          MainPosition* p1 = Clone();
          p1->Execute(*movesW_[i1]);
          p1->SetValue(true, p1->EvaluateStatically());
          wsprintfW(buffer, L"%3d: %10.10S\n", i1, (const char*)p1->GetValue(true));
          OutputDebugString(buffer);
          Assert::IsTrue(p1->GetValue(true) == val1[i1]);

          for (int i2 = 0; i2 < 7; ++i2)
          {
            MainPosition* p2 = p1->Clone();
            p2->Execute(*(p1->GetMoveList(false)[i2]));
            p2->SetValue(true, p2->EvaluateStatically());

            wsprintfW(buffer, L"%3d %3d: %10.10S\n", i1, i2, (const char*)p2->GetValue(true));
            OutputDebugString(buffer);
            Assert::IsTrue(p2->GetValue(true) == val2[i1][i2]);
            delete p2;
          }
          delete p1;
        }

        AIContext plist{};
        PositionValue valAB1 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 1);
        Assert::IsTrue(valAB1 == PositionValue(22000));

        PositionValue valAB2 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 2);
        Assert::IsTrue(valAB2 == PositionValue(19600));

        PositionValue valAB3 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 3);
        Assert::IsTrue(valAB3 == P::Won);
      }

      void BruteForceNoWinIn3(void)
      {
        using P = PositionValue::PValueType;
        constexpr static PositionValue val1[7]      {  3200,   4400,   5000,   4400,   3600,   3600,   3200 };
        constexpr static PositionValue val2[7][7]{ {   1400,   1400,    800,   1400,   1400,   1400,   1800  },
                                                   {   3000,   2000,   2000,   2600,   2600,   2600,   3000  },
                                                   {   3600,   3200,   2600,   3200,   3200,   3200,   3600  },
                                                   {   3000,   2600,   2000,   2000,   2600,   2600,   3000  },
                                                   {   2200,   1800,   1200,   1800,   1200,   1800,   2200  },
                                                   {   2200,   1800,   1200,   1800,   1800,   1200,   2200  },
                                                   {   1800,   1400,    800,   1400,   1400,   1400,   1400  } };

        auto Put = [this](int x, int y, const CorePiece& p)
        {
          const Location& lf_ = p == CorePiece::WC ? Location{ BoardPart::Stock, 0, 0 } : Location{ BoardPart::Stock, 0, 1 };
          const Location& lt_{ BoardPart::Main, static_cast<Coordinate>(x), static_cast<Coordinate>(y) };
          Actions a_{};
          a_.push_back(std::make_shared<ActionLift>(lf_, p));   // pick fresh Piece up
          a_.push_back(std::make_shared<ActionDrop>(lt_, p));   // and place it on target
          this->Execute(Move{ a_ });                              // create Move and execute it
          this->SetValue(true, this->EvaluateStatically());
        };

        Put(2, 5, CorePiece::WC);
        Assert::IsTrue(GetValue(true) == PositionValue(1800));  // 6 directions * (100 + 100) + 2 directions * (100 + 100 + 100)

        Assert::IsTrue(movesW_.size() == 7);

        wchar_t buffer[256];
        for (int i1 = 0; i1 < 7; ++i1)
        {
          MainPosition* p1 = Clone();
          p1->Execute(*movesW_[i1]);
          p1->SetValue(true, p1->EvaluateStatically());
          wsprintfW(buffer, L"%3d: %10.10S\n", i1, (const char*)p1->GetValue(true));
          OutputDebugString(buffer);
          Assert::IsTrue(p1->GetValue(true) == val1[i1]);

          for (int i2 = 0; i2 < 7; ++i2)
          {
            MainPosition* p2 = p1->Clone();
            p2->Execute(*(p1->GetMoveList(false)[i2]));
            p2->SetValue(true, p2->EvaluateStatically());

            wsprintfW(buffer, L"%3d %3d: %10.10S\n", i1, i2, (const char*)p2->GetValue(true));
            OutputDebugString(buffer);
            Assert::IsTrue(p2->GetValue(true) == val2[i1][i2]);
            delete p2;
          }
          delete p1;
        }

        {
          AIContext plist{};
          movesW_.clear();
          movesB_.clear();
          SetDepth(0);
          SetValue(true, EvaluateStatically());
          PositionValue valAB1 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 1);
          Assert::IsTrue(valAB1 == PositionValue(5000));
        }
        {
          AIContext plist{};
          movesW_.clear();
          movesB_.clear();
          SetDepth(0);
          SetValue(true, EvaluateStatically());
          PositionValue valAB2 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 2);
          Assert::IsTrue(valAB2 == PositionValue(2600));
        }
        {
          AIContext plist{};
          movesW_.clear();
          movesB_.clear();
          SetDepth(0);
          SetValue(true, EvaluateStatically());
          PositionValue valAB3 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 3);
          Assert::IsTrue(valAB3 == PositionValue(19600));
        }
        {
          AIContext plist{};
          movesW_.clear();
          movesB_.clear();
          SetDepth(0);
          SetValue(true, EvaluateStatically());
          PositionValue valAB4 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 4);
          Assert::IsTrue(valAB4 == PositionValue(16400));
        }
      }
      void BruteForceWinIn33(void)
      {
        using P = PositionValue::PValueType;
        constexpr static PositionValue val1[7]      {  3200,   4400,   5000,   4400,   3600,   3600,   3200 };
        constexpr static PositionValue val2[7][7]{ {   1400,   1400,    800,   1400,   1400,   1400,   1800  },
                                                   {   3000,   2000,   2000,   2600,   2600,   2600,   3000  },
                                                   {   3600,   3200,   2600,   3200,   3200,   3200,   3600  },
                                                   {   3000,   2600,   2000,   2000,   2600,   2600,   3000  },
                                                   {   2200,   1800,   1200,   1800,   1200,   1800,   2200  },
                                                   {   2200,   1800,   1200,   1800,   1800,   1200,   2200  },
                                                   {   1800,   1400,    800,   1400,   1400,   1400,   1400  } };

        auto Put = [this](int x, int y, const CorePiece& p)
        {
          const Location& lf_ = p == CorePiece::WC ? Location{ BoardPart::Stock, 0, 0 } : Location{ BoardPart::Stock, 0, 1 };
          const Location& lt_{ BoardPart::Main, static_cast<Coordinate>(x), static_cast<Coordinate>(y) };
          Actions a_{};
          a_.push_back(std::make_shared<ActionLift>(lf_, p));   // pick fresh Piece up
          a_.push_back(std::make_shared<ActionDrop>(lt_, p));   // and place it on target
          this->Execute(Move{ a_ });                              // create Move and execute it
          this->SetValue(true, this->EvaluateStatically());
        };

        Put(2, 5, CorePiece::WC);
        Assert::IsTrue(GetValue(true) == PositionValue(1800));  // 6 directions * (100 + 100) + 2 directions * (100 + 100 + 100)

        Assert::IsTrue(movesW_.size() == 7);

        wchar_t buffer[256];
        for (int i1 = 0; i1 < 7; ++i1)
        {
          MainPosition* p1 = Clone();
          p1->Execute(*movesW_[i1]);
          p1->SetValue(true, p1->EvaluateStatically());
          wsprintfW(buffer, L"%3d: %10.10S\n", i1, (const char*)p1->GetValue(true));
          OutputDebugString(buffer);
          Assert::IsTrue(p1->GetValue(true) == val1[i1]);

          for (int i2 = 0; i2 < 7; ++i2)
          {
            MainPosition* p2 = p1->Clone();
            p2->Execute(*(p1->GetMoveList(false)[i2]));
            p2->SetValue(true, p2->EvaluateStatically());

            wsprintfW(buffer, L"%3d %3d: %10.10S\n", i1, i2, (const char*)p2->GetValue(true));
            OutputDebugString(buffer);
            Assert::IsTrue(p2->GetValue(true) == val2[i1][i2]);
            delete p2;
          }
          delete p1;
        }

        {
          AIContext plist{};
          movesW_.clear();
          movesB_.clear();
          SetDepth(0);
          SetValue(true, EvaluateStatically());
          PositionValue valAB1 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 1);
          Assert::IsTrue(valAB1 == PositionValue(5000));
        }
        {
          AIContext plist{};
          movesW_.clear();
          movesB_.clear();
          SetDepth(0);
          SetValue(true, EvaluateStatically());
          PositionValue valAB2 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 2);
          Assert::IsTrue(valAB2 == PositionValue(2600));
        }
        {
          AIContext plist{};
          movesW_.clear();
          movesB_.clear();
          SetDepth(0);
          SetValue(true, EvaluateStatically());
          PositionValue valAB3 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 3);
          Assert::IsTrue(valAB3 == PositionValue(19600));
        }
        {
          AIContext plist{};
          movesW_.clear();
          movesB_.clear();
          SetDepth(0);
          SetValue(true, EvaluateStatically());
          PositionValue valAB4 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 4);
          Assert::IsTrue(valAB4 == P::Won);
        }
      }
      void BruteForceWinIn1(void)
      {
        using P = PositionValue::PValueType;
        constexpr static PositionValue val1[7]{       17800, P::Won,  20400,  18800,  18800, P::Won,  17800 };
        constexpr static PositionValue val2[7][7]{ {   1400,   1400,    800,   1400,   1400,   1400,   1800  },
                                                   {   3000,   2000,   2000,   2600,   2600,   2600,   3000  },
                                                   {   3600,   3200,   2600,   3200,   3200,   3200,   3600  },
                                                   {   3000,   2600,   2000,   2000,   2600,   2600,   3000  },
                                                   {   2200,   1800,   1200,   1800,   1200,   1800,   2200  },
                                                   {   2200,   1800,   1200,   1800,   1800,   1200,   2200  },
                                                   {   1800,   1400,    800,   1400,   1400,   1400,   1400  } };

        auto Put = [this](int x, int y, const CorePiece& p)
        {
          const Location& lf_ = p == CorePiece::WC ? Location{ BoardPart::Stock, 0, 0 } : Location{ BoardPart::Stock, 0, 1 };
          const Location& lt_{ BoardPart::Main, static_cast<Coordinate>(x), static_cast<Coordinate>(y) };
          Actions a_{};
          a_.push_back(std::make_shared<ActionLift>(lf_, p));   // pick fresh Piece up
          a_.push_back(std::make_shared<ActionDrop>(lt_, p));   // and place it on target
          this->Execute(Move{ a_ });                              // create Move and execute it
          this->SetValue(true, this->EvaluateStatically());
        };

        Put(2, 5, CorePiece::WC);
        Put(3, 5, CorePiece::WC);
        Put(4, 5, CorePiece::WC);
        Put(3, 4, CorePiece::BC);
        Put(4, 4, CorePiece::BC);

        Assert::IsTrue(movesW_.size() == 7);

        wchar_t buffer[256];
        for (int i1 = 0; i1 < 7; ++i1)
        {
          MainPosition* p1 = Clone();
          p1->Execute(*movesW_[i1]);
          p1->SetValue(true, p1->EvaluateStatically());
          wsprintfW(buffer, L"%3d: %10.10S\n", i1, (const char*)p1->GetValue(true));
          OutputDebugString(buffer);
          Assert::IsTrue(p1->GetValue(true) == val1[i1]);

          //for (int i2 = 0; i2 < 7; ++i2)
          //{
          //  MainPosition* p2 = p1->Clone();
          //  p2->Execute(*(p1->GetMoveList(false)[i2]));
          //  p2->SetValue(true, p2->EvaluateStatically());

          //  wsprintfW(buffer, L"%3d %3d: %10.10S\n", i1, i2, (const char*)p2->GetValue(true));
          //  OutputDebugString(buffer);
          //  Assert::IsTrue(p2->GetValue(true) == val2[i1][i2]);
          //  delete p2;
          //}
          delete p1;
        }

        {
          AIContext plist{};
          movesW_.clear();
          movesB_.clear();
          SetDepth(0);
          SetValue(true, EvaluateStatically());
          PositionValue valAB1 = Evaluate(plist, true, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 2);
          Assert::IsTrue(valAB1 == P::Won);
        }
      }

    };

    TEST_METHOD(Eval4inaRow)
    {
      TestCFourPosition pos{ 0, Variants<CFour::CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0, '\0',7U, 6U}) };
      pos.SetPiece(Location{ BoardPart::Stock, 0, 0 }, CorePiece::WC);
      pos.SetPiece(Location{ BoardPart::Stock, 0, 1 }, CorePiece::BC);

      pos.Eval4inaRow();
    }

    TEST_METHOD(BruteForceMove0)
    {
      TestCFourPosition pos{ 0, Variants<CFour::CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0, '\0', 7U, 6U}) };
      pos.SetPiece(Location{ BoardPart::Stock, 0, 0 }, CorePiece::WC);
      pos.SetPiece(Location{ BoardPart::Stock, 0, 1 }, CorePiece::BC);

      auto Put = [&pos](int x, int y, const CorePiece& p)
      {
        const Location& lf_ = p == CorePiece::WC ? Location{ BoardPart::Stock, 0, 0 } : Location{ BoardPart::Stock, 0, 1 };
        const Location& lt_{ BoardPart::Main, static_cast<Coordinate>(x), static_cast<Coordinate>(y) };
        Actions a_{};
        a_.push_back(std::make_shared<ActionLift>(lf_, p));   // pick fresh Piece up
        a_.push_back(std::make_shared<ActionDrop>(lt_, p));   // and place it on target
        pos.Execute(Move{ a_ });                              // create Move and execute it
        pos.SetValue(true, pos.EvaluateStatically());
      };

      Put(0, 5, CorePiece::WC);
      Assert::IsTrue(pos.GetValue(true) == PositionValue(1400));  // 6 directions * (100 + 100) + 2 directions * (100 + 100 + 100)

      AIContext plist{};
      PositionValue valAB1 = pos.Evaluate(plist, false, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 1);
      Assert::IsTrue(valAB1 == PositionValue(400));
    }

    TEST_METHOD(BruteForceMove1)
    {
      TestCFourPosition pos{ 0, Variants<CFour::CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0, '\0', 7U, 6U}) };
      pos.SetPiece(Location{ BoardPart::Stock, 0, 0 }, CorePiece::WC);
      pos.SetPiece(Location{ BoardPart::Stock, 0, 1 }, CorePiece::BC);

      pos.BruteForceMove1();
    }

    TEST_METHOD(BruteForceWinIn2)
    {
      TestCFourPosition pos{ 0, Variants<CFour::CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0, '\0', 7U, 6U}) };
      pos.SetPiece(Location{ BoardPart::Stock, 0, 0 }, CorePiece::WC);
      pos.SetPiece(Location{ BoardPart::Stock, 0, 1 }, CorePiece::BC);

      pos.BruteForceWinIn2();
    }

    TEST_METHOD(BruteForceWinIn3)
    {
      TestCFourPosition pos{ 0, Variants<CFour::CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0, '\0', 7U, 6U}) };
      pos.SetPiece(Location{ BoardPart::Stock, 0, 0 }, CorePiece::WC);
      pos.SetPiece(Location{ BoardPart::Stock, 0, 1 }, CorePiece::BC);

      pos.BruteForceWinIn3();
    }

    TEST_METHOD(BruteForceNoWinIn3)
    {
      TestCFourPosition pos{ 0, Variants<CFour::CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0, '\0', 7U, 6U}) };
      pos.SetPiece(Location{ BoardPart::Stock, 0, 0 }, CorePiece::WC);
      pos.SetPiece(Location{ BoardPart::Stock, 0, 1 }, CorePiece::BC);

      pos.BruteForceNoWinIn3();
    }

    TEST_METHOD(BruteForceWinIn1)
    {
      TestCFourPosition pos{ 0, Variants<CFour::CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0, '\0', 7U, 6U}) };
      pos.SetPiece(Location{ BoardPart::Stock, 0, 0 }, CorePiece::WC);
      pos.SetPiece(Location{ BoardPart::Stock, 0, 1 }, CorePiece::BC);

      pos.BruteForceWinIn1();
    }

    TEST_METHOD(Game)
    {
      for (unsigned int z = 0; z < 10; ++z)  // make sure creating a game works multiple times
      {
        CFourGame g{ 0, Variants<CFourGame>::GetPieces(0), CFourGame::GetDimensions(VariantChosen{0, '\0', 7U, 6U}) };
      }
    }

  };

}
