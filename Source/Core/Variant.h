
#include <array>
#include <unordered_set>
#include <vector>

namespace BoardGamesCore
{

  class Variant final   // PoD of possible game variants and settings
  {
  public:
    constexpr Variant(GameID ID,
                      std::string n = "", VariantCode c = '\0', Coordinate xd = 0, Coordinate yd = 0,
                      Coordinate xmin = 0, Coordinate xmax = 0, Coordinate ymin = 0, Coordinate ymax = 0) noexcept :
      ID_(ID), vName_(n), vCode_(c), xDef_(xd), yDef_(yd ? yd : xd),
      xMin_(xmin), xMax_(xmax), yMin_(ymin ? ymin : xmin), yMax_(ymax ? ymax : xmax) {}

  public:
    GameID ID_;              // Resource ID of Game
    std::string vName_;      // display name (can be nullptr, then no variant list is shown)
    VariantCode vCode_;      // variant code
    Coordinate xDef_;        // default horizontal board size
    Coordinate yDef_;        // default vertical board size (same as horizontal if missing)
    Coordinate xMin_;        // minimum horizontal board size (no change allowed if missing)
    Coordinate xMax_;        // maximum horizontal board size (no change allowed if missing)
    Coordinate yMin_;        // minimum vertical board size (same as horizontal if missing)
    Coordinate yMax_;        // maximum vertical board size (same as horizontal if missing)
  };
  typedef std::vector<Variant> VariantList;


  static_assert(!std::is_abstract<Variant>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Variant>::value, "must be trivially constructible");
  static_assert(Variant(1).vCode_ == 0, "Variant is not constexpr");



  class Vars final
  {
  public:
    constexpr Vars() noexcept = default;
    constexpr ~Vars() noexcept = default;

    void Register(const Variant& v) noexcept { variants_.push_back(v); }   // register a variant
    std::vector<GameID> GetGameIDList()                                // create a unique GameID list
    {  
      std::unordered_set<GameID> set{};
      for (const auto& v : variants_) set.insert(v.ID_);   // first put them in a set ... (removes duplicates)

      std::vector<GameID> id{};
      id.assign(set.cbegin(),set.cend());                  // ... then copy unique list into vector
      return id;
    }
    std::vector<Variant> GetVariantList(GameID id)          // create a Variant list for one GameID
    {
      std::vector<Variant> var{};
      for (const auto& v : variants_)
        if (v.ID_ == id) var.push_back(v);
      return var;
    }

    constexpr size_t GetCount() const noexcept { return variants_.size(); }

  private:
    std::vector<Variant> variants_{};             // the variant collection

  private:
    static Vars Variants;
  };

  inline Vars Variants{};

  static_assert(Vars().GetCount() == 0, "PieceMaps can't be constexpr");

}
