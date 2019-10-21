namespace BoardGamesCore
{

  class MainPosition;
  class Action abstract  // define one atomic action of a Move
  {
  private:
    Action(void) = delete; // disable blank constructor
  protected:
    inline Action(const Location& l, const Piece* p) noexcept : l_{ l }, p_{ p } {}
  public:
    inline virtual ~Action(void) noexcept = default; // virtual destructor, as class is a base class
    Action(const Action&) = default;                   // copy constructor
    Action& operator=(const Action&) = default;        // copy assignment
    Action(Action&&) = default;                        // move constructor
    Action& operator=(Action&&) = default;             // move assignment

    inline bool operator == (const Action& a) const noexcept { return l_ == a.l_ && p_ == a.p_; };
    inline bool operator != (const Action& a) const noexcept { return !(*this == a); }

    inline const Location& GetLocation(void) const noexcept { return l_; }
    inline const Piece* GetPiece(void) const noexcept { return p_; }
    inline virtual bool IsJump(void) const noexcept { return false; }
    inline virtual void Execute(MainPosition* p) const noexcept = 0;

  protected:
    const Location l_;
    const Piece* p_;
  };
  using ActionP = std::shared_ptr<Action>;

  class Actions : public std::vector<ActionP>
  {
  public:
    bool IsRepeat(const Location& l, const Offset& o = Offset{ 0,0 }) const noexcept;
    bool HasJump(void) const noexcept;
    bool operator == (const Actions& a) const noexcept;
    inline bool operator != (const Actions& a) const noexcept { return !(*this == a); }
  };


  class ActionTake : public Action  // Take-Action: Take one piece from the board (into 'hand')
  {
  public:
    ActionTake(const Location& l, const Piece* p) noexcept;
    virtual void Execute(MainPosition* p) const noexcept override;
  };

  class ActionJump : public Action  // Jump-Action: Jump over one piece on the board
  {
  public:
    inline ActionJump(const Location& l, const Piece* p) noexcept : Action(l, p) {}
    inline virtual bool IsJump(void) const noexcept override { return true; }
    virtual void Execute(MainPosition* p) const noexcept override;
  };

  class ActionPlace : public Action  // Place-Action: Place one piece on the board (from 'hand')
  {
  public:
    inline ActionPlace(const Location& l, const Piece* p) noexcept : Action(l, p) {}
    virtual void Execute(MainPosition* p) const noexcept override;
  };

}