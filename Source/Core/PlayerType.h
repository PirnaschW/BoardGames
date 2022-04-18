
namespace BoardGamesCore
{

  class PlayerType final
  {
  private:
    constexpr PlayerType(const char& t) noexcept : type_{ t } {}
  public:
    constexpr PlayerType(const PlayerType& p) noexcept = delete;                 // copy constructor
    constexpr PlayerType& operator=(const PlayerType& p) noexcept = delete;      // copy assignment
    constexpr PlayerType(PlayerType&& p) noexcept = delete;                      // move constructor
    constexpr PlayerType& operator=(PlayerType&& p) noexcept = delete;           // move assignment
    constexpr ~PlayerType() noexcept = default;

    constexpr bool operator ==(const PlayerType& t) const noexcept { return t.type_ == type_; }
    constexpr bool operator !=(const PlayerType& t) const noexcept { return !(*this == t); }
    constexpr bool IsAI() const noexcept { return type_ == Computer.type_; }
    void Serialize(std::stringstream& s) const { s << type_; }

  private:
    const char type_;

  public:  // the only instances ever to exists; handed around by pointer / reference
    static const PlayerType Human;
    static const PlayerType Computer;
  };

  constexpr inline const PlayerType PlayerType::Human{ 'H' };
  constexpr inline const PlayerType PlayerType::Computer{ 'C' };

  static_assert(!std::is_abstract<PlayerType>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class PlayerType>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class PlayerType, char&>::value, "must not be 'char&' constructible");
  static_assert(!PlayerType::Human.IsAI(), "PlayerType is not constexpr");

}
