#include "pch.h"

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


  Offset::Offset(const Location& l1, const Location& l2) noexcept : dx_ { l2.x_ - l1.x_ }, dy_{ l2.y_ - l1.y_ } {}

}
