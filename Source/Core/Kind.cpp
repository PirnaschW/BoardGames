#include "Core.h"
 
namespace BoardGamesCore
{

  void Kind::Serialize(Archive& ar) const { ar << kind_; }

  const noKind noKind::NoKind{};

}
