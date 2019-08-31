#include "stdafx.h"

namespace BoardGamesCore
{

#ifdef STILL_STEPS
  const std::vector<Location> ComplexMove::GetJumped(void) const  // return list of jumped-over locations
  {
    std::vector<Location> jumped{};
    for (auto& s : _steps)
    {
      if (s->GetType() == Step::StepType::Jump) throw ("code missing!"); // TODO
    }
    return jumped;
  }
#endif STILL_STEPS

  //Move&& Move::operator =(Move&& m) noexcept
  //{
  //  std::swap(step, m.step);
  //  value = m.value;
  //  return std::move(*this);
  //}

}
