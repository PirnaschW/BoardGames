#include "Core.h"

using namespace BoardGamesMFC;

namespace BoardGamesCore
{

  std::size_t BoardPart::GetHash() const noexcept
  {
    if (hash_) return hash_;
    std::size_t z{};
    for (auto& f : fields_)
      z ^= static_cast<size_t>(f.GetPieceIndex()) + 0x9e3779b9 + (z << 6) + (z >> 2);
    return hash_ = z;
  }

  void BoardPart::ForEach(const ActivityRO& func) const noexcept
  {
    for (auto& f : fields_) func(f.GetPieceIndex());
  }

  void BoardPart::ForEach(const ActivityRW& func)  noexcept
  {
    for (auto& f : fields_) f.SetPieceIndex(func(f.GetPieceIndex()));
  }

  std::size_t BoardPart::Index(Coordinate x, Coordinate y) const noexcept
  {
    const Coordinate x0 = x >= 0 ? x : dim_.xCount_ + x;
    const Coordinate y0 = y >= 0 ? y : dim_.yCount_ + y;
    return y0 * dim_.xCount_ + x0;
  }

  void BoardPart::DrawFrame(DC* dc) const
  {
    for (Pixel z = 4; z > 0; z--)
    {
      if (z != 2)
        dc->Rectangle(Rect{ dim_.lEdge_ - z, dim_.tEdge_ - z, dim_.lEdge_ + dim_.xCount_ * (dim_.xDim_ + dim_.xSkip_) + z, dim_.tEdge_ + dim_.yCount_ * (dim_.yDim_ + dim_.ySkip_) + z });
    }
  }

  bool BoardPart::GetLocationFromPoint(const Point& p, Coordinate& x, Coordinate& y) const noexcept
  {
    assert(fields_.size() <= static_cast<size_t>(std::numeric_limits<Coordinate>::max()));
    const Coordinate z = static_cast<Coordinate>(fields_.size());
    for (Coordinate i = 0; i<z; ++i)
      if (fields_[i].IsInRect(p)) {
        x = i % dim_.xCount_;
        y = i / dim_.xCount_;
        return true;
      }
    return false;
  }

}
