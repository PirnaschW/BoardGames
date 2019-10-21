#include "stdafx.h"

namespace BoardGamesCore
{

  void Color::Serialize(CArchive* ar) const { *ar << color_; }

  constexpr const Color Color::Void   { 'X' };
  constexpr const Color Color::NoColor{ ' ' };
  constexpr const Color Color::White  { 'W' };
  constexpr const Color Color::Black  { 'B' };

  void TileColor::Serialize(CArchive* ar) const { *ar << tilecolor_; }

  constexpr const TileColor TileColor::Light{ 'L' };
  constexpr const TileColor TileColor::Dark { 'D' };
  constexpr const TileColor TileColor::Small{ 'l' };

}
