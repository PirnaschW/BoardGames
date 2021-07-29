
namespace BoardGamesCore
{

  class PositionValue final
  {
  public:
    enum class PValueType {
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
    constexpr PositionValue(int value) noexcept : type_{ PValueType::Normal }, value_{ value } {}
    constexpr PositionValue(size_t value) noexcept : type_{ PValueType::Normal }, value_{ static_cast<int>(value) } {}

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
        default: return false;
      }
    }
    constexpr bool operator< (const PositionValue& p) const noexcept { return (*this != p) && (!(*this > p)); }
    constexpr bool operator>= (const PositionValue& p) const noexcept { return !(*this < p); }
    constexpr bool operator<= (const PositionValue& p) const noexcept { return !(*this > p); }
    constexpr const PositionValue operator+ (void) const noexcept { return *this; }
    constexpr const PositionValue operator- (void) const noexcept
    {
      switch (type_)
      {
        case PValueType::Lost: return PositionValue{ PValueType::Won };
        case PValueType::Won: return PositionValue{ PValueType::Lost };
        case PValueType::Tie: return PositionValue{ PValueType::Tie };
        case PValueType::Normal: return PositionValue{ -value_ };
        case PValueType::Undefined: return PositionValue{ PValueType::Undefined };
        default: return PositionValue{ PValueType::Undefined };
      }
    }
    constexpr const PositionValue operator+ (const PositionValue& p) const noexcept { return PositionValue{ value_ + p.value_ }; }
    constexpr const PositionValue operator- (const PositionValue& p) const noexcept { return PositionValue{ value_ - p.value_ }; }

    constexpr PositionValue& operator+= (const PositionValue& p) noexcept { value_ += p.value_; return *this; }
    constexpr PositionValue& operator-= (const PositionValue& p) noexcept { value_ -= p.value_; return *this; }
    constexpr PositionValue& operator+= (int z) noexcept { value_ += z; return *this; }
    constexpr PositionValue& operator-= (int z) noexcept { value_ -= z; return *this; }
    constexpr PositionValue& operator++ (void) noexcept { value_++; return *this; }
    constexpr PositionValue& operator-- (void) noexcept { value_--; return *this; }
    constexpr PositionValue Relative(bool w) const noexcept { return (w ? *this : -*this); }

    constexpr operator int(void) const { if (type_ != PValueType::Normal) throw std::exception("undefined PValue"); return value_; }
    operator const wchar_t* (void) const {
      static std::wstring s{};
      switch (type_)
      {
        case PValueType::Normal:    s = std::to_wstring(value_);   break;
        case PValueType::Won:       s = L"Won";                    break;
        case PValueType::Lost:      s = L"Lost";                   break;
        case PValueType::Tie:       s = L"Tie";                    break;
        case PValueType::Undefined: s = L"Undefined!";             break;
        default:                    s = L"huh?";                   break;
      }
      return s.c_str();
    }

  private:
    PValueType type_{ PValueType::Undefined };
    int value_{ 0 };
  };

  static_assert(!std::is_abstract<PositionValue>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<PositionValue>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<PositionValue, PositionValue::PValueType>::value, "is not constructible");
  static_assert(std::is_constructible<PositionValue, int>::value, "is not constructible");
  static_assert(std::is_constructible<PositionValue, size_t>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<PositionValue, PositionValue::PValueType>::value, "is not nothrow constructible");
  static_assert(std::is_nothrow_constructible<PositionValue, int>::value, "is not nothrow constructible");
  static_assert(std::is_nothrow_constructible<PositionValue, size_t>::value, "is not nothrow constructible");

}