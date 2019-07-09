#include "stdafx.h"
#include "../Game Test/TestGame.h"

namespace BoardGamesCore
{

  bool Game::AIMove(void)
  {
    MainPosition* p{ pos->GetPosition(plist) };                           // retrieve position from list
    assert(p != nullptr);

   // Test::Test::TestPosition(p);

    auto t_start = std::chrono::high_resolution_clock::now();
    double limit = 3.0;  // run for n seconds
    bool w = CurrentPlayer()->GetColor() == &Color::White;

    for (unsigned int pl = 0; true /*pl <= plies*/; pl++)                          // use iterative deepening
    {
      try
      {
        PositionValue value = p->Evaluate(plist, w, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, pl);
        pos->SetValue(w, p->GetValue(w));
        pos->SetDepth(p->GetDepth());
        if (value == PositionValue::PValueType::Lost)
        {
          ::AfxMessageBox(L"Computer resigns - Player wins!");
          _mode.Set(Mode::GameOver);
          return false;
        }
        if (value == PositionValue::PValueType::Won) {
          ::AfxMessageBox(L"You might as well resign - Computer will win!");
          break;
        }

        if (value == PositionValue::PValueType::Tie) {
          ::AfxMessageBox(L"Computer will hold a Draw.");
          break;
        }
      }
      catch (std::bad_alloc)
      {
        throw;
      }

      if (plist->size() > 150000)
        break;

      auto t_now = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = t_now - t_start;
      if (elapsed.count() > limit) break;
      plist->callback();
    }

    Execute(*(p->GetBestMove(CurrentPlayer()->GetColor() == &Color::White)));
    return true;
  }


  PositionValue MainPosition::Evaluate(AIContextP& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies)
  {
    if (plies == 0) return GetValue(w);
    //if (plist->size() > 200000) return GetValue(w);

    auto& movelist = GetMoveList(w);
    if (movelist.empty()) return GetValue(w);

    assert(plist->find(this) != plist->end());                              // the current position must have been checked before

    const auto l = [](MoveP a, MoveP b) { return *b < *a; };    // define sort predicate

    for (auto& m : movelist)                                              // for all possible opponent's moves
    {
      MainPosition* p{ GetPosition(plist,m) };                           // find the board in the list
      PositionValue v = -p->Evaluate(plist, !w, -beta, -alpha, plies - 1);   // evaluate the result
      assert(v != PositionValue::Undefined);
      m->SetValue(p->GetValue(w));                                         // save real position value into move for sorting

      // apply alpha/beta pruning
      if (v > alpha) { alpha = v; }                                       // reduce range for alpha, continue
      if (v >= beta)                                                      // cut branch off, use current value
      { 
        std::sort(movelist.begin(), movelist.end(), l);                   // sort the moves by their value (for the next level of depth
        SetValue(w, alpha);                                               // save top value in current position
        depth = plies;                                                    // save evaluation depth
        if (plies == 2) plist->callback();
        return beta;
      }
    }

    std::sort(movelist.begin(), movelist.end(), l);                       // sort the moves by their value (for the next level of depth
    SetValue(w, alpha);                                                   // save top value in current position
    depth = plies;                                                        // save evaluation depth
    if (plies == 2) plist->callback();
    return alpha;                                                         // return best value
  }


  void MainPosition::EvaluateStatically(void)   // as seen from White
  {
    assert(movelistW.empty());
    assert(movelistB.empty());
    assert(depth == 0);

    // default evaluation: count all material, and add 20 * difference of move count. Overwrite for each game as needed
    GetAllMoves();                                                                                        // fill the move lists
    depth = 1;
    if (onTurn == &Color::White && movelistW.empty()) value = PositionValue::PValueType::Lost;        // if no more moves, game over
    else if (onTurn == &Color::Black && movelistB.empty()) value = PositionValue::PValueType::Won;
    else
    {
      value = GetMoveCountFactor() * (movelistW.size() - movelistB.size());
      for (Coordinate j = 0; j < sizeY; j++)
      {
        for (Coordinate i = 0; i < sizeX; i++)                          // loop through all locations
        {                                                                 
          const Location l{ BoardPart::Main, i,j };
          const Piece* p = GetPiece(l);                                   
          if (p == nullptr) continue;                                     // field does not exist
          if ((p == &Piece::NoTile) || (p == &Piece::NoPiece)) continue;  // nothing here
          value += (p->IsColor(&Color::White) ? 1 : -1) * p->GetValue(*this,l);
        }
      }
    }
    //assert(Test::Test::TestValue(this));
  }


  MainPosition* MainPosition::GetPosition(AIContextP& plist, MoveP m) const // execute move, maintain in PList
  {
    MainPosition* pos(Clone());                                           // create a copy of the board
    if (m != nullptr) pos->Execute(*m);                                    // execute move if provided

    auto pl0 = plist->find(pos);                                           // check if we evaluated this position before
    if (pl0 != plist->end())
    {
      delete pos;
      return *pl0;                                                        // found - return it
    }

    pos->EvaluateStatically();                                            // evaluate position statically
    std::pair<std::unordered_set<MainPosition*>::iterator, bool> pl1 = plist->insert(pos);                                         // and save it
    assert(pl1.second);
    return *(pl1.first);                                                  // return the pointer to the new entry
  }

}
