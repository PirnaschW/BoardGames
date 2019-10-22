
namespace BoardGamesCore
{
  class Move
  {
  protected:
    inline Move(void) noexcept {}

  public:
    inline Move(const Actions& a) noexcept : a_(a) {}
    virtual ~Move(void) noexcept {}
    constexpr inline void SetValue(const PositionValue& v) noexcept { value_ = v; }
    constexpr inline PositionValue GetValue(void) const noexcept { return value_; }
    constexpr inline bool operator <(const Move& rhs) const noexcept { return value_ < rhs.value_; }
    inline bool operator==(const Move& m) const noexcept { return a_ == m.a_; }
    inline bool operator!=(const Move& m) const noexcept { return !(*this == m); }
    inline const Actions& GetActions(void) const noexcept { return a_; }
    inline const Location& GetFrL(void) const noexcept { return a_.front()->GetLocation(); }
    inline const Location& GetToL(void) const noexcept { return a_.back()->GetLocation(); }

  private:
    PositionValue value_{ PositionValue::Undefined };
    const Actions a_{};
  };
  static_assert(!std::is_abstract<Move>::value, "must not be constructible");
  static_assert(std::is_constructible<Move, const Move&>::value, "is not constructible");

  using MoveP = std::shared_ptr<Move>;
  using Moves = std::vector<MoveP>;

}
