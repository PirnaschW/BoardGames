
namespace BoardGamesCore
{

  using Side = unsigned char;
  class Die final // a single Die - for display mostly
  {
  private:
    constexpr Die(Side s, unsigned int IDL, unsigned int IDS) noexcept;
  public:
    Die(void) noexcept = delete;
    Die(const Die&) noexcept = delete;            // delete copy constructor
    Die& operator=(const Die&) = delete;          // delete assignment operator
    constexpr ~Die(void) noexcept = default;

    constexpr bool operator ==(const Die& d) const noexcept;
    constexpr bool operator !=(const Die& p) const noexcept;
    virtual void Serialize(Archive& ar) const;
    virtual void Draw(DC* pDC, const Rect& r) const;

  public:
    static const std::array<Die,6> Sides;

  private:
    const Side side_;
    Bitmap bmpL_;
    Bitmap bmpS_;
  };

  static_assert(!std::is_abstract<Die>::value, "is abstract");
  static_assert(!std::is_trivially_constructible<Die>::value, "must not be trivially constructible");
  static_assert(!std::is_assignable<Die, Die>::value, "is assignable");

}
