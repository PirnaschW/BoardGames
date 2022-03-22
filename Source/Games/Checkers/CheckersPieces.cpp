#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{
  const Checker     Checker    ::TheChecker    {};
  const King        King       ::TheKing       {};
  const Queen       Queen      ::TheQueen      {};
  const Para        Para       ::ThePara       {};

  const CheckersPiece CheckersPiece::CheckersPieceW    { Checker    ::TheChecker,     PieceColor::White, &CheckersQueenW, IDB_WPL };
  const CheckersPiece CheckersPiece::CheckersPieceB    { Checker    ::TheChecker,     PieceColor::Black, &CheckersQueenB, IDB_BPL };
  const CheckersPiece CheckersPiece::CheckersKingW     { King       ::TheKing,        PieceColor::White, &CheckersKingW,  IDB_WKL };
  const CheckersPiece CheckersPiece::CheckersKingB     { King       ::TheKing,        PieceColor::Black, &CheckersKingB,  IDB_BKL };
  const CheckersPiece CheckersPiece::CheckersQueenW    { Queen      ::TheQueen,       PieceColor::White, &CheckersQueenW, IDB_WQL };
  const CheckersPiece CheckersPiece::CheckersQueenB    { Queen      ::TheQueen,       PieceColor::Black, &CheckersQueenB, IDB_BQL };
  const CheckersPiece CheckersPiece::CheckersParaW     { Para       ::ThePara,        PieceColor::White, &CheckersPieceW, IDB_CHECKERSWPARA };
  const CheckersPiece CheckersPiece::CheckersParaB     { Para       ::ThePara,        PieceColor::Black, &CheckersPieceB, IDB_CHECKERSBPARA };


  void Checker::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    const int dy = pos.GetPiece(l).IsColor(PieceColor::White) ? -1 : 1;
    pos.AddIfLegal(moves, l, l + Offset(1, dy));                          // check for slide moves
    pos.AddIfLegal(moves, l, l + Offset(-1, dy));
    pos.AddIfLegalJump(moves, false, Actions{}, pos.GetPiece(l), l);        // check for jump moves
  }


  void King::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    for (auto& d : Offset::BDirection)
      pos.AddIfLegal(moves, l, l + d);                                    // check for slide moves
    pos.AddIfLegalJump(moves, false, Actions{}, pos.GetPiece(l), l);        // check for jump moves
  }


  void Queen::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    for (auto& d : Offset::BDirection)
      for (int z = 1; pos.AddIfLegal(moves, l, l + d * z); z++);          // check for slide moves
    pos.AddIfLegalJump(moves, true, Actions{}, pos.GetPiece(l), l);         // check for jump moves
  }


  void Para::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const CheckersPosition& pos = dynamic_cast<const CheckersPosition&>(p);
    const int dy = pos.GetPiece(l).IsColor(PieceColor::White) ? -1 : 1;
    pos.AddIfLegalJump(moves, false, Actions{}, pos.GetPiece(l), l);        // check only for (back-)jump moves
  }

}
