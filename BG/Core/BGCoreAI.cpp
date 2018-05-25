#include "stdafx.h"
#include "../Game Test/TestGame.h"

namespace BoardGamesCore
{

  bool Game::AIMove(void)
  {
    MainPosition* p{ pos->GetPosition(plist) };                           // retrieve position from list

   // Test::Test::TestPosition(p);

    Move::PositionValue value{ p->value };
    plies = 16;
    for (unsigned int pl = 1; pl <= plies; pl++)                          // use iterative deepening
    {
//      assert(Test::Test::TestPList(plist));
      value = p->Evaluate(plist, CurrentPlayer()->GetColor(), -Move::win, Move::win, pl);
      if (plist.size() > 500000) break;
    }

    if (value < -100000)
    {
      ::AfxMessageBox(L"Computer resigns - Player wins!");
      SetAlive(false);
      return false;
    }
    if (value > 100000) {
      ::AfxMessageBox(L"You might as well resign - Computer will win!");
    }

    Execute(p->movelistB[0]);
    return true;
  }


  Move::PositionValue MainPosition::Evaluate(MainPosition::PList& plist, const Color* on, int alpha, int beta, unsigned int plies)
  {
    auto& movelist = (on == &Color::White ? movelistW : movelistB);
    if (movelist.size() == 0) return value;

    assert(plist.find(this) != plist.end());                              // the current position must have been checked before

    for (auto& m : movelist)                                              // for all possible moves
    {
      MainPosition* p{ GetPosition(plist,&m) };                           // find the board in the list
      m.SetValue(p->value);                                               // use the known value
    }

    auto l = [on](Move const& a, Move const& b)
    { return (on == &Color::White) ? a < b : b < a; };                    // define sort predicate, depending on who's turn it is
    std::sort(movelist.begin(), movelist.end(), l);                       // sort the moves by their value 
    if (plies == 1) return value = movelist.front().GetValue();           // 1-ply means just use that result - return best move

    // more than 1 ply: drill iteratively deeper                       
    for (auto& m : movelist)                                              // for all possible opponent's moves
    {
      MainPosition* p{ GetPosition(plist,&m) };                           // find the board in the list
      Move::PositionValue v = (on == &Color::White ? 1 : -1) *
        p->Evaluate(plist, !*on, -beta, -alpha, plies - 1);               // evaluate the result
      m.SetValue(v);                                                      // save value into position and move

      // apply alpha/beta pruning
      if (v >= beta) { return value = beta; }                             // cut branch off, use current value
      if (v > alpha) { alpha = v; }                                       // reduce range for alpha, continue
    }
    return value = alpha;
  }


  Move::PositionValue MainPosition::EvaluateStatically(void)   // as seen from White
  {
    // default evaluation: count all material, and add difference of move count. Overwrite for each game as needed
    if (movelistW.size() == 0) value = -Move::win;        // if no more moves, game over
    else if (movelistB.size() == 0) value = Move::win;
    else
    {
      value = static_cast<Move::PositionValue>(movelistW.size() - movelistB.size()) * 20;
      for (unsigned int j = 0; j < sizeY; j++)
      {
        for (unsigned int i = 0; i < sizeX; i++)         // loop through all locations
        {
          const Piece* p = GetPiece(Location{ i,j });
          if (p == nullptr) continue;                    // field does not exist
          if (p->IsColor(&Color::NoColor)) continue;     // nothing here
          value += (p->IsColor(&Color::White) ? 1 : -1) * p->GetValue();
        }
      }
    }
    return value;  // always as seen from White
  }

  MainPosition* MainPosition::GetPosition(MainPosition::PList& plist, Move* m) // execute move, maintain in PList
  {
    //if (plist.size() == plist.bucket_count())
    //  plist.rehash(plist.bucket_count() * 2);  // increase set proactively

    MainPosition* pos(Clone());                                           // create a copy of the board
    if (m != nullptr) pos->Execute(*m);                                   // execute move if provided

    auto pl0 = plist.find(pos);                                           // check if we evaluated this position before
    if (pl0 != plist.end())
    {
      delete pos;
      return *pl0;                                                        // found - return it
    }

    pos->GetAllMoves();                                                   // fill the move lists
    pos->EvaluateStatically();                                            // evaluate position statically
    auto pl1 = plist.insert(pos);                                         // and save it
    assert(reinterpret_cast<unsigned int>(*(pl1.first)) != 0xDDDDDDDD);
    assert(pl1.second);
    return *(pl1.first);                                                  // return the pointer to the new entry
  }

}
