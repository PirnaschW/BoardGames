namespace BoardGamesCore
{

  class PositionValue final
  {
  public:
    enum PValueType {
      Undefined = 0x01,
      Normal = 0x02,
      Won = 0x04,
      Lost = 0x08,
      Tie = 0x10,
    };
    constexpr inline bool operator== (PValueType type) const noexcept { return _type == type; }
    constexpr inline bool operator!= (PValueType type) const noexcept { return !(*this == type); }

  public:
    constexpr inline PositionValue(PValueType type = PValueType::Undefined) noexcept : _type{ type } {}
    constexpr inline PositionValue(int value) noexcept : _type{ PValueType::Normal }, _value{ value } {}
    constexpr inline PositionValue(size_t value) noexcept : _type{ PValueType::Normal }, _value{ static_cast<int>(value) } {}

    constexpr inline bool operator== (const PositionValue & p) const noexcept { return _type == p._type && _value == p._value; }
    constexpr inline bool operator!= (const PositionValue & p) const noexcept { return !(*this == p); }
    constexpr inline bool operator> (const PositionValue & p) const noexcept
    {
      switch (_type)
      {
      case Lost: return false;                               // Lost is not greater than anything
      case Undefined: return p._type == Lost;                // Undefined is only greater than lost
      case Tie: return (p._type == Lost ||
        p._type == Undefined ||
        (p._type == Normal && p._value < 0));                // Tie is better than Lost. Undefined, and losing
      case Normal: return (p._type == Lost ||
        p._type == Undefined ||
        (p._type == Tie && _value > 0) ||                    // Any value is better than Lost; positive is better than Tie
        (p._type == Normal && _value > p._value));           // and better is better, of course
      case Won: return p._type != Won;                       // Won is better than anything except Won
      default: return false;
      }
    }
    constexpr inline bool operator< (const PositionValue & p) const noexcept { return (*this != p) && (!(*this > p)); }
    constexpr inline bool operator>= (const PositionValue & p) const noexcept { return !(*this < p); }
    constexpr inline bool operator<= (const PositionValue & p) const noexcept { return !(*this > p); }
    constexpr inline const PositionValue operator- (void) const noexcept
    {
      switch (_type)
      {
      case Lost: return PositionValue(Won);
      case Won: return PositionValue(Lost);
      case Tie: return PositionValue(Tie);
      case Normal: return PositionValue{ -_value };
      case Undefined: return PositionValue{};
      default: return PositionValue{};
      }
    }
    constexpr inline const PositionValue operator+ (const PositionValue & p) const noexcept { return PositionValue{ _value + p._value }; }
    constexpr inline const PositionValue operator- (const PositionValue & p) const noexcept { return PositionValue{ _value - p._value }; }

    constexpr inline PositionValue& operator+= (const PositionValue & p) noexcept { _value += p._value; return *this; }
    constexpr inline PositionValue& operator-= (const PositionValue & p) noexcept { _value -= p._value; return *this; }
    constexpr inline PositionValue& operator+= (int z) noexcept { _value += z; return *this; }
    constexpr inline PositionValue& operator-= (int z) noexcept { _value -= z; return *this; }
    constexpr inline PositionValue& operator++ (void) noexcept { _value++; return *this; }
    constexpr inline PositionValue& operator-- (void) noexcept { _value--; return *this; }
    constexpr inline PositionValue Relative(bool w) const noexcept { return (w ? *this : -*this); }

    constexpr inline operator unsigned int(void) const noexcept { return (int)_value; }

  private:
    int _value{ 0 };
    PValueType _type{ PValueType::Undefined };
  };

}