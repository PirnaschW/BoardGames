// depends on PValue, Location, Action

namespace BoardGamesCore
{

  class Move  // a sequence of actions for a player that are considered a complete move
  {
  protected:
    constexpr Move() noexcept {}

  public:
    constexpr Move(const Actions& a) noexcept : a_(a) {}
    constexpr virtual ~Move() noexcept = default;
    constexpr void SetValue(const PositionValue& v) noexcept { value_ = v; }
    constexpr PositionValue GetValue() const noexcept { return value_; }

    constexpr bool operator <(const Move& rhs) const noexcept { return value_ < rhs.value_; }  // NOTE: those operators compare the Position values!
    constexpr bool operator >(const Move& rhs) const noexcept { return value_ > rhs.value_; }
    constexpr bool operator <=(const Move& rhs) const noexcept { return !(*this > rhs); }
    constexpr bool operator >=(const Move& rhs) const noexcept { return !(*this < rhs); }

    constexpr bool operator ==(const Move& m) const noexcept { return a_ == m.a_; }            // NOTE: and these the whole move sequence
    constexpr bool operator !=(const Move& m) const noexcept { return !(*this == m); }
    
    constexpr const Actions& GetActions() const noexcept { return a_; }
    constexpr const Location& GetFrL() const noexcept { return a_.front()->GetLocation(); }
    constexpr const Location& GetToL() const noexcept { return a_.back()->GetLocation(); }

#ifdef LOG
  public:
    void Log() const;
#endif // LOG

  private:
    PositionValue value_{ PositionValue::PValueType::Undefined };
    const Actions a_{};
  };
  static_assert(!std::is_abstract<Move>::value, "must not be constructible");
  static_assert(std::is_constructible<Move, const Move&>::value, "is not constructible");
  static_assert(Move(Actions()).GetValue() == PositionValue::PValueType::Undefined, "Move is not constexpr");

  using Moves = std::vector<std::shared_ptr<Move>>;

}
