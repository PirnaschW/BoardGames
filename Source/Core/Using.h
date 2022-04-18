

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

  struct VariantChosen
  {
    GameID id;               // internal Game ID
    GameGroup g;             // Game group code
    VariantCode c;           // variant code
    Coordinate x;            // horizontal board size
    Coordinate y;            // vertical board size
  };
  static_assert(std::is_trivial<VariantChosen>::value, "must be a trivial structure");

  using BoardGamesMFC::Bitmap;
  using BoardGamesMFC::Archive;
  using BoardGamesMFC::DC;
  using BoardGamesMFC::Rect;

}
