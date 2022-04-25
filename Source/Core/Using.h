

namespace BoardGamesCore
{

  using Coordinate  = signed char;      // board coordinates, never larger than ~20
  using VariantCode = unsigned char;    // code for different variants of one 'game'
  using GameGroup   = unsigned char;    // code general game type / group
  using GameID      = unsigned char;    // internal ID of the game
  using Rule        = unsigned short;   // game specific multi-variant rule codes (can be enums)
  using PieceIndex  = unsigned char;    // index of a Piece in the PieceMap

  static_assert((std::numeric_limits<Coordinate>::max)() >= 32, "Coordiante too small for board size");
  static_assert((std::numeric_limits<Rule      >::max)() >= 0xFFFF, "Rule type is defined too small");
  static_assert((std::numeric_limits<PieceIndex>::max)() >= 64, "PieceIndex type is defined too small");

  template <typename T, typename S>  // use verified dynamic_cast when RTTI is available, static_cast otherwise
  inline T down_cast(S&& s)
  {
#ifdef _CPPRTTI
    T p = dynamic_cast<T>(s);
    if constexpr (std::is_pointer<T>::value)
      assert(p != nullptr);
    return p;
#else
    using base_S = std::remove_pointer_t<std::remove_cvref_t<S>>;
    using base_T = std::remove_pointer_t<std::remove_cvref_t<T>>;
    static_assert(std::is_base_of_v<base_S, base_T>);
    return static_cast<T>(s);
#endif
  }

  using BoardGamesMFC::Bitmap;
  using BoardGamesMFC::Archive;
  using BoardGamesMFC::DC;
  using BoardGamesMFC::Rect;


}
