
namespace BoardGamesCore
{

  class Mode //TODO!: replace Mode class by separated bit structure
  {
  public:
    enum class Mode_ : unsigned char {
      None = 0x00,
      SelectFr = 0x01,
      SelectTo = 0x02,
      Editing = 0x04,
      Dragging = 0x08,
      ShowStock = 0x10,
      ShowTaken = 0x20,
      GameOver = 0x80,
    };
    constexpr Mode(Mode_ a) noexcept : mode_(a) {};              // allow to create an object from the enum
    constexpr Mode& Set(Mode a) noexcept;                        // set a bit
    constexpr Mode& Del(Mode a) noexcept;                        // remove a bit
    constexpr bool IsSet(Mode::Mode_ a) const noexcept;          // test for a bit
  public:
    Mode_ mode_;
  };
  constexpr Mode::Mode_ operator|(Mode::Mode_ a, Mode::Mode_ b) noexcept { return static_cast<Mode::Mode_>(static_cast<typename std::underlying_type<Mode::Mode_>::type>(a) | static_cast<typename std::underlying_type<Mode::Mode_>::type>(b)); }
  constexpr Mode::Mode_ operator&(Mode::Mode_ a, Mode::Mode_ b) noexcept { return static_cast<Mode::Mode_>(static_cast<typename std::underlying_type<Mode::Mode_>::type>(a) & static_cast<typename std::underlying_type<Mode::Mode_>::type>(b)); }
  constexpr Mode::Mode_ operator~(Mode::Mode_ a) noexcept { return static_cast<Mode::Mode_>(~static_cast<typename std::underlying_type<Mode::Mode_>::type>(a)); }
  constexpr bool operator!(Mode::Mode_ a) noexcept { return !(static_cast<typename std::underlying_type<Mode::Mode_>::type>(a)); }

  // must be defined afterwards, as _Mode bitwise operators and Mode class both need to be known and complete
  constexpr Mode& Mode::Set(Mode a) noexcept { mode_ = mode_ | a.mode_;  return *this; }
  constexpr Mode& Mode::Del(Mode a) noexcept { mode_ = mode_ & ~a.mode_; return *this; }
  constexpr bool Mode::IsSet(Mode::Mode_ a) const noexcept { return !!(mode_ & a); }
  using Mode_ = Mode::Mode_;

  static_assert(Mode(Mode_::Editing).IsSet(Mode_::Editing), "Mode is not constexpr");

}
