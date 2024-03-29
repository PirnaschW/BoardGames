#include "Unit Test - Games.h"

#include "../../../Source/Games/Checkers/CheckersGame.h"
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

      Assert::IsTrue(CheckersPiece::CheckersPieceW.Promote(true) == CheckersPiece::CheckersQueenW);
      Assert::IsTrue(CheckersPiece::CheckersPieceB.Promote(true) == CheckersPiece::CheckersQueenB);

      const VariantChosen v{ 0,'\0', 8U, 8U };
      CheckersBoard p{ v, Variants<Checkers::CheckersGame>::GetPieces(v), CheckersGame::GetDimensions(v) };
      p.SetStartingBoard();
      Moves m{};

      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPartID::Main, 0, 7 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPartID::Main, 1, 6 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPartID::Main, 0, 5 }, m);
      Assert::IsTrue(m.size() == 1);

      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPartID::Main, 2, 7 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPartID::Main, 3, 6 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceW.CollectMoves(p, Location{ BoardPartID::Main, 2, 5 }, m);
      Assert::IsTrue(m.size() == 2);

      m.clear();
      CheckersPiece::CheckersPieceB.CollectMoves(p, Location{ BoardPartID::Main, 1, 2 }, m);
      Assert::IsTrue(m.size() == 2);
      m.clear();
      CheckersPiece::CheckersPieceB.CollectMoves(p, Location{ BoardPartID::Main, 0, 1 }, m);
      Assert::IsTrue(m.size() == 0);
      m.clear();
      CheckersPiece::CheckersPieceB.CollectMoves(p, Location{ BoardPartID::Main, 7, 2 }, m);
      Assert::IsTrue(m.size() == 1);
    }

    TEST_METHOD(TestCheckersBoardClone)
    {
      const VariantChosen v{ 0,'\0', 8U, 8U };
      const CheckersBoard p1{ v, Variants<Checkers::CheckersGame>::GetPieces(v), CheckersGame::GetDimensions(v) };
      Board* pm = p1.Clone();
      Assert::IsTrue(pm != nullptr);

      CheckersBoard* p2{ dynamic_cast<CheckersBoard*>(pm) };
      Assert::IsTrue(p2 != nullptr);
      Assert::IsTrue(p1 == *p2);
      delete p2;
    }

    TEST_METHOD(TestCheckersGame)
    {
      const VariantChosen v{ 0,'\0', 8U, 8U };
      for (unsigned int z = 0; z < 10; ++z)  // make sure creating a game works multiple times
      {
        CheckersBoard p{ v, Variants<Checkers::CheckersGame>::GetPieces(v), CheckersGame::GetDimensions(v) };
      }
    }

    TEST_METHOD(TestCheckersMovesNoQueens)
    {
      srand(0); // always start with the same sequence to allow debugging!
      for (unsigned int z = 0; z < 100; ++z)
      {
        const VariantChosen v{ 0,'\0', MaxX, MaxY };
        CheckersBoard board_{ v, Variants<Checkers::CheckersGame>::GetPieces(v), CheckersGame::GetDimensions(v) };

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
            board_.SetPiece(Location{ BoardPartID::Main, i,j }, *p);
          }
        }

        Moves mw{};
        Moves mb{};
        for (Coordinate i = 0; i < MaxX; ++i)                             // find all possible moves in this postion
        {
          for (Coordinate j = 0; j < MaxY; ++j)
          {
            const Location& lf{ BoardPartID::Main, i, j };
            const Piece& p = board_.GetPiece(lf);
            if (p == CheckersPiece::CheckersPieceW)
            {
              const Location& lul{ BoardPartID::Main, i - 1U, j - 1U };     // up left
              const Piece& pul = board_.GetPiece(lul);
              if (pul == Piece::NoPiece)
              {
                const Piece& pp = ((lul.y_ == 0) && p.IsPromotable()) ? p.Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionLift>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionDrop>(lul, pp));      // place it on target
                mw.push_back(std::make_shared<Move>(a));                  // add move to move list
              }

              const Location& lur{ BoardPartID::Main, i + 1U, j - 1U };     // up right
              const Piece& pur = board_.GetPiece(lur);
              if (pur == Piece::NoPiece)
              {
                const Piece& pp = ((lur.y_ == 0) && p.IsPromotable()) ? p.Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionLift>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionDrop>(lur, pp));      // place it on target
                mw.push_back(std::make_shared<Move>(a));                  // add move to move list
              }
            }
            else if (p == CheckersPiece::CheckersPieceB)
            {
              const Location& ldl{ BoardPartID::Main, i - 1U, j + 1U };     // down left
              const Piece& pdl = board_.GetPiece(ldl);
              if (pdl == Piece::NoPiece)
              {
                const Piece& pp = ((ldl.y_ == MaxY - 1) && p.IsPromotable()) ? p.Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionLift>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionDrop>(ldl, pp));      // place it on target
                mb.push_back(std::make_shared<Move>(a));                  // add move to move list
              }

              const Location& ldr{ BoardPartID::Main, i + 1U, j + 1U };     // dowm right
              const Piece& pdr = board_.GetPiece(ldr);
              if (pdr == Piece::NoPiece)
              {
                const Piece& pp = ((ldr.y_ == MaxY - 1) && p.IsPromotable()) ? p.Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionLift>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionDrop>(ldr, pp));      // place it on target
                mb.push_back(std::make_shared<Move>(a));                  // add move to move list
              }
            }
            else
            {
              Assert::IsTrue(p == Piece::NoPiece);                       // must be empty field
            }

          }
        }

        board_.GetAllMoves();                                                // create move list with productive code
        Moves& mw0 = board_.GetMoveList(true);
        Assert::IsTrue(mw0.size() == mw.size());                          // compare white move list SIZE with list produced in test code
        Moves& mb0 = board_.GetMoveList(false);
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
        const VariantChosen v{ 0,'\0', MaxX, MaxY };
        CheckersBoard board_{ v, Variants<Checkers::CheckersGame>::GetPieces(v), CheckersGame::GetDimensions(v) };


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
            board_.SetPiece(Location{ BoardPartID::Main, i,j }, *p);
          }
        }

        Moves mw{};
        Moves mb{};
        for (Coordinate i = 0; i < MaxX; ++i)                             // find all possible moves in this postion
        {
          for (Coordinate j = 0; j < MaxY; ++j)
          {
            const Location& lf{ BoardPartID::Main, i, j };
            const Piece& p = board_.GetPiece(lf);
            if (p == CheckersPiece::CheckersPieceW)
            {
              const Location& lul{ BoardPartID::Main, i - 1U, j - 1U };     // up left
              const Piece& pul = board_.GetPiece(lul);
              if (pul == Piece::NoPiece)
              {
                const Piece& pp = ((lul.y_ == 0) && p.IsPromotable()) ? p.Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionLift>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionDrop>(lul, pp));      // place it on target
                mw.push_back(std::make_shared<Move>(a));                  // add move to move list
              }

              const Location& lur{ BoardPartID::Main, i + 1U, j - 1U };     // up right
              const Piece& pur = board_.GetPiece(lur);
              if (pur == Piece::NoPiece)
              {
                const Piece& pp = ((lur.y_ == 0) && p.IsPromotable()) ? p.Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionLift>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionDrop>(lur, pp));      // place it on target
                mw.push_back(std::make_shared<Move>(a));                  // add move to move list
              }
            }
            else if (p == CheckersPiece::CheckersPieceB)
            {
              const Location& ldl{ BoardPartID::Main, i - 1U, j + 1U };     // down left
              const Piece& pdl = board_.GetPiece(ldl);
              if (pdl == Piece::NoPiece)
              {
                const Piece& pp = ((ldl.y_ == MaxY - 1) && p.IsPromotable()) ? p.Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionLift>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionDrop>(ldl, pp));      // place it on target
                mb.push_back(std::make_shared<Move>(a));                  // add move to move list
              }

              const Location& ldr{ BoardPartID::Main, i + 1U, j + 1U };     // dowm right
              const Piece& pdr = board_.GetPiece(ldr);
              if (pdr == Piece::NoPiece)
              {
                const Piece& pp = ((ldr.y_ == MaxY - 1) && p.IsPromotable()) ? p.Promote(true) : p;
                Actions a{};
                a.push_back(std::make_shared<ActionLift>(lf, p));         // pick piece up
                a.push_back(std::make_shared<ActionDrop>(ldr, pp));      // place it on target
                mb.push_back(std::make_shared<Move>(a));                  // add move to move list
              }
            }
            else if (p == CheckersPiece::CheckersQueenW)
            {
              for (const Offset& o : Offset::BDirection)
              {
                for (Coordinate k = 1U; k < MaxX; ++k)
                {
                  const Location& lt{ lf + o * k };
                  const Piece& pt = board_.GetPiece(lt);
                  if (pt == Piece::NoPiece)
                  {
                    Actions a{};
                    a.push_back(std::make_shared<ActionLift>(lf, p));       // pick piece up
                    a.push_back(std::make_shared<ActionDrop>(lt, p));      // place it on target
                    mw.push_back(std::make_shared<Move>(a));                // add move to move list
                  }
                  else break;                                               // end the loop if no space
                }
              }
            }
            else if (p == CheckersPiece::CheckersQueenB)
            {
              for (const Offset& o : Offset::BDirection)
              {
                for (Coordinate k = 1U; k < MaxX; ++k)
                {
                  const Location& lt{ lf + o * k };
                  const Piece& pt = board_.GetPiece(lt);
                  if (pt == Piece::NoPiece)
                  {
                    Actions a{};
                    a.push_back(std::make_shared<ActionLift>(lf, p));       // pick piece up
                    a.push_back(std::make_shared<ActionDrop>(lt, p));      // place it on target
                    mb.push_back(std::make_shared<Move>(a));                // add move to move list
                  }
                  else break;                                               // end the loop if no space
                }
              }
            }
            else
            {
              Assert::IsTrue(p == Piece::NoPiece);                       // must be empty field
            }

          }
        }

        board_.GetAllMoves();                                                // create move list with productive code
        Moves& mw0 = board_.GetMoveList(true);
        Assert::IsTrue(mw0.size() == mw.size());                          // compare white move list SIZE with list produced in test code
        Moves& mb0 = board_.GetMoveList(false);
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
