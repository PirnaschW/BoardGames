#include "Core.h"

namespace BoardGamesCore
{

  Piece::Piece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : kind_{ k }, color_{ c }, bmp_{ ID } { }

  bool Piece::operator ==(const Piece& p) const noexcept { return p.IsKind(kind_) && p.IsColor(color_); }
  bool Piece::operator !=(const Piece& p) const noexcept { return !(*this == p); }

  const Piece Piece::NoTile { noKind::NoKind, PieceColor::Void,    0 };   // nothing exists there, not even a tile
  const Piece Piece::NoPiece{ noKind::NoKind, PieceColor::NoColor, 0 };   // empty tile

  void Piece::CollectMoves(const MainPosition& p, const Location& l, Moves& m) const
  {
    kind_.CollectMoves(p, l, m);
  }

  size_t Piece::GetHash(void) const noexcept
  { 
    return kind_.GetHash() + color_.GetHash();
  }
  
  bool Piece::IsKind(const Kind& k) const noexcept
  { 
    return k == kind_;
  }
  bool Piece::IsColor(const PieceColor& c) const noexcept
  { 
    return c == color_;
  }

  bool Piece::IsBlank(void) const noexcept
  {
    return color_ == PieceColor::NoColor && kind_ == noKind::NoKind;
  }

  void Piece::Serialize(Archive& ar) const
  { 
    color_.Serialize(ar);
    kind_.Serialize(ar);
  }
  
  unsigned int Piece::GetValue(const MainPosition& p, const Location& l) const noexcept
  { 
    return kind_.GetValue(p, l);
  }

  bool Piece::CanTake(const Piece& p) const noexcept
  {
    return kind_.CanTake(p.kind_);
  }

  void Piece::Draw(DC* dc, const Rect& r) const
  {
    bmp_.Draw(dc, r);
  }

}