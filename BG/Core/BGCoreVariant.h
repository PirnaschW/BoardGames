namespace BoardGamesCore
{

  class Variant final   // PoD of possible game variants and settings
  {
  public:
    constexpr Variant(Coordinate xd, Coordinate yd = 0, const char* n = nullptr, Coordinate xmin = 0, Coordinate xmax = 0, Coordinate ymin = 0, Coordinate ymax = 0) noexcept :
      xDef(xd), yDef(yd ? yd : xd), name(n), xMin(xmin), xMax(xmax), yMin(ymin ? ymin : xmin), yMax(ymax ? ymax : xmax) {}
    Variant& operator=(const Variant&) = default;  //{ *this = v; return *this; }
  public:
    Coordinate xDef;
    Coordinate yDef;
    const char* name;
    Coordinate xMin;
    Coordinate xMax;
    Coordinate yMin;
    Coordinate yMax;
  };
  typedef std::vector<Variant> VariantList;

  template <class TClass>  // template enforces every Game to have the required static functions (abstract static functions are not possible)
  class Variants {
  public:
    constexpr inline static const VariantList& GetVariants(void) noexcept { return TClass::GetVariants(); }
    constexpr inline static const PieceMapP& GetPieces(void) noexcept {
      const PieceMapP& p{ TClass::GetPieces() };
      p->Add(&Piece::NoPiece);
      p->Add(&Piece::NoTile);
      return p;
    }
  };


  static_assert(std::is_abstract<IUI>::value, "must be abstract");
  static_assert(!std::is_trivially_constructible<class IUI>::value, "must not be trivially constructible");

}