
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
  };

  class Layout;       // forward declaration, needed inside class Game
  class TakenLayout;  // forward declaration, needed inside class Game
  class StockLayout;  // forward declaration, needed inside class Game
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
    Game(const PieceMapP& m, MainPosition* p, Layout* l, bool pl = false) noexcept;
    virtual ~Game(void) noexcept;

    virtual inline void Serialize(CArchive* ar) { pos->Serialize(ar); }
    virtual void ReadFromWWW(const std::string& gameno);
    virtual inline const std::unordered_map<std::string, const Piece*>& GetHTMLPieceMap(void) const noexcept { return Piece::GetHTMLPieceMap(); }
    virtual inline void AddToStock(const Location& l, const Piece* p) noexcept { pos->SetPiece(l, p); }
    virtual inline void ShowStock(bool show) noexcept { showStock = show; }
    virtual inline void AIAction(void) { while (IsAlive() && CurrentPlayer()->IsAI()) SetAlive(AIMove()); }  // execute computer moves while it is its turn
    virtual inline bool IsAlive(void) const noexcept { return !gameover; }
    virtual inline void SetAlive(bool a) noexcept { gameover = !a; }
    virtual inline void AddPlayer(Player* p) noexcept { players.push_back(p); }
    virtual inline void SetCurrentPlayer(unsigned int p) noexcept { current = p; }
    virtual inline Player* CurrentPlayer(void) const noexcept { return players[current]; }
    virtual inline Player* NextPlayer(void) noexcept { current = ++current % players.size(); return players[current]; }
    virtual bool AIMove(void);
    virtual void Execute(MoveP m);
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
    virtual inline void Unselect(void) override { moves.clear(); }
    virtual inline void SetUpdateCallBack(std::function<void(void)> cb) override { assert(cb != nullptr); plist->callback = cb; }

  protected:
    const PieceMapP& pMap;                       // map of all pieces used in the game
    MainPosition* pos;                           // logical position on the main playing board
//    TakenPosition* tpos;                         // taken pieces
//    StockPosition* spos;                         // piece list for editing

    Layout* lay;                                 // physical layout of the main playing board
//    TakenLayout* tlay;                           // physical layout of the taken pieces
//    StockLayout* slay;                           // physical layout of the piece list

    bool placing;                                // game allows to place new Piece

    AIContextP plist{};                          // collected positions with their evaluations
    bool showStock{ false };                     // flag to show/hide the stock
    bool moveTaken{ false };                     // flag to allow moves from taken pieces

  private:
    bool editing{ false };                       // edit mode allows to change the main playing board
    std::vector<Player*> players{};              // list of players
    unsigned int current{ 0 };                   // current player
    Moves moves{};                               // will contain all allowed moves once a start piece is selected
    bool gameover{ false };                      // once game is over, moves are disallowed
  };

  static_assert(std::is_abstract<Game>::value, "is abstract");
  static_assert(!std::is_trivially_constructible<Game>::value, "must not be trivially constructible");
  static_assert(!std::is_assignable<Game, Game>::value, "is assignable");
  static_assert(!std::is_move_assignable<Game>::value, "is move assignable");

}
