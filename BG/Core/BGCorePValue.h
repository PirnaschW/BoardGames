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
    //    constexpr inline PositionValue(PValueType type = PValueType::Undefined) noexcept : _type{ type } {}
    constexpr inline PositionValue(PValueType type) noexcept : _type{ type } {}
    constexpr inline PositionValue(int value) noexcept : _type{ PValueType::Normal }, value_{ value } {}
    constexpr inline PositionValue(size_t value) noexcept : _type{ PValueType::Normal }, value_{ static_cast<int>(value) } {}

    constexpr inline bool operator== (const PositionValue & p) const noexcept { return _type == p._type && value_ == p.value_; }
    constexpr inline bool operator!= (const PositionValue & p) const noexcept { return !(*this == p); }
    constexpr inline bool operator> (const PositionValue & p) const noexcept
    {
      switch (_type)
      {
      case Lost: return false;                               // Lost is not greater than anything
      case Undefined: return p._type == Lost;                // Undefined is only greater than lost
      case Tie: return (p._type == Lost ||
        p._type == Undefined ||
        (p._type == Normal && p.value_ < 0));                // Tie is better than Lost. Undefined, and losing
      case Normal: return (p._type == Lost ||
        p._type == Undefined ||
        (p._type == Tie && value_ > 0) ||                    // Any value is better than Lost; positive is better than Tie
        (p._type == Normal && value_ > p.value_));           // and better is better, of course
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
      case Normal: return PositionValue{ -value_ };
      case Undefined: return PositionValue{ Undefined };
      default: return PositionValue{ Undefined };
      }
    }
    constexpr inline const PositionValue operator+ (const PositionValue & p) const noexcept { return PositionValue{ value_ + p.value_ }; }
    constexpr inline const PositionValue operator- (const PositionValue & p) const noexcept { return PositionValue{ value_ - p.value_ }; }

    constexpr inline PositionValue& operator+= (const PositionValue & p) noexcept { value_ += p.value_; return *this; }
    constexpr inline PositionValue& operator-= (const PositionValue & p) noexcept { value_ -= p.value_; return *this; }
    constexpr inline PositionValue& operator+= (int z) noexcept { value_ += z; return *this; }
    constexpr inline PositionValue& operator-= (int z) noexcept { value_ -= z; return *this; }
    constexpr inline PositionValue& operator++ (void) noexcept { value_++; return *this; }
    constexpr inline PositionValue& operator-- (void) noexcept { value_--; return *this; }
    constexpr inline PositionValue Relative(bool w) const noexcept { return (w ? *this : -*this); }

    constexpr inline operator int(void) const { if (_type != Normal) throw std::exception("undefined PValue"); return value_; }
    inline operator const char*(void) const {
      static std::string s; 
      if (_type == Normal) s = std::to_string(value_);
      else s = "undefined";
      return s.c_str();
    }

  private:
    PValueType _type{ PValueType::Undefined };
    int value_{ 0 };
  };

  static_assert(!std::is_abstract<PositionValue>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<PositionValue>::value, "must not be trivially constructible");
  static_assert(std::is_constructible < PositionValue, PositionValue::PValueType>::value, "is not constructible");
  static_assert(std::is_constructible < PositionValue, int>::value, "is not constructible");
  static_assert(std::is_constructible<PositionValue, size_t>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<PositionValue, PositionValue::PValueType>::value, "is not nothrow constructible");
  static_assert(std::is_nothrow_constructible<PositionValue, int>::value, "is not nothrow constructible");
  static_assert(std::is_nothrow_constructible<PositionValue, size_t>::value, "is not nothrow constructible");

}