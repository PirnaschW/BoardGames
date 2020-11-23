
#include <vector>

namespace BoardGamesCore
{

  class Variant final   // PoD of possible game variants and settings
  {
  public:
    constexpr inline Variant(const char* n = nullptr, VariantCode c = '\0', Coordinate xd = 0, Coordinate yd = 0,
                      Coordinate xmin = 0, Coordinate xmax = 0, Coordinate ymin = 0, Coordinate ymax = 0) noexcept :
      vName_(n), vCode_(c), xDef_(xd), yDef_(yd ? yd : xd),
      xMin_(xmin), xMax_(xmax), yMin_(ymin ? ymin : xmin), yMax_(ymax ? ymax : xmax) {}

  public:
    const char* vName_;            // display name (can be nullptr, then no variant list is shown)
    const VariantCode vCode_;      // variant code
    const Coordinate xDef_;        // default horizontal board size
    const Coordinate yDef_;        // default vertical board size (same as horizontal if missing)
    const Coordinate xMin_;        // minimum horizontal board size (no change allowed if missing)
    const Coordinate xMax_;        // maximum horizontal board size (no change allowed if missing)
    const Coordinate yMin_;        // minimum vertical board size (same as horizontal if missing)
    const Coordinate yMax_;        // maximum vertical board size (same as horizontal if missing)
  };
  typedef std::vector<Variant> VariantList;


  static_assert(!std::is_abstract<Variant>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Variant>::value, "must not be trivially constructible");

}
