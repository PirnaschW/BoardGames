
namespace BoardGamesCore
{

  class PlayerType final
  {
  private:
    constexpr inline PlayerType(const char& p) noexcept : playertype{ p } {}
  public:
    inline void Serialize(CArchive* ar) const;

  private:
    const char playertype;

  public:  // the only instances ever to exists; handed around by pointer
    static const PlayerType Human;
    static const PlayerType Computer;
  };

  class Position
  {
  private:
    class PieceMap final  // local helper class
    {
    public:
      typedef unsigned char PieceIndex;
    public:
      constexpr inline PieceMap(void) {}
      inline PieceIndex GetIndex(const Piece* p) { for (PieceIndex z = 0; z < used; z++) if (map[z] == p) return z; map[used] = p; return used++; }
      inline const Piece* GetPiece(PieceIndex i) { return map[i]; }
    private:
      constexpr static const PieceIndex max{ 32 };
      PieceIndex used{ 0 };
      std::array<const Piece*, max> map{};
    };

  public:
    inline Position(Coordinate x, Coordinate y, const Piece* init = &Piece::NoPiece) noexcept
      : sizeX(x), sizeY(y), pm{ new PieceMap() }, pieces(1ULL*x*y, pm->GetIndex(init)) {}
    inline Position(const Position& p) : sizeX(p.sizeX), sizeY(p.sizeY), pm(p.pm), pieces(p.pieces) {}
    virtual ~Position(void) noexcept {}
    virtual inline bool operator ==(const Position* p) const noexcept { return pm == p->pm && pieces == p->pieces; }
    virtual std::size_t GetHash(void) const noexcept;
    virtual void Serialize(CArchive* ar) const { for (auto& p : pieces) pm->GetPiece(p)->Serialize(ar); }
    inline const Piece* GetPiece(const Location& l) const noexcept { return l.Valid(sizeX, sizeY) ? pm->GetPiece(pieces[l.Index(sizeX, sizeY)]) : nullptr; }
    virtual inline const Piece* SetPiece(const Location& l, const Piece* p) noexcept { hash = 0; pieces[l.Index(sizeX, sizeY)] = pm->GetIndex(p); return p; }
    virtual void SetPosition(std::vector<const Piece*> list);
    inline Coordinate GetSizeX(void) const noexcept { return sizeX; }
    inline Coordinate GetSizeY(void) const noexcept { return sizeY; }

  protected:
    const Coordinate sizeX;
    const Coordinate sizeY;
  private:
    std::shared_ptr<PieceMap> pm;
    std::vector<PieceMap::PieceIndex> pieces;
    mutable std::size_t hash{};
  };


  class AIContext;   // forward declaration needed for class MainPosition
  class MainPosition : public Position
  {
  public:
    typedef unsigned int Depth;

  protected:
    inline MainPosition(const MainPosition& m) noexcept : Position(m), sequence(m.sequence), onTurn(m.onTurn) {}

  public:
    inline MainPosition(Coordinate x, Coordinate y) noexcept : Position(x, y) {}
    ~MainPosition(void) noexcept override {}
    virtual MainPosition* Clone(void) const = 0;
    virtual inline bool operator ==(const MainPosition& p) const noexcept { return OnTurn() == p.OnTurn() && Position::operator==(&p); }
    virtual inline std::size_t GetHash(void) const noexcept { return Position::GetHash() + std::hash<const Color*>()(OnTurn()); }

    inline void SetOnTurn(const Color* c) noexcept { onTurn = c; }
    inline const Color* OnTurn(void) const noexcept { return onTurn; }
    virtual void NextPlayer(void) noexcept;
    virtual void PreviousPlayer(void) noexcept;  // needed for Undo

    virtual void GetAllMoves(void);              // generate all moves and save list
    virtual Moves CollectMoves(void) const { Moves m{}; return m; }
    bool JumpsOnly(Moves& moves) const noexcept;
    inline Moves& GetMoveList(bool w) { return w ? movelistW : movelistB; }
    virtual bool AddIfLegal(Moves&, const Location, const Location) const { return false; };
    virtual void EvaluateStatically(void);       // calculate position value and save
    virtual PositionValue Evaluate(AIContext& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies);
    inline PositionValue GetValue(bool w) const noexcept { return value.Relative(w); }
    virtual inline unsigned int GetMoveCountFactor(void) const noexcept { return 20; }
    inline Depth GetDepth(void) const noexcept { return depth; }
    inline Depth SetDepth(Depth d) noexcept { return depth = d; }
    inline PositionValue SetValue(bool w, PositionValue v) noexcept { return value = v.Relative(w); }
    virtual inline MoveP GetBestMove(bool w) const { return (w ? movelistW[0] : movelistB[0]); }
    virtual MainPosition* GetPosition(AIContext& plist, MoveP m = nullptr) const;     // execute move, maintain in PList
    virtual const std::vector<const Piece*> Execute(MoveP m);
    virtual void Undo(const MoveP m);

  public:
    Moves sequence{};

  protected:
    const Color* onTurn{ &Color::White };
    Depth depth{ 0 };
    PositionValue value{ PositionValue::Undefined };                 // position value for White
    Moves movelistW{};
    Moves movelistB{};
  };

  class TakenPosition : public Position
  {
  public:
    inline TakenPosition(Coordinate x, Coordinate y) noexcept : Position(x, y, &Piece::NoTile) {}
    void Push(unsigned int player, const std::vector<const Piece*>& p) noexcept;
  };

  class StockPosition : public Position
  {
  public:
    inline StockPosition(Coordinate x, Coordinate y) noexcept : Position(x, y) {}
  };

  namespace AIContextHelper
  {
    typedef MainPosition* pMP;
    struct Hash { std::size_t operator()(const pMP p) const noexcept { return p->GetHash(); } };
    struct Equality { bool operator()(const pMP Lhs, const pMP Rhs) const noexcept { return *Lhs == *Rhs; } };
  }
  
  class AIContext : public std::unordered_set< MainPosition*, AIContextHelper::Hash, AIContextHelper::Equality>
  {
  public:
    std::function<void(void)> callback;          // pointer to CDocument's callback function (to update window)
    int additionalData{};
    size_t freemem{};
  };


  class Player final
  {
  public:
    constexpr Player(const PlayerType* p = &PlayerType::Human, const Color* c = &Color::NoColor) noexcept : playertype{ p }, color{ c } {}    // constructor
    constexpr Player(const Player& p) noexcept : playertype{ p.playertype }, color{ p.color } {}                                              // copy constructor
    Player& operator=(const Player& p) noexcept { return *this = Player(p.playertype, p.color); }                                // copy assignment
    Player(Player&& p) noexcept : playertype{ std::move(p.playertype) }, color{ std::move(p.color) } {}                          // move constructor
    Player& operator=(Player&& p) noexcept { std::swap(playertype, p.playertype); std::swap(color, p.color); return *this; }     // move assignment

    inline const Color* SetColor(const Color* c) noexcept { return color = c; }
    inline const Color* GetColor(void) const noexcept { return color; }
    inline const PlayerType* SetPlayerType(const PlayerType* p) noexcept { return playertype = p; }
    inline const PlayerType* GetPlayerType(void) const noexcept { return playertype; }
    inline bool Is(const PlayerType* p) const noexcept { return playertype == p; }

  private:
    const std::string name{};
    const PlayerType* playertype;
    const Color* color;
  };


  class IUI  // Interface for UI
  {
  public:
    virtual void Draw(CDC* pDC) const = 0;
    virtual bool React(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) = 0;      // react to keyboard input (not menu shortcuts, but typing)
    virtual bool React(UINT command) = 0;                                           // react to button/menu command
    virtual bool React(UINT event, UINT nFlags, const CPoint& p) = 0;               // react to mouse events
    virtual void React(CCmdUI* pCmdUI) = 0;                                         // react to UI events (allows to set buttons greyed, etc.)
    virtual void DragTo(const CPoint& point) = 0;
    virtual void DragStart(const CPoint&) = 0;
    virtual void DragEnd(const CPoint&) = 0;
    virtual void Select(const CPoint& point) = 0;
    virtual inline void Unselect(void) = 0;
    virtual void SetUpdateCallBack(std::function<void(void)> cb) = 0;
  protected:
    bool dragging{ false };         // currently dragging a piece
    const Piece* dragPiece{};       // currently dragged piece
    CPoint dragPoint{};             // point the piece is dragged to
  };


  class Variant final   // PoD of possible game variants and settings
  {
  public:
    constexpr Variant(Coordinate xd, Coordinate yd = 0, const char* n = nullptr, Coordinate xmin = 0, Coordinate xmax = 0, Coordinate ymin = 0, Coordinate ymax = 0) noexcept :
      xDef(xd), yDef(yd ? yd : xd), name(n), xMin(xmin), xMax(xmax), yMin(ymin ? ymin : xmin), yMax(ymax ? ymax : xmax) {}
    Variant& operator=(const Variant&) = default;  //{ *this = v; return *this; }
  public:
    Coordinate xDef;
    Coordinate yDef;
    const char* name;
    Coordinate xMin;
    Coordinate xMax;
    Coordinate yMin;
    Coordinate yMax;
  };
  typedef std::vector<Variant> VariantList;

  template <class TClass>  // template enforces every Game to have the required static functions (abstract static functions are not possible)
  class Variants {
  public:
    constexpr inline static const VariantList& GetVariants(void) noexcept { return TClass::GetVariants(); }
  };


  class Layout;       // forward declaration need inside class Game
  class TakenLayout;  // forward declaration need inside class Game
  class StockLayout;  // forward declaration need inside class Game
  class Game : public IUI
  {
  private:
    Game(void) = delete;
  public:
    Game(MainPosition* p, TakenPosition* t, StockPosition* s, Layout* l, TakenLayout* tl, StockLayout* sl, bool pl = false) noexcept;
    virtual ~Game(void) noexcept;
    virtual void Serialize(CArchive* ar) { pos->Serialize(ar); }
    virtual void ReadFromWWW(const std::string& gameno);
    virtual inline const std::unordered_map<std::string, const Piece*>& GetHTMLPieceMap(void) const noexcept { return Piece::GetHTMLPieceMap(); }
    virtual inline void AddToStock(const Location& l, const Piece* p) noexcept { spos->SetPiece(l, p); }
    virtual inline void ShowStock(bool show) noexcept { showStock = show; }
    virtual inline void AIAction(void) { while (IsAlive() && CurrentPlayer()->Is(&PlayerType::Computer)) SetAlive(AIMove()); }  // execute computer moves while it is its turn
    virtual inline bool IsAlive(void) const noexcept { return !gameover; }
    virtual inline void SetAlive(bool a) noexcept { gameover = !a; }
    virtual inline void AddPlayer(Player* p) noexcept { players.push_back(p); }
    virtual inline void SetCurrentPlayer(unsigned int p) noexcept { current = p; }
    virtual inline Player* CurrentPlayer(void) const noexcept { return players[current]; }
    virtual inline Player* NextPlayer(void) noexcept { current = ++current % players.size(); return players[current]; }
    virtual bool AIMove(void);
    virtual void Execute(MoveP m);

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
    virtual inline void SetUpdateCallBack(std::function<void(void)> cb) override { assert(cb != nullptr); plist.callback = cb; }

  protected:
    MainPosition* pos;                           // logical position on the main playing board
    TakenPosition* tpos;                         // taken pieces
    StockPosition* spos;                         // piece list for editing

    Layout* lay;                                 // physical layout of the main playing board
    TakenLayout* tlay;                           // physical layout of the taken pieces
    StockLayout* slay;                           // physical layout of the piece list

    bool placing;                                // game allows to place new Piece

    AIContext plist{};                           // collected positions with their evaluations
    bool showStock{ false };                     // flag to show/hide the stock
    bool moveTaken{ false };                     // flag to allow moves from taken pieces

  private:
    bool editing{ false };                       // edit mode allows to change the main playing board
    std::vector<Player*> players{};              // list of players
    unsigned int current{ 0 };                   // current player
    Moves moves{};                   // will contain all allowed moves once a start piece is selected
    bool gameover{ false };                      // once game is over, moves are disallowed
  };

}
