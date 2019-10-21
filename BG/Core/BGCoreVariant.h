namespace BoardGamesCore
{

  class Variant final   // PoD of possible game variants and settings
  {
  public:
    constexpr Variant(Coordinate xd, Coordinate yd = 0, const char* n = nullptr, Coordinate xmin = 0, Coordinate xmax = 0, Coordinate ymin = 0, Coordinate ymax = 0) noexcept :
      xDef_(xd), yDef_(yd ? yd : xd), name_(n), xMin_(xmin), xMax_(xmax), yMin_(ymin ? ymin : xmin), yMax_(ymax ? ymax : xmax) {}
    Variant& operator=(const Variant&) = default;  //{ *this = v; return *this; }
  public:
    Coordinate xDef_;
    Coordinate yDef_;
    const char* name_;
    Coordinate xMin_;
    Coordinate xMax_;
    Coordinate yMin_;
    Coordinate yMax_;
  };
  typedef std::vector<Variant> VariantList;


  template <class TClass>  // template enforces every Game to have the required static functions (workaround, as abstract static functions are not possible)
  class Variants {
  public:
    constexpr inline static const VariantList& GetVariants(void) noexcept { return TClass::GetVariants(); }
    constexpr inline static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept { return TClass::GetDimensions(x,y); }
    constexpr inline static const PieceMapP& GetPieces(void) noexcept { const PieceMapP& p{ TClass::GetPieces() }; p->Add(&Piece::NoPiece); p->Add(&Piece::NoTile); return p; }
  };

  static_assert(std::is_abstract<IUI>::value, "must be abstract");
  static_assert(!std::is_trivially_constructible<class IUI>::value, "must not be trivially constructible");

}
