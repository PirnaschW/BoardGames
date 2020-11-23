
namespace BoardGamesCore
{
  class Actions;

  class Move
  {
  protected:
    Move(void) noexcept {}

  public:
    Move(const Actions& a) noexcept;
    virtual ~Move(void) noexcept;
    constexpr void SetValue(const PositionValue& v) noexcept { value_ = v; }
    constexpr PositionValue GetValue(void) const noexcept { return value_; }
    constexpr bool operator <(const Move& rhs) const noexcept { return value_ < rhs.value_; }
    bool operator==(const Move& m) const noexcept;
    bool operator!=(const Move& m) const noexcept;
    const Actions& GetActions(void) const noexcept;
    const Location& GetFrL(void) const noexcept;
    const Location& GetToL(void) const noexcept;

  private:
    PositionValue value_{ PositionValue::PValueType::Undefined };
    const Actions a_{};
  };
  static_assert(!std::is_abstract<Move>::value, "must not be constructible");
  static_assert(std::is_constructible<Move, const Move&>::value, "is not constructible");

  using MoveP = std::shared_ptr<Move>;
  using Moves = std::vector<MoveP>;

}
