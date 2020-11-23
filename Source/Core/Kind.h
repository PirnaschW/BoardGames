
namespace BoardGamesCore
{
  //using BoardGamesMFC::Archive;

  class MainPosition;
  //class Location;
  //class Move;
  //using MoveP = std::shared_ptr<Move>;
  //using Moves = std::vector<MoveP>;
  //class Piece;

  class Kind abstract
  {
  protected:
    constexpr Kind(const char& k) noexcept : kind_{ k } {}
  public:
    Kind& operator=(const Kind&) = delete;          // delete assignment operator

    constexpr bool operator==(const Kind& k) const noexcept { return k.kind_ == kind_; }
    size_t GetHash(void) const noexcept { return std::hash<char>()(kind_); }

    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept = 0;                             // no useful default implementation provided!
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept {};                    // useful default implementation: No moves possible
    virtual bool CanMove(const Piece& /*p*/, const Location& /*l*/, const MainPosition* /*pos*/) const noexcept { return true; }    // useful default implementation: this kind can move
    virtual bool CanTake(const Kind& /*k*/) const noexcept { return true; }                                                         // useful default implementation: this kind can take any other kind
    virtual bool CanDrop(const Piece& /*p*/, const Location& /*l*/, const MainPosition* /*pos*/) const noexcept { return false; }   // useful default implementation: this kind cannot drop
    virtual void Serialize(Archive& ar) const;

  protected:
    const char kind_;
  };

  static_assert(std::is_abstract<Kind>::value, "must be abstract");
  static_assert(!std::is_trivially_constructible<class Kind>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<class Kind, char&>::value, "must be 'char&' constructible");
  static_assert(!std::is_assignable<class Kind, class Kind>::value, "must not be assignable");

  class noKind : public Kind
  {
  private:
    constexpr noKind(void) noexcept : Kind('X') {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 0; }

  public:  // the only instance ever to exists; handed around by pointer
    static const noKind NoKind;
  };

  static_assert(!std::is_abstract<noKind>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class noKind>::value, "must not be trivially constructible");

}
