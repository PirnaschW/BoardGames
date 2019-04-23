namespace BoardGamesCore
{

  class Step abstract  // define one hop in a complete move
  {
  public:
    enum StepType {
      Normal = 0x01,    // normal 'sliding' move
      Place = 0x02,     // move places new piece (some games only)
      Take = 0x04,      // move will take one or more pieces
      Jump = 0x08,      // move jumps over one or more pieces
      Promote = 0x10,   // move promotes the piece (Chess, Shogi)
      Drop = 0x20,      // move drops a formerly taken piece back on the board
    };
  private:
    Step(void) = delete; // disable blank constructor
  protected:
    inline Step(const Field& Fr, const Field& To, Step::StepType Ty = Step::StepType::Normal) noexcept : _from{ Fr }, _to{ To }, _type{ Ty } {}
  public:
    inline const Field& GetFr(void) const noexcept { return _from; }
    inline const Field& GetTo(void) const noexcept { return _to; }
    inline const StepType GetType(void) const noexcept { return _type; }
    inline bool IsTake(void) const noexcept { return _type & StepType::Take; }

    virtual inline bool operator == (const Step& s) const noexcept { return _from == s._from && _to == s._to && _type == s._type; }
    virtual inline bool operator != (const Step& s) const noexcept { return !(s == *this); }
    virtual inline const Fields GetTakes(void) const noexcept = 0;

  private:
    const Field _from;
    const Field _to;
    const StepType _type;
  };
  static_assert(std::is_abstract<Step>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<Step>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<Step, const Field&, const Field&, Step::StepType>::value, "is not constructible");

  using StepP = std::shared_ptr<Step>;
  using Steps = std::vector<StepP>;

  class SimpleStep : public Step  // Simple Step means 'Take' is whatever is found on field 'To'
  {
  public:
    inline SimpleStep(const Field& Fr, const Field& To, Step::StepType Ty = Step::StepType::Normal) noexcept : Step(Fr, To, Ty) {}
    virtual inline const Fields GetTakes(void) const noexcept override { return Fields{GetTo()}; }
  };
  static_assert(!std::is_abstract<SimpleStep>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<SimpleStep>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<SimpleStep, const Field&, const Field&, Step::StepType>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<SimpleStep, const Field&, const Field&, Step::StepType>::value, "is not nothrow constructible");

  class ComplexStep : public Step
  {
  public:
    inline ComplexStep(const Field& Fr, const Field& To, Step::StepType Ty = Step::StepType::Normal, const Fields& Takes = {}) noexcept : Step{ Fr, To, Ty }, _takes{ Takes } {}

    virtual inline bool operator == (const ComplexStep& s) const noexcept { return this->Step::operator==(s) && _takes == s._takes; }
    virtual inline bool operator != (const ComplexStep& s) const noexcept { return !(s == *this); }
    virtual inline const Fields GetTakes(void) const noexcept override { return _takes; }

  private:
    const Fields _takes;
  };
  static_assert(!std::is_abstract<ComplexStep>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<ComplexStep>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible < ComplexStep, const Field&>::value, "is not constructible");
  static_assert(std::is_constructible < ComplexStep, const Field&, const Field&>::value, "is not constructible");
  static_assert(std::is_constructible<ComplexStep, const Field&, const Field&, Step::StepType>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<ComplexStep, const Field&, const Field&, Step::StepType>::value, "is not nothrow constructible");

}