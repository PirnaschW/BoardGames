#include "Core.h"

#include <chrono>

namespace BoardGamesCore
{

  PositionValue AI::MakeMove(Board*& b)
  {
    // purge position buffer
    memory_.Purge([&b](const Board* p) -> bool {
      if (p->sequence_.size() < b->sequence_.size()) return true; // position has less moves, no longer of interest
      else
      {
        for (size_t i = 0; i < b->sequence_.size(); i++)
        {
          if (*(p->sequence_[i]) != *(b->sequence_[i])) return true;  // position has different first N moves, no longer of interest
        }
      }
      return false; // still of interest
      }
    );

    // evaluate position
    b = memory_.InsertOrUpdate(b);
    PositionValue v = Evaluate(b, AIMethod::BruteForce, 9, 4.0);

    // now execute best move:
    const Moves& m = b->GetMoveList(b->WhiteOnTurn());
    if (m.size() > 0)
    {
      b->Execute(m[0]);
      auto b0 = memory_.FindPos(std::hash<Board>()(*b));
      assert(b0);
      b = b0;
    }
    return v;
  }

  // Evaluate by chosen method
  PositionValue AI::Evaluate(Board* board, AIMethod method, Depth maxDepth, double timeLimit)
  {
    assert(board != nullptr);

    const auto t_start = std::chrono::high_resolution_clock::now();
    for (unsigned int plies = 0; plies <= maxDepth; plies++)   // use iterative deepening
    {
      try
      {
        PositionValue value_ = EvaluateDeeper(board, method, board->WhiteOnTurn(), PositionValue::PValueType::Lost, PositionValue::PValueType::Won, plies);
        if (value_.IsDecided()) return value_;
        if (board->GetMoveList(board->WhiteOnTurn()).size() == 1) return value_;          // if there is only one move, don't spend more time evaluating it
        if (memory_.GetSize() > 4'500'000) break;
        callback_(); // update screen
      }
      catch (std::bad_alloc)
      {
        throw;
      }

      const auto t_now = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsed = t_now - t_start;
      if (timeLimit > 0 && elapsed.count() > timeLimit) break;
    }
    return board->GetValue(board->WhiteOnTurn());
  }

  PositionValue AI::EvaluateDeeper(Board* board, AIMethod method, bool w, PositionValue alpha, PositionValue beta, unsigned int plies) noexcept
  {
    const PositionValue v0 = board->GetValue(w);
    if (plies == 0) return v0;
    if (v0.IsDecided()) return v0;   // don't analyze won or lost position any further

    if (plies > 2) callback_(); // update screen

    auto& movelist = board->GetMoveList(w);
    assert(!movelist.empty()); // otherwise, v0 must be Decided!

    PositionValue best = PositionValue::PValueType::Lost;

    bool KeepGoing = true;
    PositionValue returnvalue{ PositionValue::PValueType::Undefined };
    PositionValue tobesavedvalue{ PositionValue::PValueType::Undefined };

    size_t i = 0; // we need to know how far it went after the loop
    for (i = 0; i < movelist.size(); ++i)                          // for all possible opponent's moves
    {
      auto& m = movelist[i];
      Board* b = board->Clone();
      b->Execute(m);
      b = memory_.InsertOrUpdate(b);
      PositionValue v = -EvaluateDeeper(b, method, !w, -beta, -alpha, plies - 1);
      assert(v != PositionValue::PValueType::Undefined);
      assert(v == b->GetValue(w)); // or not???
      m->SetValue(v);                                                    // save position value into move for sorting
#ifdef LOG
      m->Log();
#endif
      switch (method)
      {
        case AIMethod::BruteForce:
          if (v > best) best = v;
          tobesavedvalue = returnvalue = best;
          break;
        case AIMethod::AlphaBetaPruning:
      // apply alpha/beta pruning
          if (v >= beta)                                                      // cut branch off, use current value
          {
            tobesavedvalue = v;
            returnvalue = beta;
            KeepGoing = false;
          }
          else if (v > alpha)
          {
            alpha = v;
            returnvalue = alpha;
          }                                       // reduce range for alpha, continue
          break;
      }
      if (!KeepGoing) break;
    }

    const auto l = [](MoveP a, MoveP b) { return *b < *a; };              // define sort predicate
    std::sort(movelist.begin(), movelist.begin() + i, l);                 // sort the moves by their value (for the next level of depth)
    board->SetValue(w, tobesavedvalue);                                   // save top value in current position
    board->SetDepth(plies);                                               // save evaluation depth
    return returnvalue;                                                   // return best value
  }

//    // Evaluate by chosen method
//  PositionValue Board::Evaluate(AIContext& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies) const noexcept
//  {
//    const PositionValue v0 = GetValue(w);
//    if (plies == 0) return v0;
//    if (PositionValue::PValueType::Normal != v0) return v0;               // don't analyze won or lost position any further
//
//    auto& movelist = GetMoveList(w);
//    if (movelist.empty()) return v0;
//
//    const auto l = [](MoveP a, MoveP b) { return *b < *a; };              // define sort predicate
//
//    for (size_t i = 0; i < movelist.size(); ++i)                          // for all possible opponent's moves
//    {
//      auto& m = movelist[i];
//      TBoardP tp = ExecuteMove(m);
//      Board* p = tp.FindInMap(plist);                                      // find the board in the list
//      p->GetAllMoves();                                                   // fill the move lists
//      p->EvaluateStatically();
//      PositionValue v = -p->Evaluate(plist, !w, -beta, -alpha, plies - 1);// evaluate the result
//      assert(v != PositionValue::PValueType::Undefined);
//      m->SetValue(p->GetValue(w));                                        // save real position value into move for sorting
//#ifdef LOG
//      m->Log();
//#endif
//
//      // apply alpha/beta pruning
//      if (v >= beta)                                                      // cut branch off, use current value
//      {
//        std::sort(movelist.begin(), movelist.begin() + i + 1, l);         // sort the moves by their value (for the next level of depth)
//        SetValue(w, v);                                                   // save top value in current position
//        depth_ = plies;                                                   // save evaluation depth
//        return beta;
//      }
//      if (v > alpha) { alpha = v; }                                       // reduce range for alpha, continue
//    }
//
//    std::sort(movelist.begin(), movelist.end(), l);                       // sort the moves by their value (for the next level of depth)
//    SetValue(w, alpha);                                                   // save top value in current position
//    depth_ = plies;                                                       // save evaluation depth
//    return alpha;                                                         // return best value
//  }

//  // Evaluate by Alpha-Beta-Pruning ('AB'):
//  PositionValue Board::EvaluateAB(AIContext& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies) const noexcept
//  {
//    const PositionValue v0 = GetValue(w);
//    if (plies == 0) return v0;
//    if (PositionValue::PValueType::Normal != v0) return v0;               // don't analyze won or lost position any further
//
//    auto& movelist = GetMoveList(w);
//    if (movelist.empty()) return v0;
//
//    PositionValue best = PositionValue::PValueType::Lost;
//
//    bool KeepGoing = true;
//    PositionValue returnvalue{ PositionValue::PValueType::Undefined };
//    PositionValue tobesavedvalue{ PositionValue::PValueType::Undefined };
//
//    size_t i = 0; // we need to know how far it went after the loop
//    for (i = 0; i < movelist.size(); ++i)                          // for all possible opponent's moves
//    {
//      auto& m = movelist[i];
//      TBoardP tp = ExecuteMove(m);
//      Board* p = tp.FindInMap(plist);                                      // find the board in the list
//      p->GetAllMoves();                                                   // fill the move lists
//      p->EvaluateStatically();
//      PositionValue v = -p->EvaluateAB(plist, !w, -beta, -alpha, plies - 1);// evaluate the result
//      assert(v != PositionValue::PValueType::Undefined);
//      assert(v == p->GetValue(w)); // or not???
//      m->SetValue(v);                                                    // save position value into move for sorting
//#ifdef LOG
//      m->Log();
//#endif
//      switch (plist.GetAIMethod())
//      {
//        case AIMethod::AlphaBetaPruning:
//      // apply alpha/beta pruning
//          if (v >= beta)                                                      // cut branch off, use current value
//          {
//            tobesavedvalue = v;
//            returnvalue = beta;
//            KeepGoing = false;
//          }
//          else if (v > alpha)
//          {
//            alpha = v;
//            returnvalue = alpha;
//          }                                       // reduce range for alpha, continue
//          break;
//        case AIMethod::BruteForce:
//          if (v > best) best = v;
//          tobesavedvalue = returnvalue = best;
//          break;
//      }
//      if (!KeepGoing) break;
//    }
//
//    const auto l = [](MoveP a, MoveP b) { return *b < *a; };              // define sort predicate
//    std::sort(movelist.begin(), movelist.begin() + i, l);                 // sort the moves by their value (for the next level of depth)
//    SetValue(w, tobesavedvalue);                                          // save top value in current position
//    depth_ = plies;                                                       // save evaluation depth
//    return returnvalue;                                                   // return best value
//  }

//  // Evaluate by Brute-Force ('BF'):
//  PositionValue Board::EvaluateBF(AIContext& plist, bool w, unsigned int plies) const noexcept
//  {
//    const PositionValue v0 = GetValue(w);
//    if (plies == 0) return v0;
//    if (PositionValue::PValueType::Normal != v0) return v0;  // don't analyze won or lost position any further
//
//    auto& movelist = GetMoveList(w);
//    if (movelist.empty()) return v0;
//
//    const auto l = [](MoveP a, MoveP b) { return *b < *a; };              // define sort predicate
//
//    PositionValue best = PositionValue::PValueType::Lost;
//    for (auto& m : movelist)                                              // for all possible opponent's moves
//    {
//      TBoardP tp = ExecuteMove(m);
//      Board* p = tp.FindInMap(plist);                                     // find the board in the list
//      p->GetAllMoves();                                                   // fill the move lists
//      p->EvaluateStatically();
//      PositionValue v = -p->EvaluateBF(plist, !w, plies - 1);             // evaluate the result
//      assert(v != PositionValue::PValueType::Undefined);
//      assert(v == p->GetValue(w)); // or noy???
//      m->SetValue(p->GetValue(w));                                        // save real position value into move for sorting
//      if (v > best) best = v;
//#ifdef LOG
//      m->Log();
//#endif
//
//      //wchar_t buffer[256];
//      //const Location& l{ m->GetActions()[1]->GetLocation() };
//      //wsprintfW(buffer, L"Depth: %1d - Move %d (%d) has Value:%10.10S\n", plies, ++z, l.x_, (const char*)p->GetValue(w));
//      //OutputDebugString(buffer);
//
//    }
//
//    std::sort(movelist.begin(), movelist.end(), l);                       // sort the moves by their value (for the next level of depth)
//    SetValue(w, best);                                                    // save best value in current position
//    depth_ = plies;                                                       // save evaluation depth
//    return best;                                                          // return best value
//  }

}