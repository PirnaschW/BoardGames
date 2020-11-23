#include "Core.h"

namespace BoardGamesCore
{
  static_assert(!std::is_abstract<Offset>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Offset>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Offset, int, int>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Offset, int, int>::value, "is not nothrow constructible");

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

  Offset::Offset(const Location& l1, const Location& l2) noexcept : dx_{ l2.x_ - l1.x_ }, dy_{ l2.y_ - l1.y_ } {}
  const Offset Offset::operator*(int i) const noexcept { return Offset(dx_ * i, dy_ * i); }


}
