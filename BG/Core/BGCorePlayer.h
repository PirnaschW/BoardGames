
namespace BoardGamesCore
{

  class PlayerType final
  {
  private:
    constexpr inline PlayerType(const char& p) noexcept : playertype_{ p } {}
  public:
    constexpr inline bool operator ==(const PlayerType& t) const noexcept { return t.playertype_ == playertype_; }
    constexpr inline bool IsAI(void) const noexcept { return playertype_ == Computer.playertype_; }
    void Serialize(CArchive* ar) const;

  private:
    const char playertype_;

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
    constexpr inline Player(const PlayerType* p = &PlayerType::Human, const Color* c = &Color::NoColor) noexcept : playertype_{ p }, color_{ c } {}    // constructor

    constexpr inline bool IsAI(void) const noexcept { return playertype_->IsAI(); }
    constexpr inline const Color* GetColor(void) const noexcept { return color_; }

//  constexpr inline bool IsColor(const Color* c) const noexcept { return c == color_; }
//  constexpr inline bool IsPlayerType(const PlayerType* p) const noexcept { return p == playertype_; }

//  constexpr inline Player(const Player& p) noexcept : playertype_{ p.playertype_ }, color_{ p.color_ } {}                                              // copy constructor
//  Player& operator=(const Player& p) noexcept { return *this = Player(p.playertype_, p.color_); }                                // copy assignment
//  Player(Player&& p) noexcept : playertype_{ std::move(p.playertype_) }, color_{ std::move(p.color_) } {}                          // move constructor
//  Player& operator=(Player&& p) noexcept { std::swap(playertype_, p.playertype_); std::swap(color_, p.color_); return *this; }     // move assignment

//  constexpr inline const Color* SetColor(const Color* c) noexcept { return color_ = c; }
//  constexpr inline const PlayerType* SetPlayerType(const PlayerType* p) noexcept { return playertype_ = p; }
//  constexpr inline const PlayerType* GetPlayerType(void) const noexcept { return playertype_; }
//  constexpr inline bool Is(const PlayerType* p) const noexcept { return playertype_ == p; }

  private:
  //const std::string name_{};
    const PlayerType* playertype_;
    const Color* color_;
  };

  static_assert(!std::is_abstract<Player>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Player>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Player, class PlayerType*, class Color*>::value, "must be 'PlayerType*, Color*' constructible");

}
