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


  struct Dimension final  /// PoD structure to collect a game's layout dimensions
  {
  public:
    constexpr Dimension(
      Coordinate xc, Coordinate yc,                         // tile count in x and y directions
      unsigned int le, unsigned int te,                     // starting edge in x and y directions
      unsigned int xd, unsigned int yd,                     // tile size in x and y directions
      unsigned int xs = 0, unsigned int ys = 0) noexcept :  // extra distance between tiles in x and y directions
      xCount(xc), yCount(yc),
      xDim(xd), yDim(yd),
      xSkip(xs), ySkip(ys),
      lEdge(le), tEdge(te) {}

  public:
    const Coordinate xCount;
    const Coordinate yCount;
    const unsigned int xDim;
    const unsigned int yDim;
    const unsigned int xSkip;
    const unsigned int ySkip;
    const unsigned int lEdge;
    const unsigned int tEdge;
  };
  using Dimensions = std::array<Dimension, 3>;


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
