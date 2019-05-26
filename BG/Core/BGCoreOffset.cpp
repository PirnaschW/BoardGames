#include "stdafx.h"

namespace BoardGamesCore
{

  // standard 4 'Rook'   directions
  constexpr const Offset Offset::Rdirection[4]{ Offset(+1, +0), Offset(+0, +1),
                                                Offset(+0, -1), Offset(-1, +0) };
  // standard 4 'Bishop' directions
  constexpr const Offset Offset::Bdirection[4]{ Offset(+1, +1), Offset(+1, -1),
                                                Offset(-1, -1), Offset(-1, +1) };
  // standard 8 'Queen'  directions
  constexpr const Offset Offset::Qdirection[8]{ Offset(+1, +1), Offset(+1, +0), Offset(+1, -1),
                                                Offset(+0, +1),                 Offset(+0, -1),
                                                Offset(-1, +1), Offset(-1, +0), Offset(-1, -1) };

}
