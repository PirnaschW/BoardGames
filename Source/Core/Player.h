
namespace BoardGamesCore
{

  class Player final
  {
  public:
    constexpr Player(const PlayerType& t, const PieceColor& c) : type_{ t }, color_{ c }
    {  // must have a valid color
      if (!c.IsPlayable()) throw std::invalid_argument("invalid player color");
    }
    constexpr Player(const Player& p) noexcept = delete;                 // copy constructor
    constexpr Player& operator=(const Player& p) noexcept = delete;      // copy assignment
    constexpr Player(Player&& p) noexcept = delete;                      // move constructor
    constexpr Player& operator=(Player&& p) noexcept = delete;           // move assignment

    constexpr bool IsAI() const noexcept { return type_.IsAI(); }
    constexpr bool IsColor(const PieceColor& c) const noexcept { return c == color_; }

  private:
    const PlayerType& type_;
    const PieceColor& color_;
  };

  static_assert(!std::is_abstract<Player>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Player>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Player, class PlayerType&, class PieceColor&>::value, "must be 'PlayerType&, PieceColor*' constructible");
  static_assert(Player(PlayerType::Computer,PieceColor::Black).IsAI(), "Player is not constexpr");

}
