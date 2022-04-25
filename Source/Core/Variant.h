// Varaint.h is not only included in Core and Games, but also in the building of the MFC App - as it is used in the dialog
// this means it cannot rely on previous std:: includes from Core.h
#include <unordered_set>
#include <vector>

namespace BoardGamesCore
{

  class Variant final   // PoD of possible game variants and settings
  {
  private:
    constexpr Variant() = delete;
  public:
    constexpr Variant(GameGroup group, GameID ID,
                      std::string n = "", VariantCode c = '\0', Coordinate xd = 0, Coordinate yd = 0,
                      Coordinate xmin = 0, Coordinate xmax = 0, Coordinate ymin = 0, Coordinate ymax = 0) noexcept :
      group_(group), ID_(ID), vName_(std::move(n)), vCode_(c), xDef_(xd), yDef_(yd ? yd : xd),
      xMin_(xmin), xMax_(xmax), yMin_(ymin ? ymin : xmin), yMax_(ymax ? ymax : xmax) {}
  public:
    const GameGroup group_;        // Game group code
    const GameID ID_;              // Resource ID of Game
    const std::string vName_;      // display name (can be nullptr, then no variant list is shown)
    const VariantCode vCode_;      // variant code
    const Coordinate xDef_;        // default horizontal board size
    const Coordinate yDef_;        // default vertical board size (same as horizontal if missing)
    const Coordinate xMin_;        // minimum horizontal board size (no change allowed if missing)
    const Coordinate xMax_;        // maximum horizontal board size (no change allowed if missing)
    const Coordinate yMin_;        // minimum vertical board size (same as horizontal if missing)
    const Coordinate yMax_;        // maximum vertical board size (same as horizontal if missing)
  };

  static_assert(!std::is_abstract<Variant>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Variant>::value, "must be trivially constructible");
  static_assert(Variant(0,1).vCode_ == 0, "Variant is not constexpr");

  using VariantList = std::vector<Variant>;


  class Vars final
  {
  public:
    constexpr Vars() noexcept = default;
    constexpr ~Vars() noexcept = default;

    void Register(const Variant&& v) noexcept { variants_.emplace_back(v); }   // register a variant
    std::vector<GameID> GetGameIDList()                                // create a unique GameID list
    {  
      std::unordered_set<GameID> set{};
      for (const auto& v : variants_) set.insert(v.ID_);      // first put all IDs in a set ... (removes duplicates)
      return std::vector<GameID>(set.cbegin(), set.cend());   // ... then copy the now unique list into vector
    }
    VariantList GetVariantList(GameID id)   // create a Variant list for one GameID
    {
      VariantList var{};
      for (const auto& v : variants_)
        if (v.ID_ == id) var.push_back(v);
      return var;
    }

    constexpr size_t size() const noexcept { return variants_.size(); }

  private:
    VariantList variants_{};             // the variant collection

  private:
    static Vars Variants;
  };

  inline Vars Variants{};

  static_assert(Vars().size() == 0, "Variantlists can't be constexpr");

  class VariantChosen
  {
  public:
    GameID id;               // internal Game ID
    GameGroup g;             // Game group code
    VariantCode c;           // variant code
    Coordinate x;            // horizontal board size
    Coordinate y;            // vertical board size
  public:
    void Serialize(std::stringstream& s) const noexcept { s << id << g << c << x << y; }
  };
  static_assert(!std::is_abstract<VariantChosen>::value, "VariantChosen should not be abstract");
  static_assert(std::is_trivially_constructible<VariantChosen>::value, "VariantChosen should be trivially constructible");
  static_assert(VariantChosen().id == 0, "VariantChosen should be constexpr");

}
