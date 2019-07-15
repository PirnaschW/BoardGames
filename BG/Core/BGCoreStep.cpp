#include "stdafx.h"

namespace BoardGamesCore
{
  void ActionTake::Execute(MainPosition* p) const noexcept
  {
    assert(p->GetPiece(_l) == _p);
    if (_l._b != BoardPart::Stock)                                        // empty the source field (except in stock)
      p->SetPiece(_l, &Piece::NoPiece);
  }

  void ActionJump::Execute(MainPosition* p) const noexcept
  {
    assert(p->GetPiece(_l) == _p);
    p->SetPiece(_l, &Piece::NoPiece);                                     // empty the jumped field
  }

  void ActionPlace::Execute(MainPosition* p) const noexcept               // place piece on the the target field
  {
    if (_l._b == BoardPart::Taken)
    {  // Taken pieces will go in the next free spot
      for (Coordinate x = 0; ; x++)
      {
        const Location l{ BoardPart::Taken, x, _l._y };
        if (p->_taken.GetPiece(l)->IsKind(noKind::NoKind))
        {
          p->SetPiece(l, _p);
          return;
        }
      }
      assert(1 == 0); // must not happen - Taken is full??
    }
    else
    {
      assert(p->GetPiece(_l) == &Piece::NoPiece);
      p->SetPiece(_l, _p);
    }
  }

}