
namespace BoardGamesCore
{
  class Board;

  class Action abstract  // define one atomic action of a Move
  {
  private:
    constexpr Action() = delete; // disable blank constructor
  protected:
    constexpr Action(const Location l, PieceIndex pI) noexcept : l_{ l }, pI_{ pI } {}
    constexpr virtual ~Action() noexcept = default; // virtual destructor, as class is a base class
  public:
    constexpr Action(const Action&) = default;                   // copy constructor
    constexpr Action& operator=(const Action&) = delete;         // copy assignment
    constexpr Action(Action&&) = default;                        // move constructor
    constexpr Action& operator=(Action&&) = delete;              // move assignment

    constexpr bool operator == (const Action& a) const noexcept { return l_ == a.l_ && pI_ == a.pI_; };
    constexpr bool operator != (const Action& a) const noexcept { return !(*this == a); }

    constexpr const Location& GetLocation() const noexcept { return l_; }
    constexpr PieceIndex GetPieceIndex() const noexcept { return pI_; }

    constexpr virtual bool IsJump() const noexcept { return false; }
    constexpr virtual void Execute(Board* p) const noexcept = 0;

  protected:
    const Location l_;
    const PieceIndex pI_;
  };

  class ActionLift : public Action  // Lift-Action: Lift up one piece from the board (into 'hand')
  {
  public:
    constexpr ActionLift(const Location l, PieceIndex pI) noexcept : Action(l, pI) {}
    constexpr ~ActionLift() = default;
    virtual void Execute(Board* p) const noexcept override;
  };

  class ActionJump : public Action  // Jump-Action: Jump over one piece on the board
  {
  public:
    constexpr ActionJump(const Location l, PieceIndex pI) noexcept : Action(l, pI) {}
    constexpr ~ActionJump() = default;
    virtual bool IsJump() const noexcept override { return true; }
    virtual void Execute(Board* p) const noexcept override;
  };

  class ActionDrop : public Action  // Drop-Action: Drop one piece on the board (from 'hand')
  {
  public:
    constexpr ActionDrop(const Location l, PieceIndex pI) noexcept : Action(l, pI) {}
    constexpr ~ActionDrop() = default;

    virtual void Execute(Board* p) const noexcept override;
  };

  class ActionEliminate : public Action  // Eliminate-Action: remove a tile from the board
  {
  public:
    constexpr ActionEliminate(const Location l, PieceIndex pI) noexcept : Action(l, pI) {}
    constexpr ~ActionEliminate() = default;
    virtual void Execute(Board* p) const noexcept override;
  };


  static_assert(ActionEliminate(Location(BoardPartID::Stage,1,2),3).GetLocation() == Location(BoardPartID::Stage,1,2), "Action is not constexpr");
  static_assert(ActionDrop(Location(BoardPartID::Stage,1,2),3).GetPieceIndex() == 3, "Action is not constexpr");


  class Actions : public std::vector<std::shared_ptr<Action>>
  {
  public:
    constexpr Actions() = default;
    constexpr ~Actions() = default;
    constexpr bool operator == (const Actions& a) const noexcept {
      if (size() != a.size()) return false;
      for (auto i = 0U; i < a.size(); ++i) if (*(*this)[i] != *a[i]) return false;
      return true;
    }
    constexpr bool operator != (const Actions& a) const noexcept { return !(*this == a); }

    bool IsRepeat(const Location& l, const Offset& o) const noexcept;   // is there already a jump over l (in direction o)?
    constexpr bool HasJump() const noexcept
    {
      auto AhasJump = [](const auto& a) -> bool { return a->IsJump(); };
      return std::find_if(this->cbegin(), this->cend(), AhasJump) != this->cend();
    }

#ifdef LOG
  public:
    void Log() const;
#endif // LOG
  };

  static_assert(Actions().size()==0, "Actions is not constexpr");

}