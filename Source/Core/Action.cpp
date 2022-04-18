#include "Core.h"

namespace BoardGamesCore
{

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

  void ActionLift::Execute(Board* p) const noexcept
  {
    assert(p->GetPieceIndex(l_.x_, l_.y_, l_.b_) == pI_);                         // verify the source field really has that piece
    if (l_.b_ != BoardPartID::Stock)                                        // empty the source field (except in Stock)
      p->SetPieceIndex(PMap[Piece::NoPiece], l_.x_, l_.y_, l_.b_);
  }


  void ActionJump::Execute(Board* p) const noexcept
  {
    assert(p->GetPieceIndex(l_.x_, l_.y_, l_.b_) == pI_);                     // nothing to do - if the jumped piece is affected, next action will handle it
  }


  void ActionDrop::Execute(Board* p) const noexcept                // place piece on the target field
  {
    if (l_.b_ == BoardPartID::Taken)                                        // Taken pieces will go in the next free spot
    {
      for (Coordinate x = 0; ; x++)
      {
        if (p->GetPieceIndex(x, l_.y_, BoardPartID::Taken) == PMap[Piece::NoPiece])
        {
          p->SetPieceIndex(pI_, x, l_.y_, BoardPartID::Taken);
          return;
        }
      }
    }
    else
    {
      assert(p->GetPieceIndex(l_.x_, l_.y_, l_.b_) == PMap[Piece::NoPiece]);           // target field must be free
      assert(pI_ != PMap[Piece::NoPiece]);                                    // don't drop 'nothing'
      p->SetPieceIndex(pI_, l_.x_, l_.y_, l_.b_);
    }
  }


  void ActionEliminate::Execute(Board* p) const noexcept           // remove the target field from the board
  {
    assert(l_.b_ == BoardPartID::Stage);                                   // only change the main board ever
    assert(p->GetPieceIndex(l_.x_, l_.y_) == PMap[Piece::NoPiece]);                  // target field must be free?
    assert(pI_ == PMap[Piece::NoTile]);                                          // replacement must be 'NoTile'
    p->SetPieceIndex(pI_, l_.x_, l_.y_);
  }

}