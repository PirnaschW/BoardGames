
namespace BoardGamesCore
{

  class MainPosition;  // forward declaration needed in class Kind
  class Kind abstract
  {
  protected:
    constexpr inline Kind(const char& k) noexcept : kind{ k } {}
  public:
    Kind& operator=(const Kind&) = delete;          // delete assignment operator

    inline bool operator==(const Kind& k) const noexcept { return k.kind == kind; }
    inline size_t GetHash(void) const noexcept { return std::hash<char>()(kind); }
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept = 0;          // no useful default implementation provided!
    virtual inline void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const {};         // useful default implementation: No moves possible
    virtual inline bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept { return false; }   // useful default implementation: cannot drop
    virtual void Serialize(CArchive* ar) const;

  private:
    const char kind;
  };

  static_assert(std::is_abstract<Kind>::value, "must be abstract");
  static_assert(!std::is_trivially_constructible<class Kind>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class Kind, char&>::value, "must be 'char&' constructible");
  static_assert(!std::is_assignable<class Kind, class Kind>::value, "must not be assignable");

  class noKind : public Kind
  {
  private:
    constexpr inline noKind(void) noexcept : Kind('X') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 0; }

  public:  // the only instance ever to exists; handed around by pointer
    static const noKind NoKind;
  };
  static_assert(!std::is_abstract<noKind>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class noKind>::value, "must not be trivially constructible");

}
