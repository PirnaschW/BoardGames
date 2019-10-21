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

  bool Actions::IsRepeat(const Location& l, const Offset& o) const noexcept
  {
    // loop through the actions, find a Jump of l, then before it must be a Take, then compare the diff to given direction
    for (unsigned int i = 1; i < this->size(); ++i)
    {
      if ((*this)[i]->IsJump() && (*this)[i]->GetLocation() == l)  // jumping over the given location
      {
        const Offset o0{ (*this)[i]->GetLocation(), (*this)[i-1]->GetLocation() };
        if (o0.IsParallel(o)) return true;
      }
    }
    return false;
  }

  bool Actions::HasJump(void) const noexcept
  {
    auto AhasJump = [](const ActionP& a) -> bool { return a->IsJump(); };
    return std::find_if(this->cbegin(), this->cend(), AhasJump) != this->cend();
  }


  ActionTake::ActionTake(const Location& l, const Piece* p) noexcept : Action(l, p)
  {
    assert(p != nullptr);
    assert(p != &Piece::NoTile);
    assert(p != &Piece::NoPiece);
  }

  void ActionTake::Execute(MainPosition* p) const noexcept
  {
    assert(p->GetPiece(l_) == p_);                                        // verify the source field really has that piece
    if (l_.b_ != BoardPart::Stock)                                        // empty the source field (except in Stock)
      p->SetPiece(l_, &Piece::NoPiece);
  }

  void ActionJump::Execute(MainPosition* p) const noexcept
  {
    assert(p->GetPiece(l_) == p_);                                        // nothing to do - if the jumped piece is affected, next action will handle it
  }

  void ActionPlace::Execute(MainPosition* p) const noexcept               // place piece on the the target field
  {
    if (l_.b_ == BoardPart::Taken)                                        // Taken pieces will go in the next free spot
    {  
      for (Coordinate x = 0; ; x++)
      {
        const Location l{ BoardPart::Taken, x, l_.y_ };
        if (p->taken_.GetPiece(l)->IsKind(noKind::NoKind))
        {
          p->SetPiece(l, p_);
          return;
        }
      }
      assert(1 == 0); // must not happen - Taken is full??
    }
    else
    {
      assert(p->GetPiece(l_) == &Piece::NoPiece);                         // target field must be free
      p->SetPiece(l_, p_);
    }
  }

}