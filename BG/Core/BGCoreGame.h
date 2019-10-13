
namespace BoardGamesCore
{

  namespace AIContextHelper
  {
    typedef MainPosition* pMP;
    struct Hash { inline std::size_t operator()(const pMP p) const noexcept { return p->GetHash(); } };
    struct Equality { inline bool operator()(const pMP Lhs, const pMP Rhs) const noexcept { return *Lhs == *Rhs; } };
  }

  class AIContext final : public std::unordered_set< MainPosition*, AIContextHelper::Hash, AIContextHelper::Equality>
  {
  public:
    std::function<void(void)> callback;          // pointer to CDocument's callback function (to update window)
//    int additionalData{};
    size_t freemem{};
    void Purge(const Moves& sequence) noexcept;
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
    constexpr inline _Mode(__Mode a) noexcept : mode(a) {};               // allow to create an object from the enum
    constexpr inline _Mode& Set(_Mode a) noexcept;                        // set a bit
    constexpr inline _Mode& Del(_Mode a) noexcept;                        // remove a bit
    constexpr inline bool IsSet(_Mode::__Mode a) const noexcept;          // test for a bit
  public:
    __Mode mode;
  };
  constexpr inline _Mode::__Mode operator|(_Mode::__Mode a, _Mode::__Mode b) noexcept { return static_cast<_Mode::__Mode>(static_cast<typename std::underlying_type<_Mode::__Mode>::type>(a) | static_cast<typename std::underlying_type<_Mode::__Mode>::type>(b)); }
  constexpr inline _Mode::__Mode operator&(_Mode::__Mode a, _Mode::__Mode b) noexcept { return static_cast<_Mode::__Mode>(static_cast<typename std::underlying_type<_Mode::__Mode>::type>(a) & static_cast<typename std::underlying_type<_Mode::__Mode>::type>(b)); }
  constexpr inline _Mode::__Mode operator~(_Mode::__Mode a) noexcept { return static_cast<_Mode::__Mode>(~static_cast<typename std::underlying_type<_Mode::__Mode>::type>(a)); }
  constexpr inline bool operator!(_Mode::__Mode a) noexcept { return !(static_cast<typename std::underlying_type<_Mode::__Mode>::type>(a)); }

  constexpr inline _Mode& _Mode::Set(_Mode a) noexcept { mode = mode | a.mode;  return *this; }
  constexpr inline _Mode& _Mode::Del(_Mode a) noexcept { mode = mode & ~a.mode; return *this; }
  constexpr inline bool _Mode::IsSet(_Mode::__Mode a) const noexcept { return !!(mode & a); }
  using Mode = _Mode::__Mode;

  class MainLayout;   // forward declaration, needed inside class Game
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
    Game(const PieceMapP& m, MainPosition* p, MainLayout* l) noexcept;
    virtual ~Game(void) noexcept;

    virtual inline void Serialize(CArchive* ar) { pos->Serialize(ar); }
    virtual void ReadFromWWW(const std::string& gameno);
    virtual inline const std::unordered_map<std::string, const Piece*>& GetHTMLPieceMap(void) const noexcept { return Piece::GetHTMLPieceMap(); }
    virtual inline void AddToStock(const Location& l, const Piece* p) noexcept { pos->SetPiece(l, p); }
    virtual inline void AIAction(void) { while (!(_mode.IsSet(Mode::GameOver)) && CurrentPlayer()->IsAI()) if (!AIMove()) _mode = Mode::GameOver; }  // execute computer moves while it is its turn

    virtual inline void AddPlayer(Player* p) noexcept { players.push_back(p); }
    virtual inline void SetCurrentPlayer(unsigned int p) noexcept { current = p; }
    virtual inline Player* CurrentPlayer(void) const noexcept { return players[current]; }
    virtual inline Player* NextPlayer(void) noexcept { current = ++current % players.size(); return players[current]; }
    virtual bool AIMove(void);
    virtual void Execute(const Move& m);


    // UI functionality
    virtual void Draw(CDC* pDC) const override;
    virtual bool React(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) override;                     // react to keyboard input (not menu shortcuts, but typing)
    virtual bool React(UINT command) override;                                                          // react to button/menu command
    virtual bool React(UINT event, UINT nFlags, const CPoint& p) override;                              // react to mouse events
    virtual void React(CCmdUI* pCmdUI) override;                                                        // react to UI events (allows to set buttons greyed, etc.)
    virtual inline void DragTo(const CPoint& point) override { dragPoint = point; }
    virtual void DragStart(const CPoint&) override;
    virtual void DragEnd(const CPoint&) override;
    virtual void Select(const CPoint& point) override;
    virtual inline void Unselect(void) override { moves.clear(); _mode.Del(Mode::SelectTo); _mode.Set(Mode::SelectFr); }
    virtual inline void SetUpdateCallBack(std::function<void(void)> cb) override { assert(cb != nullptr); plist->callback = cb; }

  protected:
    const PieceMapP& pMap;                       // map of all pieces used in the game
    MainPosition* pos;                           // logical position on the main playing board
    MainLayout* lay;                             // physical layout of the main playing board

    AIContextP plist{};                          // collected positions with their evaluations

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

}
