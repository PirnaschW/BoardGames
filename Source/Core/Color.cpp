#include "Core.h"

namespace BoardGamesCore
{

  void PieceColor::Serialize(Archive& ar) const { ar << color_; }

  constexpr const PieceColor PieceColor::Void   { 'X' };
  constexpr const PieceColor PieceColor::NoColor{ ' ' };
  constexpr const PieceColor PieceColor::White  { 'W' };
  constexpr const PieceColor PieceColor::Black  { 'B' };

  
  const TileColor TileColor::Light { 'L', IDB_XXL };  // normal size light tile
  const TileColor TileColor::Dark  { 'D', IDB_XXD };  // normal size dark  tile
  const TileColor TileColor::Small { 'l', IDB_XXS };  // small  size light tile

  void TileColor::Serialize(Archive& ar) const { ar << tilecolor_; }

  void TileColor::Draw(DC* pDC, const Rect& r) const { cb_.Draw(pDC, r); }

}
