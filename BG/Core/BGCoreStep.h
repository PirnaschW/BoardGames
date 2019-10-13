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

  class Actions : public std::vector<ActionP>
  {
  public:
    bool IsRepeat(const Piece* p, const Location fr, const Location to) const noexcept;
  };

  class ActionTake : public Action  // Take-Action: Take one piece from the board (into 'hand')
  {
  public:
    inline ActionTake(const Location& l, const Piece* p) noexcept : Action(l, p) {}
    inline virtual bool IsTake(void) const noexcept { return true; }
    void Execute(MainPosition* p) const noexcept override;
    inline bool operator == (const ActionTake& a) const noexcept { return _l == a._l && _p == a._p; }
    inline bool operator != (const ActionTake& a) const noexcept { return !(*this == a); }
  };

  // (only inserted if the jumping is relevant for the game)
  class ActionJump : public Action  // Jump-Action: Jump over one piece on the board
  {
  public:
    inline ActionJump(const Location& l, const Piece* p) noexcept : Action(l, p) {}
    void Execute(MainPosition* p) const noexcept override;
    inline bool operator == (const ActionJump& a) const noexcept { return _l == a._l && _p == a._p; }
    inline bool operator != (const ActionJump& a) const noexcept { return !(*this == a); }
  };

  class ActionPlace : public Action  // Place-Action: Place one piece on the board (from 'hand')
  {
  public:
    inline ActionPlace(const Location& l, const Piece* p) noexcept : Action(l, p) {}
    inline virtual bool IsTake(void) const noexcept { return false; }
    void Execute(MainPosition* p) const noexcept override;
    inline bool operator == (const ActionPlace& a) const noexcept { return _l == a._l && _p == a._p; }
    inline bool operator != (const ActionPlace& a) const noexcept { return !(*this == a); }
  };

}