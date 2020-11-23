
namespace BoardGamesCore
{

  class PlayerType final
  {
  private:
    constexpr PlayerType(const char& p) noexcept : playertype_{ p } {}
  public:
    constexpr bool operator ==(const PlayerType& t) const noexcept { return t.playertype_ == playertype_; }
    constexpr bool IsAI(void) const noexcept { return playertype_ == Computer.playertype_; }
    void Serialize(Archive& ar) const;

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
    constexpr Player(const PlayerType* p = &PlayerType::Human, const PieceColor& c = PieceColor::NoColor) noexcept : playertype_{ p }, color_{ c } {}    // constructor

    constexpr bool IsAI(void) const noexcept { return playertype_->IsAI(); }
    constexpr const PieceColor& GetColor(void) const noexcept { return color_; }

//  constexpr bool IsColor(const PieceColor& c) const noexcept { return c == color_; }
//  constexpr bool IsPlayerType(const PlayerType* p) const noexcept { return p == playertype_; }

//  constexpr Player(const Player& p) noexcept : playertype_{ p.playertype_ }, color_{ p.color_ } {}                                              // copy constructor
//  Player& operator=(const Player& p) noexcept { return *this = Player(p.playertype_, p.color_); }                                // copy assignment
//  Player(Player&& p) noexcept : playertype_{ std::move(p.playertype_) }, color_{ std::move(p.color_) } {}                          // move constructor
//  Player& operator=(Player&& p) noexcept { std::swap(playertype_, p.playertype_); std::swap(color_, p.color_); return *this; }     // move assignment

//  constexpr const PieceColor& SetColor(const PieceColor& c) noexcept { return color_ = c; }
//  constexpr const PlayerType* SetPlayerType(const PlayerType* p) noexcept { return playertype_ = p; }
//  constexpr const PlayerType* GetPlayerType(void) const noexcept { return playertype_; }
//  constexpr bool Is(const PlayerType* p) const noexcept { return playertype_ == p; }

  private:
  //const std::string name_{};
    const PlayerType* playertype_;
    const PieceColor& color_;
  };

  static_assert(!std::is_abstract<Player>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Player>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Player, class PlayerType*, class PieceColor&>::value, "must be 'PlayerType*, PieceColor*' constructible");

}
