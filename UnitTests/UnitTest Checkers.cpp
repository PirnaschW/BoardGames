#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BoardGamesCore;

#include "../BG/Game Checkers/CheckersGame.h"
using namespace Checkers;

namespace UnitTestCheckers
{
  constexpr Coordinate MaxX{ 8U };
  constexpr Coordinate MaxY{ 8U };

  TEST_CLASS(UnitTestCheckers)
  {
  public:

    TEST_METHOD(TestCheckersPieces)
    {
      Assert::IsTrue(CheckersPiece::CheckersPieceW.IsPromotable());
      Assert::IsTrue(CheckersPiece::CheckersPieceB.IsPromotable());
      Assert::IsFalse(CheckersPiece::CheckersKingW.IsPromotable());
      Assert::IsFalse(CheckersPiece::CheckersKingB.IsPromotable());
      Assert::IsFalse(CheckersPiece::CheckersQueenW.IsPromotable());
      Assert::IsFalse(CheckersPiece::CheckersQueenB.IsPromotable());

      Assert::IsTrue(CheckersPiece::CheckersPieceW.Promote(true) == &CheckersPiece::CheckersQueenW);
      Assert::IsTrue(CheckersPiece::CheckersPieceB.Promote(true) == &CheckersPiece::CheckersQueenB);

      const CheckersPosition p{ Variants<Checkers::CheckersGame>::GetPieces(), CheckersGame::GetDimensions(8U, 8U) };
      Moves m{};

      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPart::Main, 0, 7 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPart::Main, 1, 6 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPart::Main, 0, 5 }, m);
      Assert::IsTrue(m.size() == 1);

      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPart::Main, 2, 7 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPart::Main, 3, 6 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPart::Main, 2, 5 }, m);
      Assert::IsTrue(m.size() == 2);

      m.clear();
      CheckersPiece::CheckersPieceB.CollectMoves(p, Location{ BoardPart::Main, 1, 2 }, m);
      Assert::IsTrue(m.size() == 2);
      m.clear();
      CheckersPiece::CheckersPieceB.CollectMoves(p, Location{ BoardPart::Main, 0, 1 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceB.CollectMoves(p, Location{ BoardPart::Main, 7, 2 }, m);
      Assert::IsTrue(m.size() == 1);
    }

    TEST_METHOD(TestCheckersPositionClone)
    {
      CheckersPosition p1{ Variants<Checkers::CheckersGame>::GetPieces(), CheckersGame::GetDimensions(8U, 8U) };
      MainPosition* pm = p1.Clone();
      Assert::IsTrue(pm != nullptr);

      CheckersPosition* p2{ dynamic_cast<CheckersPosition*>(pm) };
      Assert::IsTrue(p2 != nullptr);
      Assert::IsTrue(p1 == *p2);
    }

    TEST_METHOD(TestCheckersGame)
    {
      for (unsigned int z = 0; z < 10; ++z)  // make sure creating a game works multiple times
      {
        CheckersGame g{ Variants<CheckersGame>::GetPieces(), CheckersGame::GetDimensions(8U, 8U) };
      }
    }

    TEST_METHOD(TestCheckersMovesNoQueens)
    {
      srand(0); // always start with the same sequence to allow debugging!
      for (unsigned int z = 0; z < 100; ++z)
      {
        CheckersPosition pos{ Variants<Checkers::CheckersGame>::GetPieces(), CheckersGame::GetDimensions(MaxX, MaxY) };

        for (Coordinate i = 0; i < MaxX; ++i)                             // create a random position
        {
          for (Coordinate j = 0; j < MaxY; ++j)
          {
            if ((i + j) % 2) continue;

            const Piece* p{ &Piece::NoPiece };
            switch (rand() % 4)
            {
              case 0: p = &CheckersPiece::CheckersPieceW; break;
              case 1: p = &CheckersPiece::CheckersPieceB; break;
              default: break;
            }
            pos.SetPiece(Location{ BoardPart::Main, i,j }, p);
          }
        }

        Moves mw{};
        Moves mb{};
        for (Coordinate i = 0; i < MaxX; ++i)                             // find all possible moves in this postion
        {
          for (Coordinate j = 0; j < MaxY; ++j)
          {
            const Location& lf{ BoardPart::Main, i, j };
            const Piece* p = pos.GetPiece(lf);
            if (p == &CheckersPiece::CheckersPieceW)
            {
              const Location& lul{ BoardPart::Main, i - 1U, j - 1U };     // up left
              const Piece* pul = pos.GetPiece(lul);
              if (pul == &Piece::NoPiece)
              {
                const Piece* pp = ((lul.y_ == 0) && p->IsPromotable()) ? p->Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionTake>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionPlace>(lul, pp));      // place it on target
                mw.push_back(std::make_shared<Move>(a));                  // add move to move list
              }

              const Location& lur{ BoardPart::Main, i + 1U, j - 1U };     // up right
              const Piece* pur = pos.GetPiece(lur);
              if (pur == &Piece::NoPiece)
              {
                const Piece* pp = ((lur.y_ == 0) && p->IsPromotable()) ? p->Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionTake>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionPlace>(lur, pp));      // place it on target
                mw.push_back(std::make_shared<Move>(a));                  // add move to move list
              }
            }
            else if (p == &CheckersPiece::CheckersPieceB)
            {
              const Location& ldl{ BoardPart::Main, i - 1U, j + 1U };     // down left
              const Piece* pdl = pos.GetPiece(ldl);
              if (pdl == &Piece::NoPiece)
              {
                const Piece* pp = ((ldl.y_ == MaxY - 1) && p->IsPromotable()) ? p->Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionTake>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionPlace>(ldl, pp));      // place it on target
                mb.push_back(std::make_shared<Move>(a));                  // add move to move list
              }

              const Location& ldr{ BoardPart::Main, i + 1U, j + 1U };     // dowm right
              const Piece* pdr = pos.GetPiece(ldr);
              if (pdr == &Piece::NoPiece)
              {
                const Piece* pp = ((ldr.y_ == MaxY - 1) && p->IsPromotable()) ? p->Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionTake>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionPlace>(ldr, pp));      // place it on target
                mb.push_back(std::make_shared<Move>(a));                  // add move to move list
              }
            }
            else
            {
              Assert::IsTrue(p == &Piece::NoPiece);                       // must be empty field
            }

          }
        }

        pos.GetAllMoves();                                                // create move list with productive code
        Moves& mw0 = pos.GetMoveList(true);
        Assert::IsTrue(mw0.size() == mw.size());                          // compare white move list SIZE with list produced in test code
        Moves& mb0 = pos.GetMoveList(false);
        Assert::IsTrue(mb0.size() == mb.size());                          // compare black move list SIZE with list produced in test code

        // sort order for moves
        auto sorter = [](const MoveP& p1, const MoveP& p2)                // lambda to sort moves by coordinates
        {
          const Location& l1f = p1->GetFrL();
          const Location& l2f = p2->GetFrL();
          if (l1f.x_ != l2f.x_) return l1f.x_ < l2f.x_;                   // first sort by From-X
          if (l1f.y_ != l2f.y_) return l1f.y_ < l2f.y_;                   // then by From-Y
          const Location& l1t = p1->GetToL();
          const Location& l2t = p2->GetToL();
          if (l1t.x_ != l2t.x_) return l1t.x_ < l2t.x_;                   // then by To-X
          if (l1t.y_ != l2t.y_) return l1t.y_ < l2t.y_;                   // then by To-Y
          return p1 < p2;                                                 // and finally by value
        };

        sort(mw.begin(), mw.end(), sorter);                               // sort both arrays the same way to be able to compare them
        sort(mw0.begin(), mw0.end(), sorter);                             // sort both arrays the same way to be able to compare them
        for (unsigned int i = 0; i < mw0.size(); ++i)                     // compare white move list CONTENT with list produced in test code
        {
          if (*mw0[i] != *mw[i])
          {
            assert(*mw0[i] == *mw[i]);
          }
          Assert::IsTrue(*mw0[i] == *mw[i]);
        }

        sort(mb.begin(), mb.end(), sorter);
        sort(mb0.begin(), mb0.end(), sorter);
        for (unsigned int i = 0; i < mb0.size(); ++i)                     // compare black move list CONTENT with list produced in test code
        {
          if (*mb0[i] != *mb[i])
          {
            assert(*mb0[i] == *mb[i]);
          }
          Assert::IsTrue(*mb0[i] == *mb[i]);
        }
      }
    }


    TEST_METHOD(TestCheckersMovesQueens)
    {
      srand(0); // always start with the same sequence to allow debugging!
      for (unsigned int z = 0; z < 100; ++z)  // for now, only 1 time
      {
        CheckersPosition pos{ Variants<Checkers::CheckersGame>::GetPieces(), CheckersGame::GetDimensions(MaxX, MaxY) };


        for (Coordinate i = 0; i < MaxX; ++i)                             // create a random position
        {
          for (Coordinate j = 0; j < MaxY; ++j)
          {
            if ((i + j) % 2) continue;

            const Piece* p{ &Piece::NoPiece };
            switch (rand() % 8)
            {
              case 0: p = &CheckersPiece::CheckersPieceW; break;
              case 1: p = &CheckersPiece::CheckersPieceB; break;
              case 2: p = &CheckersPiece::CheckersQueenW; break;
              case 3: p = &CheckersPiece::CheckersQueenB; break;
              default: break;
            }
            pos.SetPiece(Location{ BoardPart::Main, i,j }, p);
          }
        }

        Moves mw{};
        Moves mb{};
        for (Coordinate i = 0; i < MaxX; ++i)                             // find all possible moves in this postion
        {
          for (Coordinate j = 0; j < MaxY; ++j)
          {
            const Location& lf{ BoardPart::Main, i, j };
            const Piece* p = pos.GetPiece(lf);
            if (p == &CheckersPiece::CheckersPieceW)
            {
              const Location& lul{ BoardPart::Main, i - 1U, j - 1U };     // up left
              const Piece* pul = pos.GetPiece(lul);
              if (pul == &Piece::NoPiece)
              {
                const Piece* pp = ((lul.y_ == 0) && p->IsPromotable()) ? p->Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionTake>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionPlace>(lul, pp));      // place it on target
                mw.push_back(std::make_shared<Move>(a));                  // add move to move list
              }

              const Location& lur{ BoardPart::Main, i + 1U, j - 1U };     // up right
              const Piece* pur = pos.GetPiece(lur);
              if (pur == &Piece::NoPiece)
              {
                const Piece* pp = ((lur.y_ == 0) && p->IsPromotable()) ? p->Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionTake>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionPlace>(lur, pp));      // place it on target
                mw.push_back(std::make_shared<Move>(a));                  // add move to move list
              }
            }
            else if (p == &CheckersPiece::CheckersPieceB)
            {
              const Location& ldl{ BoardPart::Main, i - 1U, j + 1U };     // down left
              const Piece* pdl = pos.GetPiece(ldl);
              if (pdl == &Piece::NoPiece)
              {
                const Piece* pp = ((ldl.y_ == MaxY - 1) && p->IsPromotable()) ? p->Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionTake>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionPlace>(ldl, pp));      // place it on target
                mb.push_back(std::make_shared<Move>(a));                  // add move to move list
              }

              const Location& ldr{ BoardPart::Main, i + 1U, j + 1U };     // dowm right
              const Piece* pdr = pos.GetPiece(ldr);
              if (pdr == &Piece::NoPiece)
              {
                const Piece* pp = ((ldr.y_ == MaxY - 1) && p->IsPromotable()) ? p->Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionTake>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionPlace>(ldr, pp));      // place it on target
                mb.push_back(std::make_shared<Move>(a));                  // add move to move list
              }
            }
            else if (p == &CheckersPiece::CheckersQueenW)
            {
              for (const Offset& o : Offset::Bdirection)
              {
                for (Coordinate k = 1U; k < MaxX; ++k)
                {
                  const Location& lt{ lf + o * k };
                  const Piece* pt = pos.GetPiece(lt);
                  if (pt == &Piece::NoPiece)
                  {
                    Actions a{};
                    a.push_back(std::make_shared<ActionTake>(lf, p));       // pick piece up
                    a.push_back(std::make_shared<ActionPlace>(lt, p));      // place it on target
                    mw.push_back(std::make_shared<Move>(a));                // add move to move list
                  }
                  else break;                                               // end the loop if no space
                }
              }
            }
            else if (p == &CheckersPiece::CheckersQueenB)
            {
              for (const Offset& o : Offset::Bdirection)
              {
                for (Coordinate k = 1U; k < MaxX; ++k)
                {
                  const Location& lt{ lf + o * k };
                  const Piece* pt = pos.GetPiece(lt);
                  if (pt == &Piece::NoPiece)
                  {
                    Actions a{};
                    a.push_back(std::make_shared<ActionTake>(lf, p));       // pick piece up
                    a.push_back(std::make_shared<ActionPlace>(lt, p));      // place it on target
                    mb.push_back(std::make_shared<Move>(a));                // add move to move list
                  }
                  else break;                                               // end the loop if no space
                }
              }
            }
            else
            {
              Assert::IsTrue(p == &Piece::NoPiece);                       // must be empty field
            }

          }
        }

        pos.GetAllMoves();                                                // create move list with productive code
        Moves& mw0 = pos.GetMoveList(true);
        Assert::IsTrue(mw0.size() == mw.size());                          // compare white move list SIZE with list produced in test code
        Moves& mb0 = pos.GetMoveList(false);
        Assert::IsTrue(mb0.size() == mb.size());                          // compare black move list SIZE with list produced in test code

        // sort order for moves
        auto sorter = [](const MoveP& p1, const MoveP& p2)                // lambda to sort moves by coordinates
        {
          const Location& l1f = p1->GetFrL();
          const Location& l2f = p2->GetFrL();
          if (l1f.x_ != l2f.x_) return l1f.x_ < l2f.x_;                   // first sort by From-X
          if (l1f.y_ != l2f.y_) return l1f.y_ < l2f.y_;                   // then by From-Y
          const Location& l1t = p1->GetToL();
          const Location& l2t = p2->GetToL();
          if (l1t.x_ != l2t.x_) return l1t.x_ < l2t.x_;                   // then by To-X
          if (l1t.y_ != l2t.y_) return l1t.y_ < l2t.y_;                   // then by To-Y
          return p1 < p2;                                                 // and finally by value
        };

        sort(mw.begin(), mw.end(), sorter);                               // sort both arrays the same way to be able to compare them
        sort(mw0.begin(), mw0.end(), sorter);                             // sort both arrays the same way to be able to compare them
        for (unsigned int i = 0; i < mw0.size(); ++i)                     // compare white move list CONTENT with list produced in test code
        {
          if (*mw0[i] != *mw[i])
          {
            assert(*mw0[i] == *mw[i]);
          }
          Assert::IsTrue(*mw0[i] == *mw[i]);
        }

        sort(mb.begin(), mb.end(), sorter);
        sort(mb0.begin(), mb0.end(), sorter);
        for (unsigned int i = 0; i < mb0.size(); ++i)                     // compare black move list CONTENT with list produced in test code
        {
          if (*mb0[i] != *mb[i])
          {
            assert(*mb0[i] == *mb[i]);
          }
          Assert::IsTrue(*mb0[i] == *mb[i]);
        }
      }
    }

  };

}
