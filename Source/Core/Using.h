namespace BoardGamesCore
{

  using Coordinate = unsigned char;    // board coordinates - never negative, never larger than ~20
  using VariantCode = unsigned char;   // code for different variants of one 'game'
  using GameID = unsigned char;        // internal ID of the game

  struct VariantChosen
  {
    GameID id;               // internal Game ID
    VariantCode c;           // variant code
    Coordinate x;            // horizontal board size
    Coordinate y;            // vertical board size
  };
  static_assert(std::is_trivial<VariantChosen>::value, "must be a trivial structure");

  struct GameData {                    // collection of all data that defines a game: (BK ID), internal ID, variant code, board size
  public:
    int tp;
    VariantChosen v;
  };
  static_assert(std::is_trivial<GameData>::value, "must be a trivial structure");
}
