
namespace BoardGamesCore
{

  class Offset final
  {
    friend class Location;
  public:
    constexpr Offset(int xx, int yy) noexcept : dx{ xx }, dy{ yy } {}

    constexpr inline const Offset operator*(int i) const noexcept { return Offset(dx * i, dy * i); }

  private:
    const int dx;
    const int dy;

  public:
    static const Offset Rdirection[4];   // standard 4 'Rook'   directions
    static const Offset Bdirection[4];   // standard 4 'Bishop' directions
    static const Offset Qdirection[8];   // standard 8 'Queen'  directions
  };
  static_assert(!std::is_trivially_constructible<class Offset>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Offset, int, int>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Offset, int, int>::value, "is not nothrow constructible");

  typedef unsigned int Coordinate;

  class Location final
  {
  public:
    constexpr inline Location(Coordinate xx, Coordinate yy) noexcept : x{ xx }, y{ yy } {}

    constexpr inline bool operator==(const Location& l) const noexcept { return l.x == x && l.y == y; }
    constexpr inline bool operator!=(const Location& l) const noexcept { return !(l == *this); }
    constexpr inline Location operator+(const Offset& o) const noexcept { Location l(*this); return l += o; }
    constexpr inline Location& operator+=(const Offset& o) noexcept { x += o.dx, y += o.dy; return *this; }

    constexpr inline bool Valid(Coordinate sizeX, Coordinate sizeY) const noexcept { return x >= 0 && x < sizeX && y >= 0 && y < sizeY; }
    constexpr inline Coordinate Index(Coordinate sizeX, Coordinate /*sizeY*/) const noexcept { return y * sizeX + x; }

  public:
    // can't be protected, as the values need to be used in many places
    // can't be const, or assignments between Locations wouldn't work.
    Coordinate x;
    Coordinate y;
  };

  class Piece;  // forward declaration needed in class Field
  class Field final // combines a Location and a Piece on it
  {
  public:
    constexpr inline Field(const Location l, const Piece* p) noexcept : l{ l }, p{ p } {}
    constexpr inline bool operator==(const Field& f) const noexcept { return l == f.l && p == f.p; }
    constexpr inline bool operator!=(const Field& f) const noexcept { return !(f == *this); }

    constexpr inline const Location GetLocation(void) const noexcept { return l; }
    constexpr inline const Piece* GetPiece(void) const noexcept { return p; }

  private:
    Location l;      // can't be const, or assignments between Fields wouldn't work.
    const Piece* p;
  };


  class Step final // define one hop in a complete move
  {
  public:
    enum StepType {
      Normal = 0x01,    // normal 'sliding' move
      Place = 0x02,     // move places new piece (some games only)
      Take = 0x04,      // move will take one or more pieces
      Jump = 0x08,      // move jumps over one or more pieces
      Promote = 0x10,   // move promotes the piece (Chess, Shogi)
      Drop = 0x20,      // move drops a formerly taken piece back on the board
    };
  private:
    Step(void) = delete; // disable blank constructor
  public:
    inline Step(const Field& Fr, const Field& To, Step::StepType Ty = Step::StepType::Normal, const std::vector<Field>& Take = {}) noexcept : from{ Fr }, to{ To }, type{ Ty }, take{ Take } {}
    inline bool operator == (const Step& s) const noexcept { return from == s.from && to == s.to && type == s.type && take == s.take; }
    inline bool operator != (const Step& s) const noexcept { return !(s == *this); }

    inline const Field& GetFr(void) const noexcept { return from; }
    inline const Field& GetTo(void) const noexcept { return to; }
    inline const StepType GetType(void) const noexcept { return type; }
    inline const std::vector<Field>& GetTake() const noexcept { return take; }
    inline bool IsTake(void) const noexcept { return type & StepType::Take; }

  private:
    const Field from;
    const Field to;
    const StepType type;
    const std::vector<Field> take;
  };


  class PositionValue final
  {
  public:
    enum PValueType {
      Undefined = 0x01,
      Normal = 0x02,
      Won = 0x04,
      Lost = 0x08,
      Tie = 0x10,
    };
    constexpr inline bool operator== (PValueType t) const noexcept { return type == t; }
    constexpr inline bool operator!= (PValueType t) const noexcept { return !(*this == t); }

  public:
    constexpr inline PositionValue(PValueType t = PValueType::Undefined) noexcept : type{ t } {}
    constexpr inline PositionValue(long v) noexcept : type{ PValueType::Normal }, value{ v } {}

    constexpr inline bool operator== (const PositionValue& v) const noexcept { return type == v.type && value == v.value; }
    constexpr inline bool operator!= (const PositionValue& v) const noexcept { return !(*this == v); }
    constexpr inline bool operator> (const PositionValue& v) const noexcept
    {
      switch (type)
      {
        case Lost: return false;                                                        // Lost is not greater than anything
        case Undefined: return v.type == Lost;                                          // Undefined is only greater than lost
        case Tie: return (v.type == Lost ||
          v.type == Undefined ||
          (v.type == Normal && v.value < 0));                            // Tie is better than Lost. Undefined, and losing
        case Normal: return (v.type == Lost ||
          v.type == Undefined ||
          (v.type == Tie && value > 0) ||                             // Any value is better than Lost; positive is better than Tie
          (v.type == Normal && value > v.value));                     // and better is better, of course
        case Won: return v.type != Won;                                                 // Won is better than anything except Won
        default: return false;
      }
    }
    constexpr inline bool operator< (const PositionValue& v) const noexcept { return (*this != v) && (!(*this > v)); }
    constexpr inline bool operator>= (const PositionValue& v) const noexcept { return !(*this < v); }
    constexpr inline bool operator<= (const PositionValue& v) const noexcept { return !(*this > v); }
    constexpr inline const PositionValue operator- (void) const noexcept
    {
      switch (type)
      {
        case Lost: return PositionValue(Won);
        case Won: return PositionValue(Lost);
        case Tie: return PositionValue(Tie);
        case Normal: return PositionValue{ -value };
        case Undefined: return PositionValue{};
        default: return PositionValue{};
      }
    }
    constexpr inline const PositionValue operator+ (const PositionValue& v) const noexcept { return PositionValue{ value + v.value }; }
    constexpr inline const PositionValue operator- (const PositionValue& v) const noexcept { return PositionValue{ value - v.value }; }

    constexpr inline PositionValue& operator+= (const PositionValue& v) noexcept { value += v.value; return *this; }
    constexpr inline PositionValue& operator-= (const PositionValue& v) noexcept { value -= v.value; return *this; }
    constexpr inline PositionValue& operator+= (int z) noexcept { value += z; return *this; }
    constexpr inline PositionValue& operator-= (int z) noexcept { value -= z; return *this; }
    constexpr inline PositionValue& operator++ (void) noexcept { value++; return *this; }
    constexpr inline PositionValue& operator-- (void) noexcept { value--; return *this; }
    constexpr inline PositionValue Relative(bool w) const noexcept { return (w ? *this : -*this); }

    constexpr inline operator unsigned int(void) const noexcept { return (int)value; }

  private:
    int value{ 0 };
    PValueType type{ PValueType::Undefined };
  };

  class Move
  {
  public:
    inline Move(const Step& s) : step{ s } {}
    inline Move(const std::vector<Step>& St) : step{ St } {}
    Move(const Move& move) = default;
    Move&& operator =(Move&& m) noexcept;
    ~Move(void) {}
    inline bool operator==(const Move& m) const noexcept { return step == m.step; }

    inline const std::vector<Step> GetSteps(void) const noexcept { return step; }
    bool IsTake(void) const noexcept;
    inline const Field& GetFr(void) const { return step.front().GetFr(); }
    inline const Field& GetTo(void) const { return step.back().GetTo(); }
    const std::vector<Location> GetJumped(void) const;  // return list of jumped-over locations
    constexpr inline void SetValue(PositionValue v) noexcept { value = v; }
    constexpr inline PositionValue GetValue(void) const noexcept { return value; }
    constexpr inline bool operator <(const Move& rhs) const noexcept { return value < rhs.value; }

  private:
    std::vector<Step> step;
    PositionValue value{};
  };


  class TileColor final
  {
  private:
    constexpr inline TileColor(const char& f) noexcept : tilecolor{ f } {}
  public:
    inline void Serialize(CArchive& ar) const { ar << tilecolor; }

  private:
    const char tilecolor;

  public:  // the only instances ever to exists; handed around by pointer
    static const TileColor Light;
    static const TileColor Dark ;
    static const TileColor Small;
  };


  class Color final
  {
  private:
    constexpr inline Color(const char& c) noexcept : color{ c } {}
  public:
    inline size_t GetHash(void) const noexcept { return std::hash<char>()(color); }
    inline const Color* operator !(void) const noexcept { return color == 'W' ? &Black : &White; }
    inline void Serialize(CArchive& ar) const { ar << color; }

  private:
    const char color;

  public:  // the only instances ever to exists; handed around by pointer
    static const Color NoColor;
    static const Color White;
    static const Color Black;
  };


  class MainPosition;  // forward declaration needed in class Kind
  class Kind
  {
  protected:
    constexpr inline Kind(const char& k) noexcept : kind{ k } {}
  public:
    inline bool operator==(const Kind& k) const noexcept { return k.kind == kind; }
    inline size_t GetHash(void) const noexcept { return std::hash<char>()(kind); }
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept { return 0; }
    virtual void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, std::vector<Move>& /*m*/) const {};
    virtual void Serialize(CArchive& ar) const { ar << kind; }
    virtual bool CanDrop(const MainPosition* /*pos*/, const Location& /*l*/) const noexcept { return false; }

  private:
    const char kind;

  public:
    static const Kind NoKind;
  };


  class PlayerType final
  {
  private:
    constexpr inline PlayerType(const char& p) noexcept : playertype{ p } {}
  public:
    inline void Serialize(CArchive& ar) const { ar << playertype; }

  private:
    const char playertype;

  public:  // the only instances ever to exists; handed around by pointer
    static const PlayerType Human;
    static const PlayerType Computer;
  };


  // cannot be constexpr as it loads bitmaps at runtime
  class Piece
  {
  protected:
    inline Piece(const Kind* k, const Color* c, UINT nID_l, UINT nID_d, UINT nID_s) noexcept
      : kind{ k }, color{ c }, ID_l{ nID_l }, ID_d{ nID_d }, ID_s{ nID_s } {}
  public:
    Piece(void) noexcept = delete;
    Piece(const Piece&) noexcept = delete;            // delete copy constructor
    Piece& operator=(const Piece&) = delete;          // delete assignment operator
    virtual ~Piece(void) noexcept {}
    inline size_t GetHash(void) const noexcept { return kind->GetHash() + color->GetHash(); }
    virtual void Serialize(CArchive& ar) const { color->Serialize(ar); kind->Serialize(ar); }
    inline void CollectMoves(const MainPosition& p, const Location l, std::vector<Move>& m) const { kind->CollectMoves(p, l, m); }
    virtual inline unsigned int GetValue(const MainPosition& p, const Location l) const noexcept { return kind->GetValue(p,l); }
    inline bool IsKind(const Kind& k) const noexcept { return k == *kind; }
    inline bool IsColor(const Color* c) const noexcept { return c == color; }
    inline const Color* GetColor(void) const noexcept { return color; }
    inline bool IsBlank(void) const noexcept { return color == &Color::NoColor && kind == &Kind::NoKind; }
    virtual inline bool IsPromotable(void) const noexcept { return false; }
    virtual inline const Piece* Promote(bool /*up*/) const noexcept { return this; };           // by default: no promotions
    virtual void Draw(CDC* pDC, const CRect& r, const TileColor* f) const;
    static const std::unordered_map<std::string, const Piece*>& GetHTMLPieceMap(void) noexcept;
    static std::vector<const Piece*> ListFromHTML(std::string s, const std::unordered_map<std::string, const Piece*>&);

  public:
    static const Piece NoTile;    // nothing exists there, don't draw the tile at all
    static const Piece NoPiece;   // no piece on the tile, but still draw it

  protected:
    const Kind* kind;      // points to a static shared object ('Singleton') - we don't own it
    const Color* color;    // points to a static shared object ('Singleton') - we don't own it

  private:
    UINT ID_l;             // bitmap ID for light tile
    UINT ID_d;             // bitmap ID for dark tile
    UINT ID_s;             // bitmap ID for small tile
    mutable CBitmap cb_l{};  // mutable to allow 'lazy' fill - also, Windows doesn't allow filling before main()
    mutable CBitmap cb_d{};
    mutable CBitmap cb_s{};
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
    virtual void Serialize(CArchive& ar) const { for (auto& p : pieces) pm->GetPiece(p)->Serialize(ar); }
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
    virtual std::vector<Move> CollectMoves(void) const { std::vector<Move> m{}; return m; }
    bool JumpsOnly(std::vector<Move>& moves) const noexcept;
    inline std::vector<Move>& GetMoveList(bool w) { return w ? movelistW : movelistB; }
    virtual bool AddIfLegal(std::vector<Move>&, const Location, const Location) const { return false; };
    virtual void EvaluateStatically(void);       // calculate position value and save
    virtual PositionValue Evaluate(AIContext& plist, bool w, PositionValue alpha, PositionValue beta, unsigned int plies);
    inline PositionValue GetValue(bool w) const noexcept { return value.Relative(w); }
    virtual inline unsigned int GetMoveCountFactor(void) const noexcept { return 20; }
    inline Depth GetDepth(void) const noexcept { return depth; }
    inline Depth SetDepth(Depth d) noexcept { return depth = d; }
    inline PositionValue SetValue(bool w, PositionValue v) noexcept { return value = v.Relative(w); }
    virtual inline const Move& GetBestMove(bool w) const { return (w ? movelistW[0] : movelistB[0]); }
    virtual MainPosition* GetPosition(AIContext& plist, Move* m = nullptr) const;     // execute move, maintain in PList
    virtual const std::vector<const Piece*> Execute(const Move& m);
    virtual void Undo(const Move& m);

  public:
    std::vector<Move> sequence{};

  protected:
    const Color* onTurn{ &Color::White };
    Depth depth{ 0 };
    PositionValue value{ PositionValue::Undefined };                 // position value for White
    std::vector<Move> movelistW{};
    std::vector<Move> movelistB{};
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
    virtual void Serialize(CArchive& ar) { pos->Serialize(ar); }
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
    virtual inline void SetUpdateCallBack(std::function<void(void)> cb) override { plist.callback = cb; }

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
    std::vector<Move> moves{};                   // will contain all allowed moves once a start piece is selected
    bool gameover{ false };                      // once game is over, moves are disallowed
  };

}
