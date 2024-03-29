
namespace BoardGamesCore
{

  class Game abstract : public IUI
  {
  private:
    // don't allow trivial construction, copying, and moving
    Game() = delete;                        // trivial construction
    Game(const Game&) = delete;             // copy constructor
    Game& operator=(const Game&) = delete;  // copy assignment
    Game(Game&&) = delete;                  // move constructor
    Game& operator=(Game&&) = delete;       // move assignment

  public:
    Game(const VariantChosen& v, const std::vector<PieceIndex>& list, Board* b) noexcept;
    virtual ~Game() noexcept;

    virtual void Serialize(std::stringstream& s) const noexcept;

    // UI functionality
    virtual void Draw(DC* dc) const override;
    virtual bool React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) override { return false; }                       // react to keyboard input (not menu shortcuts, but typing)
    virtual bool React(unsigned int command) override { return React(state_, command); }                                               // react to button/menu command
    virtual bool React(unsigned int event, unsigned int nFlags, const Point& p) override { point_ = p; return React(state_, event); }  // react to mouse events
    virtual void React(CmdUI* pCmdUI) override // tell UI which buttons / menu items are available / greyed / checked
    {
      const auto id = pCmdUI->GetID();
      if (React(State::UIAvailable, id)) pCmdUI->Enable(true);
      if (React(State::UIChecked, id)) pCmdUI->SetCheck();
    };                                                    
    virtual void SetUpdateCallBack(std::function<void()> cb) override { assert(cb != nullptr); ai_.SetCallback(cb); }


  protected: // Finite State Machine
    enum class State
    {
      SelectFrom,    // standard - game is ready for human player to select a piece
      SelectTarget,  // piece selected; player should now select target
      ReadyToSubmit, // player move is ready to be submitted
      BoardEditing,  // board is being edited
      Dragging,      // a piece is being dragged with the mouse
      GameOver,      // the game is over - won or lost
      UIAvailable,   // is event available to user in UI
      UIChecked,     // is event shown with a check mark in UI
    };

    bool React(State state, unsigned int event) noexcept;  // FSM (Finite State Machine) for UI handling
    bool React_CanMove() const noexcept;
    bool React_CanEdit() const noexcept;
    bool React_Editing() const noexcept;
    bool React_SelectFrom() noexcept;
    bool React_Reset() noexcept;
    bool React_SelectTarget() noexcept;
    bool React_AIMove() noexcept;
    bool React_StartEdit() noexcept;
    bool React_StartDrag() noexcept;
    bool React_Drag() noexcept;
    bool React_Drop() noexcept;

  protected:
    virtual void AddPlayer(const PlayerType& t, const PieceColor& c) noexcept { players_.push_back(new Player(t,c)); }

    //[[deprecated]] [[TODO::Unittest]] virtual void SetCurrentPlayer(unsigned int p) noexcept { current_ = p; }
    //[[deprecated]] [[TODO::Unittest]] virtual const Player& NextPlayer() noexcept;
    //[[deprecated]] [[TODO::Unittest]] virtual void Execute(const Move& m);

    //[[TODO::Unittest]] virtual void AddToStock(const Location& l, const Piece& p) noexcept;

  protected:
    VariantChosen v_;                                 // Game ID and Variant
    AI ai_{};                                         // 'Artificial Intelligence' - evaluator and collected boards 
    Board* board_;                                    // current playing board (not owned! AI owns all boards)                                                      

  private:                                            
    State state_{ State::SelectFrom };                // current game State
    Point point_{};                                   // last Mouse event point
    Moves moves_{};                                   // will contain all allowed moves once a start piece is selected
                                                      
    std::vector<Player*> players_{};                  // list of players
    unsigned int current_{ 0 };                       // current player
  };

//  static_assert(sizeof(Game) == 272, "size of Game has changed");

  static_assert(std::is_abstract<Game>::value, "is abstract");
  static_assert(!std::is_trivially_constructible<Game>::value, "must not be trivially constructible");
  static_assert(!std::is_assignable<Game, Game>::value, "is assignable");
  static_assert(!std::is_move_assignable<Game>::value, "is move assignable");

  template <class TClass>  // template enforces every Game to have the required static functions (workaround, as abstract static functions are not possible)
  class Registration {
  public:
    constexpr static void Register() noexcept { TClass::Register(); }
    constexpr static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept { return TClass::GetDimensions(v); }
  };

}
