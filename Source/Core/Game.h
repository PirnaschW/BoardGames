
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
    Game(const VariantChosen& v, Board* b) noexcept;
    virtual ~Game() noexcept;

    virtual void Serialize(std::stringstream& s) const noexcept;
    void SetupBoard(const std::vector<PieceIndex>& list) noexcept;

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
      UIChecked,     // is event checked in UI
    };

    bool React(State state, unsigned int event)
    {
      struct StateChange {
        State state;
        unsigned int event;
        struct Hash { std::size_t operator()(const StateChange& c) const noexcept { return std::hash<long>()((c.event << 8L) + static_cast<long>(c.state)); } };
        struct Equality { bool operator()(const StateChange& c1, const StateChange& c2) const noexcept { return c1.state == c2.state && c1.event == c2.event; } };
      };

      static const std::unordered_map<StateChange, std::function<bool(void)>, StateChange::Hash, StateChange::Equality> reactMap_{
        { {State::SelectFrom,    BoardGamesMFC::Mouse_LButton_Up   }, [this]() -> bool { return React_SelectFrom  (); } },
        { {State::SelectTarget,  BoardGamesMFC::Mouse_RButton_Up   }, [this]() -> bool { return React_Reset       (); } },
        { {State::SelectTarget,  BoardGamesMFC::Mouse_LButton_Up   }, [this]() -> bool { return React_SelectTarget(); } },
        { {State::ReadyToSubmit, BoardGamesMFC::Mouse_RButton_Up   }, [this]() -> bool { return React_Reset       (); } },
        { {State::ReadyToSubmit, ID_EDIT_MOVE                      }, [this]() -> bool { return React_AIMove      (); } },
        { {State::SelectFrom,    ID_EDIT_BOARD                     }, [this]() -> bool { return React_StartEdit   (); } },
        { {State::BoardEditing,  ID_EDIT_BOARD                     }, [this]() -> bool { return React_Reset       (); } },
        { {State::BoardEditing,  BoardGamesMFC::Mouse_LButton_Down }, [this]() -> bool { return React_StartDrag   (); } },
        { {State::Dragging,      BoardGamesMFC::Mouse_Move         }, [this]() -> bool { return React_Drag        (); } },
        { {State::Dragging,      BoardGamesMFC::Mouse_LButton_Up   }, [this]() -> bool { return React_Drop        (); } },
        { {State::UIAvailable,   ID_EDIT_MOVE                      }, [this]() -> bool { return React_CanMove     (); } },
        { {State::UIAvailable,   ID_EDIT_BOARD                     }, [this]() -> bool { return React_CanEdit     (); } },
        { {State::UIChecked,     ID_EDIT_BOARD                     }, [this]() -> bool { return React_Editing     (); } },
      };
      const auto validStateChange = reactMap_.find({ state, event });
      return validStateChange != reactMap_.end() && validStateChange->second();
    }

    bool React_CanMove()
    {
      return state_ == State::ReadyToSubmit;
    }

    bool React_CanEdit()
    {
      return state_ == State::SelectFrom || state_ == State::BoardEditing;
    }

    bool React_Editing()
    {
      return state_ == State::BoardEditing;
    }

    bool React_SelectFrom()
    {
      Location l{ BoardPartID::Stage, 0U,0U };
      if (!board_->GetLocationFromPoint(point_, l)) return false;        // user clicked somewhere outside
      for (const auto& m : board_->GetMoveList(board_->WhiteOnTurn()))   // filter moves of the selected piece into 'moves'
      {
        const Location& lf = m->GetFrL();
        if (lf == l || (lf.b_ == BoardPartID::Stock && m->GetToL() == l)) moves_.push_back(m);
      }
      if (moves_.empty()) return false;
      state_ = State::SelectTarget;
      return true;
    }

    bool React_Reset()
    {
      moves_.clear();
      state_ = State::SelectFrom;
      return true;
    }

    bool React_SelectTarget()
    {
      Location l{ BoardPartID::Stage, 0U,0U };
      if (!board_->GetLocationFromPoint(point_, l)) return false;        // user clicked somewhere outside
      for (const auto& m : moves_)   // check through allowed moves
      {
        if (m->GetToL() == l)
        {
          MoveP mm = m;
          moves_.clear();
          moves_.push_back(mm);
          state_ = State::ReadyToSubmit;
          return true;
        }
      }
      return false;
    }

    bool React_AIMove()
    {
      AIAction();  // execute computer move
      state_ = State::SelectFrom;
      return true;
    }
    bool React_StartEdit()
    {
      state_ = State::BoardEditing;
      return true;
    }
    bool React_StartDrag()
    {
      Location l{ BoardPartID::Stage, 0U,0U };
      if (!board_->GetLocationFromPoint(point_, l)) return false; // clicked somewhere invalid
      PieceIndex pI = board_->GetPieceIndex(l.x_, l.y_, l.b_);
      if (pI == PMap[Piece::NoTile]) return false;;
      dragPoint_ = point_;
      dragPiece_ = pI;
      state_ = State::Dragging;
      return true;
    }
    bool React_Drag()
    {
      dragPoint_ = point_;
      return true;
    }
    bool React_Drop()
    {
      Location l{ BoardPartID::Stage, 0U,0U };
      if (board_->GetLocationFromPoint(point_, l))
      {
        if (l.b_ != BoardPartID::Stock)
          board_->SetPieceIndex(dragPiece_, l.x_, l.y_, l.b_); // dropped on a valid target
      }
      dragPoint_ = {};
      dragPiece_ = PMap[Piece::NoPiece];
      state_ = State::BoardEditing;
      return true;
    }

  protected:
    virtual void AddPlayer(const PlayerType& t, const PieceColor& c) noexcept { players_.push_back(new Player(t,c)); }
    virtual bool AIOnTurn() const noexcept { return !board_->WhiteOnTurn(); }
    virtual void AIAction() { while (!(mode_.IsSet(Mode_::GameOver)) && AIOnTurn()) mode_.Set(ai_.MakeMove(board_)); }// execute computer moves while it is its turn
    //[[deprecated]] [[TODO::Unittest]] virtual void SetCurrentPlayer(unsigned int p) noexcept { current_ = p; }
    //[[deprecated]] [[TODO::Unittest]] virtual const Player& NextPlayer() noexcept;
    //[[deprecated]] [[TODO::Unittest]] virtual void Execute(const Move& m);
    //[[deprecated]] [[TODO::Unittest]] virtual bool AIMove();

  //[[TODO::Unittest]] virtual void AddToStock(const Location& l, const Piece& p) noexcept;

  protected:
    VariantChosen v_;                                 // Game ID and Variant
    AI ai_{};                                         // 'Artificial Intelligence' - evaluator and collected boards 
    Board* board_;                                    // current playing board (not owned! AI owns all boards)                                                      

  private:                                            
    Mode mode_{ Mode_::SelectFr & Mode_::ShowStock }; // current game mode is: pick FRom; show stock
    State state_{ State::SelectFrom };  // current game State
    Point point_{};                // last Mouse event point
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
