#include "stdafx.h"

namespace BoardGamesCore
{

  const std::vector<Location> ComplexMove::GetJumped(void) const  // return list of jumped-over locations
  {
    std::vector<Location> jumped{};
    for (auto& s : _steps)
    {
      if (s->GetType() == Step::StepType::Jump) throw ("code missing!"); // TODO
    }
    return jumped;
  }


  //Move&& Move::operator =(Move&& m) noexcept
  //{
  //  std::swap(step, m.step);
  //  value = m.value;
  //  return std::move(*this);
  //}

}
