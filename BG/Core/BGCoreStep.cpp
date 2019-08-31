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


  bool Actions::IsRepeat(const Piece* p, const Location fr, const Location to) const noexcept
  {
    bool first{ true };            // flag: we are looking for the first part of a move
    Location l1{ fr };             // starting point of the move
    Location lj{ fr._b,(fr._x + to._x) / 2U,(fr._y + to._y) / 2U };

    // loop through the actions, first find a Take of p, then a Place of p, then compare to given move
    for (const auto& aa : *this)
    {
      if (first)  // looking for first?
      {
        // check for a Take of p
        if (aa->IsTake() && aa->GetPiece() == p)
        {
          // save it
          l1 = aa->GetLocation();
          first = false;
        }
      }
      else        // looking for second
      {
        // check for a previous Jump over same location
        if (aa->IsTake() && aa->GetLocation() == lj) return true;

        // check for a Place of p
        if (!aa->IsTake() && aa->GetPiece() == p)
        {
          const Location l2 = aa->GetLocation();
          // compare forward and backward
          if ((l1 == fr && l2 == to) || (l2 == fr && l1 == to)) return true;

          first = true; // go on and look for another first
        }
      }
    }
    return false;
  }

}