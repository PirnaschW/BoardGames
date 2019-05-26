#include "stdafx.h"

namespace BoardGamesCore
{
  void Kind::Serialize(CArchive* ar) const { *ar << kind; }

  const noKind noKind::NoKind{};

}
