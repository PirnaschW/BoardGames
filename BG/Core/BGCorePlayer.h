
namespace BoardGamesCore
{

  class PlayerType final
  {
  private:
    constexpr inline PlayerType(const char& p) noexcept : playertype{ p } {}
  public:
    constexpr inline bool operator ==(const PlayerType& t) const noexcept { return t.playertype == playertype; }
    void Serialize(CArchive* ar) const;

  private:
    const char playertype;

  public:  // the only instances ever to exists; handed around by pointer
    static const PlayerType Human;
    static const PlayerType Computer;
  };

  static_assert(!std::is_abstract<PlayerType>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class PlayerType>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class PlayerType, char&>::value, "must not be 'char&' constructible");

}
