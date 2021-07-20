
namespace BoardGamesCore
{

  namespace AIContextHelper
  {
    using pMP = MainPosition*;
    struct Hash { std::size_t operator()(const pMP p) const noexcept { return p->GetHash(); } };
    struct Equality { bool operator()(const pMP Lhs, const pMP Rhs) const noexcept { return *Lhs == *Rhs; } };
  }

  class AIContext final
  {
    using MPSet = std::unordered_set< MainPosition*, AIContextHelper::Hash, AIContextHelper::Equality>;
  public:
    ~AIContext(void) { for (MainPosition* p : map_) delete p; }
    MPSet::const_iterator find(MainPosition* p) const noexcept { return map_.find(p); }
    MPSet::const_iterator begin(void) const noexcept { return map_.begin(); }
    MPSet::const_iterator end(void) const noexcept { return map_.end(); }
    size_t erase(MainPosition* p) noexcept { return map_.erase(p); }
    MPSet::const_iterator erase(MPSet::const_iterator it) noexcept { return map_.erase(it); }
    size_t size(void) const noexcept { return map_.size(); }
    std::pair<std::unordered_set<MainPosition*>::iterator, bool> insert(MainPosition* p) noexcept { return map_.insert(p); }


    std::function<void(void)> callback;          // pointer to CDocument's callback function (to update window)
    size_t freemem{};
    void Purge(const Moves& sequence_) noexcept;
  private:
    MPSet map_{};
  };

  class _Mode
  {
  public:
    enum class __Mode : unsigned char {
      SelectFr = 0x01,
      SelectTo = 0x02,
      Editing = 0x04,
      Dragging = 0x08,
      ShowStock = 0x10,
      ShowTaken = 0x20,
      GameOver = 0x80,
    };
    constexpr _Mode(__Mode a) noexcept : mode(a) {};               // allow to create an object from the enum
    constexpr _Mode& Set(_Mode a) noexcept;                        // set a bit
    constexpr _Mode& Del(_Mode a) noexcept;                        // remove a bit
    constexpr bool IsSet(_Mode::__Mode a) const noexcept;          // test for a bit
  public:
    __Mode mode;
  };
  constexpr _Mode::__Mode operator|(_Mode::__Mode a, _Mode::__Mode b) noexcept { return static_cast<_Mode::__Mode>(static_cast<typename std::underlying_type<_Mode::__Mode>::type>(a) | static_cast<typename std::underlying_type<_Mode::__Mode>::type>(b)); }
  constexpr _Mode::__Mode operator&(_Mode::__Mode a, _Mode::__Mode b) noexcept { return static_cast<_Mode::__Mode>(static_cast<typename std::underlying_type<_Mode::__Mode>::type>(a) & static_cast<typename std::underlying_type<_Mode::__Mode>::type>(b)); }
  constexpr _Mode::__Mode operator~(_Mode::__Mode a) noexcept { return static_cast<_Mode::__Mode>(~static_cast<typename std::underlying_type<_Mode::__Mode>::type>(a)); }
  constexpr bool operator!(_Mode::__Mode a) noexcept { return !(static_cast<typename std::underlying_type<_Mode::__Mode>::type>(a)); }

  constexpr _Mode& _Mode::Set(_Mode a) noexcept { mode = mode | a.mode;  return *this; }
  constexpr _Mode& _Mode::Del(_Mode a) noexcept { mode = mode & ~a.mode; return *this; }
  constexpr bool _Mode::IsSet(_Mode::__Mode a) const noexcept { return !!(mode & a); }
  using Mode = _Mode::__Mode;

  class Game abstract : public IUI
  {
 
  private:
    // don't allow trivial construction, copying, and moving
    Game(void) = delete;                    // trivial construction
    Game(const Game&) = delete;             // copy constructor
    Game& operator=(const Game&) = delete;  // copy assignment
    Game(Game&&) = delete;                  // move constructor
    Game& operator=(Game&&) = delete;       // move assignment
  public:
    Game(const VariantChosen& v, const PieceMapP& m, MainPosition* p, MainLayout* l) noexcept;
    virtual ~Game(void) noexcept;

    virtual void Serialize(Archive& ar);
    virtual void AddToStock(const Location& l, const Piece& p) noexcept { pos->SetPiece(l, p); }
    virtual void AIAction(void) { while (!(_mode.IsSet(Mode::GameOver)) && CurrentPlayer()->IsAI()) if (!AIMove()) _mode = Mode::GameOver; }  // execute computer moves while it is its turn

    virtual void AddPlayer(Player* p) noexcept { players.push_back(p); }
    virtual void SetCurrentPlayer(unsigned int p) noexcept { current = p; }
    virtual Player* CurrentPlayer(void) const noexcept { return players[current]; }
    virtual Player* NextPlayer(void) noexcept { current = ++current % players.size(); return players[current]; }
    virtual bool AIMove(void);
    virtual void Execute(const Move& m);


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
    virtual void Unselect(void) override { moves.clear(); _mode.Del(Mode::SelectTo); _mode.Set(Mode::SelectFr); }
    virtual void SetUpdateCallBack(std::function<void(void)> cb) override { assert(cb != nullptr); plist.callback = cb; }
    virtual void SetPosition(std::vector<const Piece*>& list) override { if (list.empty()) pos->SetStartingPosition(v_); else pos->SetPosition(list); }


  protected:
    VariantChosen v_;                            // Game ID and Variant
    const PieceMapP& pMap_;                      // map of all pieces used in the game
    MainPosition* pos;                           // logical position on the main playing board
    MainLayout* lay;                             // physical layout of the main playing board

    AIContext plist{};                           // collected positions with their evaluations

  private:
    _Mode _mode{ Mode::SelectFr };               // current game mode
    Moves moves{};                               // will contain all allowed moves once a start piece is selected

    std::vector<Player*> players{};              // list of players
    unsigned int current{ 0 };                   // current player
  };

  static_assert(std::is_abstract<Game>::value, "is abstract");
  static_assert(!std::is_trivially_constructible<Game>::value, "must not be trivially constructible");
  static_assert(!std::is_assignable<Game, Game>::value, "is assignable");
  static_assert(!std::is_move_assignable<Game>::value, "is move assignable");

  template <class TClass>  // template enforces every Game to have the required static functions (workaround, as abstract static functions are not possible)
  class Variants {
  public:
    constexpr static const VariantList& GetVariants(void) noexcept { return TClass::GetVariants(); }
    constexpr static const Dimensions GetDimensions(const VariantChosen& v) noexcept { return TClass::GetDimensions(v); }
    constexpr static const PieceMapP& GetPieces(const VariantChosen& v) noexcept { const PieceMapP& p{ TClass::GetPieces(v) }; p->Add(Piece::NoPiece); p->Add(Piece::NoTile); return p; }
  };

}
