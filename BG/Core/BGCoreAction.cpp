#include "stdafx.h"

namespace BoardGamesCore
{

  bool Actions::operator == (const Actions& a) const noexcept
  {
    if (size() != a.size()) return false;
    for (unsigned int i = 0U; i < a.size(); ++i)
    {
      if (*(*this)[i] != *a[i]) return false;
    }
    return true;
  }

  bool Actions::IsRepeat(const Piece* p, const Location fr, const Location to) const noexcept
  {
    bool first{ true };            // flag: we are looking for the first part of a move
    Location l1{ fr };             // starting point of the move
    Location lj{ fr.b_,(fr.x_ + to.x_) / 2U,(fr.y_ + to.y_) / 2U };

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


  ActionTake::ActionTake(const Location& l, const Piece* p) noexcept : Action(l, p)
  {
    assert(p != nullptr); assert(p != &Piece::NoPiece);
  }

  void ActionTake::Execute(MainPosition* p) const noexcept
  {
    assert(p->GetPiece(l_) == p_);
    if (l_.b_ != BoardPart::Stock)                                        // empty the source field (except in stock)
      p->SetPiece(l_, &Piece::NoPiece);
  }

  void ActionJump::Execute(MainPosition* p) const noexcept
  {
    assert(p->GetPiece(l_) == p_);
    p->SetPiece(l_, &Piece::NoPiece);                                     // empty the jumped field
  }

  void ActionPlace::Execute(MainPosition* p) const noexcept               // place piece on the the target field
  {
    if (l_.b_ == BoardPart::Taken)
    {  // Taken pieces will go in the next free spot
      for (Coordinate x = 0; ; x++)
      {
        const Location l{ BoardPart::Taken, x, l_.y_ };
        if (p->_taken.GetPiece(l)->IsKind(noKind::NoKind))
        {
          p->SetPiece(l, p_);
          return;
        }
      }
      assert(1 == 0); // must not happen - Taken is full??
    }
    else
    {
      assert(p->GetPiece(l_) == &Piece::NoPiece);
      p->SetPiece(l_, p_);
    }
  }

}