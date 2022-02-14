#include "Core.h"

namespace BoardGamesCore
{

  Action::Action(const Location l, const Piece& p) noexcept : l_{ l }, p_{ p } {}

  bool Action::operator == (const Action& a) const noexcept { return l_ == a.l_ && p_ == a.p_; };
  bool Action::operator != (const Action& a) const noexcept { return !(*this == a); }

  const Location& Action::GetLocation(void) const noexcept { return l_; }

  const Piece& Action::GetPiece(void) const noexcept { return p_; }

  bool Action::IsJump(void) const noexcept { return false; }


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
        const Location Loc1 = (*this)[i]->GetLocation();
        const Location Loc2 = (*this)[i - 1]->GetLocation();
        const Offset o0(Loc1, Loc2);
//        const Offset o0( (*this)[i]->GetLocation(), (*this)[i-1]->GetLocation() );
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


  ActionLift::ActionLift(const Location l, const Piece& p) noexcept : Action(l, p)
  {
    assert(p != Piece::NoTile);
    assert(p != Piece::NoPiece);
  }

  void ActionLift::Execute(MainPosition* p) const noexcept
  {
    assert(p->GetPiece(l_) == p_);                                        // verify the source field really has that piece
    if (l_.b_ != BoardPart::Stock)                                        // empty the source field (except in Stock)
      p->SetPiece(l_, Piece::NoPiece);
  }


  ActionJump::ActionJump(const Location l, const Piece& p) noexcept : Action(l, p) {}

  void ActionJump::Execute(MainPosition* p) const noexcept
  {
    assert(p->GetPiece(l_) == p_);                                        // nothing to do - if the jumped piece is affected, next action will handle it
  }


  ActionDrop::ActionDrop(const Location l, const Piece& p) noexcept : Action(l, p) {}

  void ActionDrop::Execute(MainPosition* p) const noexcept                // place piece on the target field
  {
    if (l_.b_ == BoardPart::Taken)                                        // Taken pieces will go in the next free spot
    {  
      for (Coordinate x = 0; ; x++)
      {
        const Location l{ BoardPart::Taken, x, l_.y_ };
        if (p->taken_.GetPiece(l).IsKind(noKind::NoKind))
        {
          p->SetPiece(l, p_);
          return;
        }
      }
    }
    else
    {
      assert(p->GetPiece(l_) == Piece::NoPiece);                          // target field must be free
      p->SetPiece(l_, p_);
    }
  }


  ActionEliminate::ActionEliminate(const Location l, const Piece& p) noexcept : Action(l, p) {}
  
  void ActionEliminate::Execute(MainPosition* p) const noexcept           // remove the target field from the board
  {                                                                       
    assert(l_.b_ == BoardPart::Main);                                     // only change the main board ever
    assert(p->GetPiece(l_) == Piece::NoPiece);                            // target field must be free?
    assert(p_ == Piece::NoTile);                                          // replacement must be 'NoTile'
    p->SetPiece(l_, p_);
  }

}