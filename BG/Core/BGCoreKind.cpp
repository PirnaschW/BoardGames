#include "stdafx.h"

namespace BoardGamesCore
{
  void Kind::Serialize(CArchive* ar) const { *ar << kind_; }

  const noKind noKind::NoKind{};

}
