#include "stdafx.h"

#include "TestResource.h"
#include "TestGame.h"


namespace Test
{
  TestPosition::TestPosition(unsigned int x, unsigned int y) : MainPosition(x, y)
  {
    if (x == 4 and y == 4)
    {
      SetPiece(Location(0, 0), &ChessPiece::WQ);
      SetPiece(Location(0, 1), &ChessPiece::BQ);
      SetPiece(Location(0, 2), &ChessPiece::WR);
      SetPiece(Location(0, 3), &ChessPiece::BR);
      SetPiece(Location(1, 0), &ChessPiece::WB);
      SetPiece(Location(1, 1), &ChessPiece::BB);
      SetPiece(Location(1, 2), &ChessPiece::WN);
      SetPiece(Location(1, 3), &ChessPiece::BN);
      SetPiece(Location(2, 0), &ChessPiece::WQ);
      SetPiece(Location(2, 1), &ChessPiece::BQ);
      SetPiece(Location(2, 2), &ChessPiece::WR);
      SetPiece(Location(2, 3), &ChessPiece::BR);
      SetPiece(Location(3, 0), &ChessPiece::WB);
      SetPiece(Location(3, 1), &ChessPiece::BB);
      SetPiece(Location(3, 2), &ChessPiece::WN);
      SetPiece(Location(3, 3), &ChessPiece::BN);
    }
    else if (x == 3 and y == 3)
    {
      SetPiece(Location(0, 0), &ChessPiece::WQ);
      SetPiece(Location(0, 1), &ChessPiece::BQ);
      SetPiece(Location(0, 2), &ChessPiece::WR);
      SetPiece(Location(1, 0), &ChessPiece::BR);
      SetPiece(Location(1, 1), &ChessPiece::WB);
      SetPiece(Location(1, 2), &ChessPiece::BB);
      SetPiece(Location(2, 0), &ChessPiece::WN);
      SetPiece(Location(2, 1), &ChessPiece::BN);
      SetPiece(Location(2, 2), &ChessPiece::WQ);
    }
    else if (x == 2 and y == 2)
    {
      SetPiece(Location(0, 0), &ChessPiece::WQ);
      SetPiece(Location(0, 1), &ChessPiece::BQ);
      SetPiece(Location(1, 0), &ChessPiece::BB);
      SetPiece(Location(1, 1), &ChessPiece::WR);
    }

  }

  bool TestPosition::AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const
  {
    const Piece* p = GetPiece(to);
    if (p == nullptr) return false;  // out of board
    if (p->IsBlank()) return true;   // not a move, but keep trying this direction
    if (p->IsColor(OnTurn())) return false;  // own piece

    m.push_back(Step{ Field{fr,GetPiece(fr)}, Field{to,p},Step::StepType::Take, std::vector<Field>{Field{to,p}} });
    return false;
  };


  TestGame::TestGame(unsigned int x, unsigned int y) : TestGame(
    new TestPosition(x, y), new TakenPosition(x*y / 2, 2), new StockPosition(5, 2),
    new TestLayout(x, y), new TestTakenLayout(x, y), new TestStockLayout(x, y)) {}

  TestGame::TestGame(TestPosition* p, TakenPosition* t, StockPosition* s,
    TestLayout* l, TestTakenLayout* tl, TestStockLayout* sl) : Game{ p,t,s,l,tl,sl }
  {
    AddToStock(Location(0, 0), &ChessPiece::WQ);
    AddToStock(Location(1, 0), &ChessPiece::WR);
    AddToStock(Location(2, 0), &ChessPiece::WB);
    AddToStock(Location(3, 0), &ChessPiece::WN);
    AddToStock(Location(0, 1), &ChessPiece::BQ);
    AddToStock(Location(1, 1), &ChessPiece::BR);
    AddToStock(Location(2, 1), &ChessPiece::BB);
    AddToStock(Location(3, 1), &ChessPiece::BN);
  }


  bool Test::TestOffset(void)
  {
    Location l{ 2,2 };
    assert((l == Location{ 2,2 }));

    for (auto& d : Offset::Rdirection)
    {
      Location l1{ 2,2 };
      Location l2 = l1 + d;
      l1 += d;
      assert((l1 == l2));
    }
    return true;
  }

  bool Test::TestEvaluate(void)
  {
    try
    {
      //TestGame game(4, 4);

      //std::vector<Move> m{game.GetAllMoves()};

      //assert(m.size() == 16);
      //assert(m[0].GetFr() == Location(0, 0)); assert(m[0].GetTo() == Location(1, 1));
      //assert(m[1].GetFr() == Location(0, 0)); assert(m[1].GetTo() == Location(0, 1));
      //assert(m[2].GetFr() == Location(0, 2)); assert(m[2].GetTo() == Location(0, 3));
      //assert(m[3].GetFr() == Location(0, 2)); assert(m[3].GetTo() == Location(0, 1));
      //assert(m[4].GetFr() == Location(1, 0)); assert(m[4].GetTo() == Location(2, 1));
      //assert(m[5].GetFr() == Location(1, 0)); assert(m[5].GetTo() == Location(0, 1));
      //assert(m[6].GetFr() == Location(1, 2)); assert(m[6].GetTo() == Location(3, 3));
      //assert(m[7].GetFr() == Location(1, 2)); assert(m[7].GetTo() == Location(3, 1));
      //assert(m[8].GetFr() == Location(2, 0)); assert(m[8].GetTo() == Location(3, 1));
      //assert(m[9].GetFr() == Location(2, 0)); assert(m[9].GetTo() == Location(1, 1));
      //assert(m[10].GetFr() == Location(2, 0)); assert(m[10].GetTo() == Location(2, 1));
      //assert(m[11].GetFr() == Location(2, 2)); assert(m[11].GetTo() == Location(2, 3));
      //assert(m[12].GetFr() == Location(2, 2)); assert(m[12].GetTo() == Location(2, 1));
      //assert(m[13].GetFr() == Location(3, 0)); assert(m[13].GetTo() == Location(2, 1));
      //assert(m[14].GetFr() == Location(3, 2)); assert(m[14].GetTo() == Location(1, 3));
      //assert(m[15].GetFr() == Location(3, 2)); assert(m[15].GetTo() == Location(1, 1));

      //for (auto& mi : m)
      //{
      //  Move m0 = mi;
      //  assert(m0.GetFr() == mi.GetFr()); assert(m0.GetTo() == mi.GetTo());
      //  assert(m0 == mi);
      //}


      //unsigned int pl0 = game.Plies(static_cast<unsigned int> (m.size()));
      //assert(pl0 == 4);

      //MainPosition* pos = game.GetPosition();
      //assert(pos->OnTurn() == &Color::White);


      //AIContext plist{};
      //pos->EvaluateStatically();
      //plist.insert(pos);

      //Move Best{std::vector<Step>{}};
      //PositionValue max = PositionValue::PValueType::Lost;
      //for (auto& mi : m)                                      // for all possible opponent's moves
      //{
      //  MainPosition* pp(pos->Clone());                       // create a copy of the board
      //  std::vector<const Piece*> taken = pp->Execute(mi);    // apply the move to the copy, return material taken
      //  pp->NextPlayer();
      //  PositionValue r{};

      //  auto pp0 = plist.find(pp);
      //  if (pp0 == plist.end())
      //  {
      //    r = -pp->EvaluateStatically();                       // static positional evaluation from the player on turn's point of view
      //    plist.insert(pp);
      //  }
      //  else
      //  {
      //    r = (*pp0)->value;
      //    delete pp;
      //  }

      //  mi.SetValue(r);
      //  if (r > max)
      //  {
      //    max = r;                                // save best rating for sorting
      //    Best = mi;
      //    assert(Best.GetFr() == mi.GetFr()); assert(Best.GetTo() == mi.GetTo());
      //    assert(Best == mi);
      //  }
      //}

      //assert(m.size() == 16);
      //assert(m[ 0].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[ 1].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[ 2].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[ 3].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[ 4].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[ 5].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[ 6].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[ 7].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[ 8].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[ 9].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[10].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[11].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[12].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[13].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[14].GetSteps()[0].GetType() == Step::StepType::Take);
      //assert(m[15].GetSteps()[0].GetType() == Step::StepType::Take);

      //assert(m[ 0].GetValue() ==  -40);
      //assert(m[ 1].GetValue() ==  580);
      //assert(m[ 2].GetValue() ==  140);
      //assert(m[ 3].GetValue() ==  580);
      //assert(m[ 4].GetValue() ==  600);
      //assert(m[ 5].GetValue() ==  580);
      //assert(m[ 6].GetValue() ==  -60);
      //assert(m[ 7].GetValue() ==  -20);
      //assert(m[ 8].GetValue() ==  -40);
      //assert(m[ 9].GetValue() ==  -20);
      //assert(m[10].GetValue() ==  620);
      //assert(m[11].GetValue() ==  160);
      //assert(m[12].GetValue() ==  620);
      //assert(m[13].GetValue() ==  580);
      //assert(m[14].GetValue() == -100);
      //assert(m[15].GetValue() ==  -20);

      AIContext plist{};
      TestGame game(3, 3);
      MainPosition* pos = game.GetPosition()->Clone();
      pos->EvaluateStatically();
      plist.insert(pos);

      TestMoveUndo(pos);
      TestTaken(pos);

      {
        bool b00 = !(PositionValue{ PositionValue::Undefined } < PositionValue{ PositionValue::Lost });
        bool b01 = (PositionValue{ PositionValue::Undefined } < PositionValue{ PositionValue::Tie });
        bool b02 = (PositionValue{ PositionValue::Undefined } < PositionValue{ PositionValue::Won });
        bool b03 = (PositionValue{ PositionValue::Undefined } < PositionValue{ PositionValue::Normal });
        bool b04 = !(PositionValue{ PositionValue::Undefined } < PositionValue{ PositionValue::Undefined });

        bool b10 = !(PositionValue{ PositionValue::Lost } < PositionValue{ PositionValue::Lost });
        bool b11 = (PositionValue{ PositionValue::Lost } < PositionValue{ PositionValue::Tie });
        bool b12 = (PositionValue{ PositionValue::Lost } < PositionValue{ PositionValue::Won });
        bool b13 = (PositionValue{ PositionValue::Lost } < PositionValue{ PositionValue::Normal });
        bool b14 = (PositionValue{ PositionValue::Lost } < PositionValue{ PositionValue::Undefined });

        bool b20 = !(PositionValue{ PositionValue::Won } < PositionValue{ PositionValue::Lost });
        bool b21 = !(PositionValue{ PositionValue::Won } < PositionValue{ PositionValue::Tie });
        bool b22 = !(PositionValue{ PositionValue::Won } < PositionValue{ PositionValue::Won });
        bool b23 = !(PositionValue{ PositionValue::Won } < PositionValue{ PositionValue::Normal });
        bool b24 = !(PositionValue{ PositionValue::Won } < PositionValue{ PositionValue::Undefined });

        bool b30 = !(PositionValue{ PositionValue::Tie } < PositionValue{ PositionValue::Lost });
        bool b31 = !(PositionValue{ PositionValue::Tie } < PositionValue{ PositionValue::Tie });
        bool b32 = (PositionValue{ PositionValue::Tie } < PositionValue{ PositionValue::Won });
        bool b33 = !(PositionValue{ PositionValue::Tie } < PositionValue{ -1 });
        bool b34 = (PositionValue{ PositionValue::Tie } < PositionValue{ 1 });
        bool b35 = !(PositionValue{ PositionValue::Tie } < PositionValue{ PositionValue::Undefined });

        bool b40 = !(PositionValue{ PositionValue::Normal } < PositionValue{ PositionValue::Lost });
        bool b41 = (PositionValue{ -1 } < PositionValue{ PositionValue::Tie });
        bool b42 = !(PositionValue{ 1 } < PositionValue{ PositionValue::Tie });
        bool b43 = (PositionValue{ -1 } < PositionValue{ 1 });
        bool b44 = !(PositionValue{ 1 } < PositionValue{ -1 });
        bool b45 = (PositionValue{ PositionValue::Normal } < PositionValue{ PositionValue::Won });
        bool b46 = !(PositionValue{ PositionValue::Normal } < PositionValue{ PositionValue::Undefined });

        bool b99 =
          b00 &&
          b01 &&
          b02 &&
          b03 &&
          b04 &&
          b10 &&
          b11 &&
          b12 &&
          b13 &&
          b14 &&
          b20 &&
          b21 &&
          b22 &&
          b23 &&
          b24 &&
          b30 &&
          b31 &&
          b32 &&
          b33 &&
          b34 &&
          b35 &&
          b40 &&
          b41 &&
          b42 &&
          b43 &&
          b44 &&
          b45 &&
          b46;
        assert(b99);
      }

      PositionValue Best{ 0 };
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 1);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 2);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 3);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 4);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 5);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 6);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 7);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 8);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 9);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 10);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 11);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 12);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 13);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 14);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 15);
      Best = pos->Evaluate(plist, &Color::White, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 16);

      bool ok{ true };
      //    ok = game.AIMove();
      assert(ok);

      for (auto& p : plist) delete p;
    }
    catch (...)
    {
      static int i{ 0 };
    };
    return true;
  }

  bool Test::TestAll(void)
  {
    assert(TestOffset());
    assert(TestEvaluate());
    return true;
  }

  class TestPosAccess : public MainPosition
  {
  public:
    inline const std::vector<Move> GetMoveList(bool w) const { return w ? movelistW : movelistB; }
  };
  bool Test::TestMoveUndo(const MainPosition* pos)  // try all moves (for both colors) and undo them, to verify the Undo method
  {
    static int z{ 0 };
    const TestPosAccess* const tp = static_cast<const TestPosAccess* const>(pos);

    for (auto& m : tp->GetMoveList(true))
    {
      z++;
      std::unique_ptr<MainPosition> pos1(pos->Clone());
      pos1->Execute(m);
      pos1->Undo(m);
      assert(*pos1 == *pos);
    }
    for (auto& m : tp->GetMoveList(false))
    {
      z++;
      std::unique_ptr<MainPosition> pos1(pos->Clone());
      pos1->Execute(m);
      pos1->Undo(m);
      assert(*pos1 == *pos);
    }
    return true;
  }

  bool Test::TestTaken(const MainPosition* pos)  // verify all taken pieces
  {
    static int z{ 0 };
    const TestPosAccess* const tp = static_cast<const TestPosAccess* const>(pos);

    for (auto& m : tp->GetMoveList(true))
    {
      for (auto& s : m.GetSteps())
      {
        for (auto& t : s.GetTake())
        {
          z++;
          assert(t.GetPiece() == pos->GetPiece(t.GetLocation()));
        }
      }
    }
    for (auto& m : tp->GetMoveList(false))
    {
      for (auto& s : m.GetSteps())
      {
        for (auto& t : s.GetTake())
        {
          z++;
          assert(t.GetPiece() == pos->GetPiece(t.GetLocation()));
        }
      }
    }
    return true;
  }

  bool Test::TestPosition(const MainPosition* pos)  // verify the position is ok
  {
    AIContext plist{};

    try
    {
      std::size_t s = sizeof(*pos);
      for (unsigned int z = s; z < 500000+s; ++z)
      {
        MainPosition* p = pos->Clone();

        plist.insert(p);
      }
    }
    catch (const CMemoryException& /*ex*/)
    {

    }

    static int z{ 0 };

    z++;
    assert(pos != nullptr);
    assert(reinterpret_cast<unsigned int>(pos) != 0xcdcdcdcd);
    assert(TestTaken(pos));
    assert(TestMoveUndo(pos));
    return true;
  }

  bool Test::TestPList(const AIContext& plist)  // verify the position list
  {
    static int z{ 0 };
    for (auto& it : plist)
    {
      z++;
      assert(TestPosition(it));
    }
    return true;
  }

  bool Test::TestValue(const MainPosition* pos)
  {
    static int z{ 0 };
    z++;

    char buffer[100];
    std::string s{};
    for (unsigned int i = 0; i < pos->sequence.size(); ++i)
    {
      const Move& m = pos->sequence[i];
      sprintf_s(buffer, "%d. %c%c - %c%c, ", i + 1, m.GetFr().GetLocation().x + 'a', m.GetFr().GetLocation().y + '1',
                                                    m.GetTo().GetLocation().x + 'a', m.GetTo().GetLocation().y + '1');
      s += buffer;
    }
    sprintf_s(buffer, "pos value = %u\n", (unsigned int) pos->GetValue(true));
    s += buffer;
    TRACE(s.c_str());
    return true;
  }

}
