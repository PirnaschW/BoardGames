
namespace BoardGamesCore
{

  class PlayerType final
  {
  private:
    constexpr inline PlayerType(const char& p) noexcept : playertype{ p } {}
  public:
    constexpr inline bool operator ==(const PlayerType& t) const noexcept { return t.playertype == playertype; }
    constexpr inline bool IsAI(void) const noexcept { return playertype == Computer.playertype; }
    void Serialize(CArchive* ar) const;

  private:
    const char playertype;

  public:  // the only instances ever to exists; handed around by pointer
    static const PlayerType Human;
    static const PlayerType Computer;
  };

  static_assert(!std::is_abstract<PlayerType>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class PlayerType>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class PlayerType, char&>::value, "must not be 'char&' constructible");


  class Player final
  {
  public:
    constexpr inline Player(const PlayerType* p = &PlayerType::Human, const Color* c = &Color::NoColor) noexcept : playertype{ p }, color{ c } {}    // constructor

    constexpr inline bool IsAI(void) const noexcept { return playertype->IsAI(); }
    constexpr inline const Color* GetColor(void) const noexcept { return color; }

//  constexpr inline bool IsColor(const Color* c) const noexcept { return c == color; }
//  constexpr inline bool IsPlayerType(const PlayerType* p) const noexcept { return p == playertype; }

//  constexpr inline Player(const Player& p) noexcept : playertype{ p.playertype }, color{ p.color } {}                                              // copy constructor
//  Player& operator=(const Player& p) noexcept { return *this = Player(p.playertype, p.color); }                                // copy assignment
//  Player(Player&& p) noexcept : playertype{ std::move(p.playertype) }, color{ std::move(p.color) } {}                          // move constructor
//  Player& operator=(Player&& p) noexcept { std::swap(playertype, p.playertype); std::swap(color, p.color); return *this; }     // move assignment

//  constexpr inline const Color* SetColor(const Color* c) noexcept { return color = c; }
//  constexpr inline const PlayerType* SetPlayerType(const PlayerType* p) noexcept { return playertype = p; }
//  constexpr inline const PlayerType* GetPlayerType(void) const noexcept { return playertype; }
//  constexpr inline bool Is(const PlayerType* p) const noexcept { return playertype == p; }

  private:
//  const std::string name{};
    const PlayerType* playertype;
    const Color* color;
  };

  static_assert(!std::is_abstract<Player>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Player>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Player, class PlayerType*, class Color*>::value, "must be 'PlayerType*, Color*' constructible");

}
