// depends on Board?, Location?, Moves?

namespace BoardGamesCore
{

  class Board;
  class Piece;
  class Move;
  using MoveP = std::shared_ptr<Move>;
  using Moves = std::vector<MoveP>;


  class Kind abstract
  {
  protected:
    constexpr Kind(const char& k) noexcept : kind_{ k } {}  // only ever construct static objects from within class
    constexpr ~Kind() = default;
  public:
    constexpr Kind(const Kind&) = delete;                  // delete copy constructor
    constexpr Kind& operator=(const Kind&) = delete;       // delete copy assignment
    constexpr Kind(Kind&&) = delete;                       // delete move constructor
    constexpr Kind& operator=(Kind&&) = delete;            // delete move assignment

    constexpr bool operator==(const Kind& k) const noexcept { return k.kind_ == kind_; }
    constexpr bool operator!=(const Kind& k) const noexcept { return !(*this == k); }
    constexpr bool operator==(const char k) const noexcept { return k == kind_; }  // needed for deserialization
    size_t GetHash() const noexcept { return std::hash<char>()(kind_); }
    void Serialize(std::stringstream& s) const  { s << kind_; }

    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept = 0;                                // no useful default implementation provided!
    virtual void CollectMoves(const Board& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept {};                       // useful default implementation: No moves possible
    virtual bool CanMove(const Piece& /*p*/, const Location& /*l*/, const Board* /*board_*/) const noexcept { return true; }    // useful default implementation: this kind can move
    virtual bool CanTake(const Kind& /*k*/) const noexcept { return true; }                                                     // useful default implementation: this kind can take any other kind
    virtual bool CanDrop(const Piece& /*p*/, const Location& /*l*/, const Board* /*board_*/) const noexcept { return false; }   // useful default implementation: this kind cannot drop

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
    constexpr noKind() noexcept : Kind('X') {}
  public:
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 0; }

  public:  // the only instance ever to exists; handed around by pointer
    static const noKind NoKind;
  };

  constexpr inline const noKind noKind::NoKind{};

  static_assert(!std::is_abstract<noKind>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class noKind>::value, "must not be trivially constructible");
  static_assert(noKind::NoKind == noKind::NoKind, "Kinds can't be constexpr");

}
