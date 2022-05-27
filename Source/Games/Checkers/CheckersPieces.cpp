#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{
  const Checker     Checker    ::TheChecker    {};
  const King        King       ::TheKing       {};
  const Queen       Queen      ::TheQueen      {};
  const Para        Para       ::ThePara       {};


  void Checker::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersBoard& board_ = down_cast<const CheckersBoard&>(p);
    const int dy = PMap[board_.GetPieceIndex(l.x_,l.y_)].IsColor(PieceColor::White) ? -1 : 1;
    board_.AddIfLegal(moves, l, l + Offset(1, dy));                          // check for slide moves
    board_.AddIfLegal(moves, l, l + Offset(-1, dy));
    board_.AddIfLegalJump(moves, false, Actions{}, board_.GetPieceIndex(l.x_, l.y_), l);        // check for jump moves
  }


  void King::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersBoard& board_ = down_cast<const CheckersBoard&>(p);
    for (auto& d : Offset::BDirection)
      board_.AddIfLegal(moves, l, l + d);                                    // check for slide moves
    //board_.AddIfLegalJump(moves, false, Actions{}, board_.GetPieceIndex(l.x_, l.y_), l);        // check for jump moves
  }


  void Queen::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersBoard& board_ = down_cast<const CheckersBoard&>(p);
    for (auto& d : Offset::BDirection)
      for (int z = 1; board_.AddIfLegal(moves, l, l + d * z); z++);          // check for slide moves
    //board_.AddIfLegalJump(moves, true, Actions{}, board_.GetPieceIndex(l.x_, l.y_), l);         // check for jump moves
  }


  void Para::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersBoard& board_ = down_cast<const CheckersBoard&>(p);
    //const int dy = board_.GetPieceIndex(l.x_, l.y_).IsColor(PieceColor::White) ? -1 : 1;
    //board_.AddIfLegalJump(moves, false, Actions{}, board_.GetPieceIndex(l.x_, l.y_), l);        // check only for (back-)jump moves
  }

}
