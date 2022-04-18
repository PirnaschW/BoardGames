
namespace BoardGamesCore
{

  class Game abstract : public IUI
  {
 
  private:
    // don't allow trivial construction, copying, and moving
    Game() = delete;                    // trivial construction
    Game(const Game&) = delete;             // copy constructor
    Game& operator=(const Game&) = delete;  // copy assignment
    Game(Game&&) = delete;                  // move constructor
    Game& operator=(Game&&) = delete;       // move assignment

  public:
    Game(const VariantChosen& v, Board* b) noexcept;
    virtual ~Game() noexcept;

    virtual void Serialize(Archive& ar);
    void SetBoard(const std::vector<PieceIndex>& list) noexcept;

    // UI functionality
    virtual void Draw(DC* dc) const override;
    virtual bool React(unsigned int /*nChar*/, unsigned int /*nRepCnt*/, unsigned int /*nFlags*/) override;                     // react to keyboard input (not menu shortcuts, but typing)
    virtual bool React(unsigned int command) override;                                                          // react to button/menu command
    virtual bool React(unsigned int event, unsigned int nFlags, const Point& p) override;                              // react to mouse events
    virtual void React(CmdUI* pUI) override;                                                        // react to UI events (allows to set buttons greyed, etc.)
    virtual void DragTo(const Point& point) override { dragPoint_ = point; }
    virtual void DragStart(const Point&) override;
    virtual void DragEnd(const Point&) override;
    virtual void Select(const Point& point) override;
    virtual void Unselect() override { moves_.clear(); mode_.Del(Mode_::SelectTo); mode_.Set(Mode_::SelectFr); }
    virtual void SetUpdateCallBack(std::function<void()> cb) override { assert(cb != nullptr); ai_.SetCallback(cb); }

  protected:
//    virtual void AddToStock(const Location& l, const Piece& p) noexcept;
    virtual void AIAction();   // execute computer moves while it is its turn

    virtual void AddPlayer(Player* p) noexcept;
    virtual void SetCurrentPlayer(unsigned int p) noexcept;
    virtual const Player& CurrentPlayer() const noexcept;
    virtual const Player& NextPlayer() noexcept;
    virtual bool AIMove();
    virtual void Execute(const Move& m);

  protected:
    VariantChosen v_;                                 // Game ID and Variant
    Board* board_;                                    // current playing board (not owned! plist owns all boards)
                                                      
    AI ai_{};                                         // 'Artificial Intelligence' - evaluator and collected boards 

  private:                                            
    Mode mode_{ Mode_::SelectFr & Mode_::ShowStock };  // current game mode is: pick FRom; show stock
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
