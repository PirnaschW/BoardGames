#include "stdafx.h"

namespace BoardGamesCore
{

  void ActionTake::Execute(MainPosition* p) const noexcept { assert(p->GetPiece(_l) == _p); if (_l._b != BoardPart::Stock) p->SetPiece(_l, &Piece::NoPiece); }     // empty the source field (except in stock)

  void ActionJump::Execute(MainPosition* p) const noexcept { assert(p->GetPiece(_l) == _p); p->SetPiece(_l, &Piece::NoPiece); }     // empty the jumped field

  void ActionPlace::Execute(MainPosition* p) const noexcept { assert(p->GetPiece(_l) == &Piece::NoPiece); p->SetPiece(_l, _p); }    // place piece on the the target field

}