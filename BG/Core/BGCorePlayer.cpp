#include "pch.h"

namespace BoardGamesCore
{

  constexpr const PlayerType PlayerType::Human{ 'H' };
  constexpr const PlayerType PlayerType::Computer{ 'C' };

  void PlayerType::Serialize(CArchive* ar) const { *ar << playertype_; }

}