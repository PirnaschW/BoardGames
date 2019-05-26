
namespace BoardGamesCore
{

  class Move abstract
  {
  protected:
    inline Move(void) noexcept {}

  public:
    virtual ~Move(void) noexcept {}
    constexpr inline void SetValue(const PositionValue& v) noexcept { _value = v; }
    constexpr inline PositionValue GetValue(void) const noexcept { return _value; }
    constexpr inline bool operator <(const Move& rhs) const noexcept { return _value < rhs._value; }

//    virtual inline bool operator==(const Move& m) const = 0;
    virtual inline const Steps GetSteps(void) const = 0;
    virtual inline const StepP GetStep(unsigned int i = 0) const noexcept = 0;
    virtual inline bool IsTake(void) const noexcept = 0;
    virtual inline const Field& GetFr(void) const noexcept = 0;
    virtual inline const Field& GetTo(void) const noexcept = 0;

  private:
    PositionValue _value{ PositionValue::Undefined };
  };
  static_assert(std::is_abstract<Move>::value, "must not be constructible");
  static_assert(!std::is_constructible<Move, const Move&>::value, "is not constructible");

  using MoveP = std::shared_ptr<Move>;
  using Moves = std::vector<MoveP>;


  class SimpleMove : public Move   //  one Piece moves one Step, optional takes one Piece from move target
  {
  public:
    inline SimpleMove(const StepP s) noexcept : _step(s) {}

    virtual inline bool operator==(const SimpleMove& m) const { return _step == m._step; };
    virtual inline const Steps GetSteps(void) const override { return Steps(1, _step); }
    virtual inline const StepP GetStep(unsigned int /* i */ = 0) const noexcept override { return _step; }
    virtual inline bool IsTake(void) const noexcept override { return _step->IsTake(); }
    virtual inline const Field& GetFr(void) const noexcept override { return _step->GetFr(); }
    virtual inline const Field& GetTo(void) const noexcept override { return _step->GetTo(); }

  private:
    const StepP _step;
  };
  static_assert(!std::is_abstract<SimpleMove>::value, "is not constructible");
  static_assert(!std::is_trivially_constructible<SimpleMove>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<SimpleMove, const StepP>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<SimpleMove, const StepP>::value, "is not nothrow constructible");

  class ComplexMove : public Move  //  optional multiple Pieces, multi-Step moves, optional multi-takes
  {
  public:
    inline ComplexMove(const Steps& s) noexcept : _steps{ s } {}

    virtual inline bool operator==(const ComplexMove& m) const { return _steps == m._steps; };
    virtual inline const Steps GetSteps(void) const override { return _steps; }
    virtual inline const StepP GetStep(unsigned int i = 0) const noexcept override { return _steps[i]; }
    virtual inline bool IsTake(void) const noexcept override { for (auto s : _steps) if (s->IsTake()) return true; return false; }
    virtual inline const Field& GetFr(void) const noexcept override { return _steps.front()->GetFr(); }
    virtual inline const Field& GetTo(void) const noexcept override { return _steps.back()->GetTo(); }

    const std::vector<Location> GetJumped(void) const;  // return list of jumped-over locations

  private:
    const Steps& _steps;
  };
  static_assert(!std::is_abstract<ComplexMove>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<ComplexMove>::value, "must not be trivially constructible");
  static_assert(std::is_constructible < ComplexMove, const Steps&>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<ComplexMove, const Steps&>::value, "is not nothrow constructible");

}
