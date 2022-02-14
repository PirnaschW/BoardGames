#include "Core.h"

#include <chrono>
#include <algorithm>

namespace BoardGamesCore
{
  
  void AIContext::Purge(const Moves& sequence_) noexcept
  {
    static int dummy{};
    dummy++;

    auto l = [&sequence_] (const MainPosition* p) -> bool
    {
      if (p->sequence_.size() < sequence_.size()) return true;
      else
      {
        for (size_t i = 0; i < sequence_.size(); i++)
        {
          if (*(p->sequence_[i]) != *sequence_[i]) return true;
        }
      }
    };

    for (auto it = begin(); it != end();)
    {
      bool trash{ false };
      if ((*it)->sequence_.size() < sequence_.size()) trash = true;
      else
      {
        for (size_t i = 0; i < sequence_.size(); i++)
        {
          if (*(*it)->sequence_[i] != *sequence_[i])
          {
            trash = true;
            break;
          }
        }
      }

      //MainPosition* p{ *it };
      //++it;
      //if (trash)
      //{
      //  erase(p);
      //  delete p;
      //}

      if (trash)
      {
        const MainPosition* p{ *it };
        it = erase(it);
        delete p;
      }
      else it++;

    }

//    std::unordered_set< MainPosition*, AIContextHelper::Hash, AIContextHelper::Equality> pp{};
//    std::swap(*this, pp);

    dummy++;
  }

  bool Game::AIMove(void)
  {
    // cleanup position buffer
    plist.Purge(pos->sequence_);  // Positions with less than the current amount of moves can be discarded, they will not be needed any more

    MainPosition* p{ pos->GetPosition(plist) };                           // retrieve position from list
    assert(p != nullptr);

    const auto t_start = std::chrono::high_resolution_clock::now();
    const double limit = 4.0;  // run for n seconds
    bool w = CurrentPlayer()->GetColor() == PieceColor::White;

    for (unsigned int pl = 0; /*pl <= 4*/; pl++)                               // use iterative deepening
    {
      try
      {
        PositionValue value_ = p->Evaluate(plist, w, PositionValue::PValueType::Lost, PositionValue::PValueType::Won, pl);
//      PositionValue value_ = p->EvaluateBF(plist, w, pl);
        pos->SetValue(w, value_);
        pos->SetDepth(pl);
        plist.callback();
        if (value_ == PositionValue::PValueType::Lost)
        {
          BoardGamesMFC::AfxMessageBox(L"Computer resigns - Player wins!");
          _mode.Set(Mode::GameOver);
          return false;
        }
        if (value_ == PositionValue::PValueType::Won) {
          BoardGamesMFC::AfxMessageBox(L"You might as well resign - Computer will win!");
          break;
        }

        if (value_ == PositionValue::PValueType::Tie) {
          BoardGamesMFC::AfxMessageBox(L"Computer will hold a Draw.");
          break;
        }
      }
      catch (std::bad_alloc)
      {
        throw;
      }

      if (plist.size() > 4500000)
        break;

      const auto t_now = std::chrono::high_resolution_clock::now();
      const std::chrono::duration<double> elapsed = t_now - t_start;
      if (elapsed.count() > limit) break;
      if (p->GetMoveList(CurrentPlayer()->GetColor() == PieceColor::White).size() == 1) break;          // if there is only one move, don't spend time evaluating it
//      plist->callback();
    }

    Execute(*(p->GetBestMove(CurrentPlayer()->GetColor() == PieceColor::White)));
    return true;
  }


  PositionValue MainPosition::Evaluate(AIContext& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies) const noexcept
  {
    const PositionValue v0 = GetValue(w);
    if (plies == 0) return v0;
    if (PositionValue::PValueType::Normal != v0) return v0;               // don't analyze won or lost position any further

    auto& movelist = GetMoveList(w);
    if (movelist.empty()) return v0;

    const auto l = [](MoveP a, MoveP b) { return *b < *a; };              // define sort predicate

    for (int i = 0; i < movelist.size(); ++i)                             // for all possible opponent's moves
    {
      auto& m = movelist[i];
      MainPosition* p{ GetPosition(plist,m) };                            // find the board in the list
      PositionValue v = -p->Evaluate(plist, !w, -beta, -alpha, plies - 1);// evaluate the result
      assert(v != PositionValue::PValueType::Undefined);
      m->SetValue(p->GetValue(w));                                        // save real position value into move for sorting
#ifdef LOG
      m->Log();
#endif

      // apply alpha/beta pruning
      if (v >= beta)                                                      // cut branch off, use current value
      {
        std::sort(movelist.begin(), movelist.begin() + i + 1, l);         // sort the moves by their value (for the next level of depth)
        SetValue(w, v);                                                   // save top value in current position
        depth_ = plies;                                                   // save evaluation depth
        return beta;
      }
      if (v > alpha) { alpha = v; }                                       // reduce range for alpha, continue
    }

    std::sort(movelist.begin(), movelist.end(), l);                       // sort the moves by their value (for the next level of depth)
    SetValue(w, alpha);                                                   // save top value in current position
    depth_ = plies;                                                       // save evaluation depth
    return alpha;                                                         // return best value
  }


  PositionValue MainPosition::EvaluateBF(AIContext& plist, bool w, unsigned int plies) const noexcept
  {
    const PositionValue v0 = GetValue(w);
    if (plies == 0) return v0;
    if (PositionValue::PValueType::Normal != v0) return v0;  // don't analyze won or lost position any further

    auto& movelist = GetMoveList(w);
    if (movelist.empty()) return v0;

    PositionValue best = PositionValue::PValueType::Lost;
    for (auto& m : movelist)                                              // for all possible opponent's moves
    {
      MainPosition* p{ GetPosition(plist,m) };                            // find the board in the list
      PositionValue v = -p->EvaluateBF(plist, !w, plies - 1);             // evaluate the result
      assert(v != PositionValue::PValueType::Undefined);
      m->SetValue(v);                                                     // save position value into move for sorting
      if (v > best) best = v;
#ifdef LOG
      m->Log();
#endif

      //wchar_t buffer[256];
      //const Location& l{ m->GetActions()[1]->GetLocation() };
      //wsprintfW(buffer, L"Depth: %1d - Move %d (%d) has Value:%10.10S\n", plies, ++z, l.x_, (const char*)p->GetValue(w));
      //OutputDebugString(buffer);

    }

    const auto l = [](MoveP a, MoveP b) { return *b < *a; };              // define sort predicate
    std::sort(movelist.begin(), movelist.end(), l);                       // sort the moves by their value (for the next level of depth)
    SetValue(w, best);                                                    // save best value in current position
    return best;                                                          // return best value
  }


  [[ nodiscard ]] PositionValue MainPosition::EvaluateStatically(void) const noexcept   // as seen from White
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    // default evaluation: count all material, and add difference of move count. Overwrite for each game as needed
    GetAllMoves();                                                             // fill the move lists
    depth_ = 1;
    if (onTurn_ == &PieceColor::White && movesW_.empty()) return PositionValue::PValueType::Lost;     // if no more moves, game over
    if (onTurn_ == &PieceColor::Black && movesB_.empty()) return PositionValue::PValueType::Won;
    PositionValue v{ GetMoveCountFactor() * (movesW_.size() - movesB_.size()) };
    for (Coordinate j = 0; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)                                // loop through all locations
      {
        const Location l{ BoardPart::Main, i,j };
        const Piece& p = GetPiece(l);
        if ((p == Piece::NoTile) || (p == Piece::NoPiece)) continue;         // nothing here
        v += (p.IsColor(PieceColor::White) ? 1 : -1) * p.GetValue(*this, l);
      }
    }
    return v;
  }

  [[ nodiscard ]] PositionValue MainPosition::EvaluateChainLengths(unsigned int towin) const noexcept           // as seen from White
  {
    PositionValue v{ 0 };
    for (Coordinate j = 0; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)                                                      // loop through all locations
      {                                                                       
        PositionValue vv = EvaluateChainLength(Location(BoardPart::Main, i, j), towin);
        if (vv != PositionValue::PValueType::Normal) return vv;
        v += vv;
      }
    }

    return v;
  }

  [[ nodiscard ]] PositionValue MainPosition::EvaluateChainLength(Location l, unsigned int towin) const noexcept    // as seen from White
  {
    int v1{ 0 };
    int v2{ 0 };

    const Piece* p = &GetPiece(l);
    if (p->IsColor(PieceColor::NoColor)) return 0;                                       // nothing here, so no chain can start
    const bool w = p->IsColor(PieceColor::White);                                        
    const int n = sizeof(Offset::QDirection) / sizeof(Offset::QDirection[0]) / 2;        
    for (int i = 0; i < n; ++i)                                                          
    {                                                                                    
      int bonus = 0;                                                                     
      const Offset& d = Offset::QDirection[i];                                           
      const Piece* pp{ &GetPiece(l + d * -1) };                                          
      if (pp != &Piece::NoTile && pp->IsColor(p->GetColor())) continue;                  // if same color is that direction, we counted it already, so move on
      if (pp != &Piece::NoTile && pp->IsBlank()) ++bonus;                                // free field, give extra points - much better than opponent's piece
      Location ll{ l };
//      Location ll{ BoardPart::Main,i,j };
      unsigned int z{ 1 };
      while ((pp = &GetPiece(ll += d)) != nullptr)
      {
        if (pp->IsColor(p->GetColor())) z++;
        else
        {
          if (pp->IsColor(PieceColor::NoColor)) ++bonus;                                 // if line ends with free field, give an extra point - much better than opponent's piece
          break;
        }
      }
      if (z >= towin) return w ? PositionValue::PValueType::Won : PositionValue::PValueType::Lost;
      (w ? v1 : v2) += GetChainValue(z) + bonus * GetChainValue(z-1);
    }
    return v1 - v2;
  }

  unsigned int MainPosition::GetChainValue(unsigned int z) const noexcept
  {
    switch (z)
    {
      case 0:  return          1U;
      case 1:  return         10U;
      case 2:  return        100U;
      case 3:  return       1000U;
      case 4:  return      10000U;
      case 5:  return     100000U;
      case 6:  return    1000000U;
      case 7:  return   10000000U;
      case 8:  return  100000000U;
      case 9:  return 1000000000U;
      default: return          0U;
    }                             
  }


  MainPosition* MainPosition::GetPosition(AIContext& plist, MoveP m) const noexcept                // execute move, maintain resulting pos in PList
  {
    MainPosition* pos(Clone());                                                                    // create a copy of the board
    if (m != nullptr) pos->Execute(*m);                                                            // execute move if provided
                                                                                                   
    auto pl0 = plist.find(pos);                                                                    // check if we evaluated this position before
    if (pl0 != plist.end())                                                                        
    {                                                                                              
      delete pos;                                                                                  
      return *pl0;                                                                                 // found - return it
    }

    pos->SetValue(true,pos->EvaluateStatically());                                                 // evaluate position statically
    std::pair<std::unordered_set<MainPosition*>::iterator, bool> pl1 = plist.insert(pos);          // and save it
    assert(pl1.second);
    return *(pl1.first);                                                                           // return the pointer to the new entry
  }

}
