namespace BoardGamesCore
{

  class MainPosition;
  class Action abstract  // define one atomic action of a Move
  {
  private:
    Action(void) = delete; // disable blank constructor
  protected:
    inline Action(const Location& l, const Piece* p) noexcept : _l{ l }, _p{ p } {}
  public:
    inline virtual ~Action(void) noexcept = default; // virtual destructor, as class is a base class
    Action(const Action&) = default;                   // copy constructor
    Action& operator=(const Action&) = default;        // copy assignment
    Action(Action&&) = default;                        // move constructor
    Action& operator=(Action&&) = default;             // move assignment

    inline const Location& GetLocation(void) const noexcept { return _l; }
    inline const Piece* GetPiece(void) const noexcept { return _p; }
    //inline bool operator == (const Action& a) const noexcept { return _l == a._l && _p == a._p; }
    //inline bool operator != (const Action& a) const noexcept { return !(*this == a); }
    inline virtual bool IsTake(void) const noexcept = 0;
    inline virtual void Execute(MainPosition* p) const noexcept = 0;

  protected:
    const Location _l;
    const Piece* _p;
  };
  using ActionP = std::shared_ptr<Action>;
  using Actions = std::vector<ActionP>;

  class ActionTake : public Action  // Take-Action: Take one piece from the board (into 'hand')
  {
  public:
    inline ActionTake(const Location& l, const Piece* p) noexcept : Action(l, p) {}
    inline virtual bool IsTake(void) const noexcept { return true; }
    void Execute(MainPosition* p) const noexcept;
    inline bool operator == (const ActionTake& a) const noexcept { return _l == a._l && _p == a._p; }
    inline bool operator != (const ActionTake& a) const noexcept { return !(*this == a); }
  };

  // (only inserted if the jumping is relevant for the game)
  class ActionJump : public Action  // Jump-Action: Jump over one piece on the board
  {
  public:
    inline ActionJump(const Location& l, const Piece* p) noexcept : Action(l, p) {}
    void Execute(MainPosition* p) const noexcept;
    inline bool operator == (const ActionJump& a) const noexcept { return _l == a._l && _p == a._p; }
    inline bool operator != (const ActionJump& a) const noexcept { return !(*this == a); }
  };

  class ActionPlace : public Action  // Place-Action: Place one piece on the board (from 'hand')
  {
  public:
    inline ActionPlace(const Location& l, const Piece* p) noexcept : Action(l, p) {}
    inline virtual bool IsTake(void) const noexcept { return false; }
    void Execute(MainPosition* p) const noexcept;
    inline bool operator == (const ActionPlace& a) const noexcept { return _l == a._l && _p == a._p; }
    inline bool operator != (const ActionPlace& a) const noexcept { return !(*this == a); }
  };




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
    inline Step(const Field& Fr, const Field& To, Step::StepType Ty = Step::StepType::Normal) noexcept : _type{ Ty }, _from{ Fr }, _to{ To } {}
  public:
    inline virtual ~Step(void) noexcept = default; // virtual destructor, as class is a base class
    Step(const Step&) = default;                   // copy constructor
    Step& operator=(const Step&) = default;        // copy assignment
    Step(Step&&) = default;                        // move constructor
    Step& operator=(Step&&) = default;             // move assignment

    inline const Field& GetFr(void) const noexcept { return _from; }
    inline const Field& GetTo(void) const noexcept { return _to; }
    inline const StepType GetType(void) const noexcept { return _type; }
    inline bool IsTake(void) const noexcept { return _type & StepType::Take; }
    inline bool operator == (const Step& s) const noexcept { return _from == s._from && _to == s._to && _type == s._type; }
    inline bool operator != (const Step& s) const noexcept { return !(s == *this); }

    virtual inline const Fields GetTakes(void) const noexcept = 0;
    virtual inline const Field& GetTake(unsigned int z = 0) const = 0;

  private:
    const StepType _type;
    const Field _from;
    const Field _to;
  };
  static_assert(std::is_abstract<Step>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<Step>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible<Step, const Field&, const Field&, Step::StepType>::value, "is not constructible");

  using StepP = std::shared_ptr<Step>;
  using Steps = std::vector<StepP>;

  class StepPlace : public Step  // Placement Step means only insert a new Piece; no takes
  {
  public:
    inline StepPlace(const Field& To) noexcept : Step(To, To, Step::StepType::Place) {}
    virtual inline const Fields GetTakes(void) const noexcept override { return Fields{}; }
    virtual inline const Field& GetTake(unsigned int /*z*/ = 0) const { return GetTo(); }
  };
  static_assert(!std::is_abstract<StepPlace>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<StepPlace>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<StepPlace, const Field&>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<StepPlace, const Field&>::value, "is not nothrow constructible");

  class StepSimple : public Step  // Simple Step means 'Take' is whatever is found on field 'To'
  {
  public:
    inline StepSimple(const Field& Fr, const Field& To, Step::StepType Ty = Step::StepType::Normal) noexcept : Step(Fr, To, Ty) {}
    virtual inline const Fields GetTakes(void) const noexcept override { return Fields{GetTo()}; }
    virtual inline const Field& GetTake(unsigned int /*z*/ = 0) const { return GetTo(); }
  };
  static_assert(!std::is_abstract<StepSimple>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<StepSimple>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<StepSimple, const Field&, const Field&, Step::StepType>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<StepSimple, const Field&, const Field&, Step::StepType>::value, "is not nothrow constructible");

  class StepComplex : public Step
  {
  public:
    inline StepComplex(const Field& Fr, const Field& To, Step::StepType Ty = Step::StepType::Normal, const Fields& Takes = {}) noexcept : Step{ Fr, To, Ty }, _takes{ Takes } {}

    virtual inline bool operator == (const StepComplex& s) const noexcept { return this->Step::operator==(s) && _takes == s._takes; }
    virtual inline bool operator != (const StepComplex& s) const noexcept { return !(s == *this); }
    virtual inline const Fields GetTakes(void) const noexcept override { return _takes; }
    virtual inline const Field& GetTake(unsigned int z = 0) const { return _takes[z]; }

  private:
    const Fields _takes;
  };
  static_assert(!std::is_abstract<StepComplex>::value, "must not be constructible");
  static_assert(!std::is_trivially_constructible<StepComplex>::value, "must not be trivially constructible");
  static_assert(!std::is_constructible < StepComplex, const Field&>::value, "is not constructible");
  static_assert(std::is_constructible < StepComplex, const Field&, const Field&>::value, "is not constructible");
  static_assert(std::is_constructible<StepComplex, const Field&, const Field&, Step::StepType>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<StepComplex, const Field&, const Field&, Step::StepType>::value, "is not nothrow constructible");

}