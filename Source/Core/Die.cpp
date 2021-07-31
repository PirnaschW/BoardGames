#include "Core.h"

namespace BoardGamesCore
{

  constexpr Die::Die(Side s, unsigned int IDL, unsigned int IDS) noexcept : side_{ s }, bmpL_{ IDL }, bmpS_{ IDS } { }
  constexpr bool Die::operator ==(const Die& p) const noexcept { return p.side_ == side_; }
  constexpr bool Die::operator !=(const Die& p) const noexcept { return !(*this == p); }
  void Die::Serialize(Archive& ar) const { ar << side_; }
  void Die::Draw(DC* dc, const Rect& r) const { bmpL_.Draw(dc, r); }

  const std::array<Die, 6> Die::Sides{
    Die{ 1, IDB_S1L, IDB_S1S },
    Die{ 2, IDB_S2L, IDB_S2S },
    Die{ 3, IDB_S3L, IDB_S3S },
    Die{ 4, IDB_S4L, IDB_S4S },
    Die{ 5, IDB_S5L, IDB_S5S },
    Die{ 6, IDB_S6L, IDB_S6S },
  };

}