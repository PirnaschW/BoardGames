
namespace BoardGamesCore
{
  class Location;
  class Piece;
  class MainPosition;

  class Action abstract  // define one atomic action of a Move
  {
  private:
    Action(void) = delete; // disable blank constructor
  protected:
    Action(const Location l, const Piece& p) noexcept;
  public:
    virtual ~Action(void) noexcept = default; // virtual destructor, as class is a base class
    Action(const Action&) = default;                   // copy constructor
    Action& operator=(const Action&) = default;        // copy assignment
    Action(Action&&) = default;                        // move constructor
    Action& operator=(Action&&) = default;             // move assignment

    bool operator == (const Action& a) const noexcept;
    bool operator != (const Action& a) const noexcept;

    const Location& GetLocation(void) const noexcept;
    const Piece& GetPiece(void) const noexcept;
    virtual bool IsJump(void) const noexcept;
    virtual void Execute(MainPosition* p) const noexcept = 0;

  protected:
    const Location l_;
    const Piece& p_;
  };
  using ActionP = std::shared_ptr<Action>;

  class Actions : public std::vector<ActionP>
  {
  public:
    bool IsRepeat(const Location& l, const Offset& o) const noexcept;   // is there already a jump over l (in direction o)?
    bool HasJump(void) const noexcept;
    bool operator == (const Actions& a) const noexcept;
    bool operator != (const Actions& a) const noexcept { return !(*this == a); }
  };


  class ActionLift : public Action  // Lift-Action: Lift up one piece from the board (into 'hand')
  {
  public:
    ActionLift(const Location l, const Piece& p) noexcept;
    virtual void Execute(MainPosition* p) const noexcept override;
  };

  class ActionJump : public Action  // Jump-Action: Jump over one piece on the board
  {
  public:
    ActionJump(const Location l, const Piece& p) noexcept;
    virtual bool IsJump(void) const noexcept override { return true; }
    virtual void Execute(MainPosition* p) const noexcept override;
  };

  class ActionDrop : public Action  // Drop-Action: Drop one piece on the board (from 'hand')
  {
  public:
    ActionDrop(const Location l, const Piece& p) noexcept;
    virtual void Execute(MainPosition* p) const noexcept override;
  };

  class ActionEliminate : public Action  // Eliminate-Action: remove a tile from the board
  {
  public:
    ActionEliminate(const Location l, const Piece& p) noexcept;
    virtual void Execute(MainPosition* p) const noexcept override;
  };

}