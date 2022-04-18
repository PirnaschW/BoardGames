// not dependent on anything

namespace BoardGamesCore
{

  using PValueBaseType = long long;
  class PositionValue final
  {
  public:
    enum class PValueType : unsigned char
    {
      Undefined = 0x01,
      Normal = 0x02,
      Won = 0x04,
      Lost = 0x08,
      Tie = 0x10,
    };

    constexpr bool operator== (PValueType type) const noexcept { return type_ == type; }
    constexpr bool operator!= (PValueType type) const noexcept { return !(*this == type); }

  public:
    constexpr PositionValue(PValueType type) noexcept : type_{ type } {}
    constexpr PositionValue(PValueBaseType value) noexcept : type_{ PValueType::Normal }, value_{ value } {}

    constexpr bool operator== (const PositionValue& p) const noexcept { return type_ == p.type_ && value_ == p.value_; }
    constexpr bool operator!= (const PositionValue& p) const noexcept { return !(*this == p); }
    constexpr bool operator> (const PositionValue& p) const noexcept
    {
      switch (type_)
      {
        case PValueType::Lost: return false;                               // Lost is not greater than anything
        case PValueType::Undefined: return p.type_ == PValueType::Lost;    // Undefined is only greater than lost
        case PValueType::Tie: return (p.type_ == PValueType::Lost ||
          p.type_ == PValueType::Undefined ||
          (p.type_ == PValueType::Normal && p.value_ < 0));                // Tie is better than Lost. Undefined, and losing
        case PValueType::Normal: return (p.type_ == PValueType::Lost ||
          p.type_ == PValueType::Undefined ||
          (p.type_ == PValueType::Tie && value_ > 0) ||                    // Any value is better than Lost; positive is better than Tie
          (p.type_ == PValueType::Normal && value_ > p.value_));           // and better is better, of course
        case PValueType::Won: return p.type_ != PValueType::Won;           // Won is better than anything except Won
        default: assert(false); return false;
      }
    }
    constexpr bool operator< (const PositionValue& p) const noexcept { return (*this != p) && (!(*this > p)); }
    constexpr bool operator>= (const PositionValue& p) const noexcept { return !(*this < p); }
    constexpr bool operator<= (const PositionValue& p) const noexcept { return !(*this > p); }
    constexpr const PositionValue operator+ () const noexcept { return *this; }
    constexpr const PositionValue operator- () const noexcept
    {
      switch (type_)
      {
        case PValueType::Lost: return PositionValue{ PValueType::Won };
        case PValueType::Won: return PositionValue{ PValueType::Lost };
        case PValueType::Tie: return PositionValue{ PValueType::Tie };
        case PValueType::Normal: return PositionValue{ -value_ };
        case PValueType::Undefined: return PositionValue{ PValueType::Undefined };
        default: assert(false); return PositionValue{ PValueType::Undefined };
      }
    }
    constexpr PositionValue operator+ (const PositionValue& p) const noexcept { return PositionValue{ value_ + p.value_ }; }
    constexpr PositionValue operator- (const PositionValue& p) const noexcept { return PositionValue{ value_ - p.value_ }; }

    constexpr PositionValue& operator+= (const PositionValue& p) noexcept { value_ += p.value_; return *this; }
    constexpr PositionValue& operator-= (const PositionValue& p) noexcept { value_ -= p.value_; return *this; }
    constexpr PositionValue& operator+= (PValueBaseType z) noexcept { value_ += z; return *this; }
    constexpr PositionValue& operator-= (PValueBaseType z) noexcept { value_ -= z; return *this; }
    constexpr PositionValue& operator++ () noexcept { value_++; return *this; }
    constexpr PositionValue& operator-- () noexcept { value_--; return *this; }
    [[TODO::Unittest]] constexpr PositionValue operator* (PValueBaseType f) const noexcept { return PositionValue{ value_ * f }; }
    constexpr PositionValue Relative(bool w) const noexcept { return (w ? *this : -*this); }
    constexpr bool IsDecided() const noexcept { return type_ == PValueType::Lost || type_ == PValueType::Won; }

    constexpr operator PValueBaseType() const { if (type_ != PValueType::Normal) throw std::exception("undefined PValue"); return value_; }
    operator const wchar_t* () const {
      static std::wstring s{};
      switch (type_)
      {
        case PValueType::Normal:    s = std::to_wstring(value_);   break;
        case PValueType::Won:       s = L"Won";                    break;
        case PValueType::Lost:      s = L"Lost";                   break;
        case PValueType::Tie:       s = L"Tie";                    break;
        case PValueType::Undefined: s = L"Undefined!";             break;
        default: assert(false);     s = L"huh?";                   break;
      }
      return s.c_str();
    }

#ifdef LOG
  public:
    void Log() const;
#endif // LOG

  private:
    PValueBaseType value_{ 0LL };
    PValueType type_{ PValueType::Undefined };
  };

  static_assert(!std::is_abstract<PositionValue>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<PositionValue>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<PositionValue, PositionValue::PValueType>::value, "is not constructible");
  static_assert(std::is_constructible<PositionValue, int>::value, "is not constructible");
  static_assert(std::is_constructible<PositionValue, size_t>::value, "is not constructible");
  static_assert(std::is_constructible<PositionValue, PValueBaseType>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<PositionValue, PositionValue::PValueType>::value, "is not nothrow constructible");
  static_assert(std::is_nothrow_constructible<PositionValue, int>::value, "is not nothrow constructible");
  static_assert(std::is_nothrow_constructible<PositionValue, size_t>::value, "is not nothrow constructible");
  static_assert(PositionValue(0) != PositionValue::PValueType::Tie, "PositionValue is not constexpr");
  static_assert(sizeof(PositionValue) == 16, "PValue size too large");

}