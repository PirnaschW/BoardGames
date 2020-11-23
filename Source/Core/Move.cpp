#include "Core.h"

namespace BoardGamesCore
{

  Move::Move(const Actions& a) noexcept : a_(a) {}
  Move::~Move(void) noexcept {}
  bool Move::operator==(const Move& m) const noexcept { return a_ == m.a_; }
  bool Move::operator!=(const Move& m) const noexcept { return !(*this == m); }
  const Actions& Move::GetActions(void) const noexcept { return a_; }
  const Location& Move::GetFrL(void) const noexcept { return a_.front()->GetLocation(); }
  const Location& Move::GetToL(void) const noexcept { return a_.back()->GetLocation(); }

}
