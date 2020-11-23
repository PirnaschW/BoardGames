#include "Core.h"

namespace BoardGamesCore
{

  constexpr const PlayerType PlayerType::Human{ 'H' };
  constexpr const PlayerType PlayerType::Computer{ 'C' };

  void PlayerType::Serialize(Archive& ar) const { ar << playertype_; }

}