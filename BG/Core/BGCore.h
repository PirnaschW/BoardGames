
namespace BoardGamesCore
{

  class MainPosition;
  class Piece;

  class Offset final
  {
    friend class Location;
  public:
    constexpr Offset(int xx, int yy) : dx{ xx }, dy{ yy } {}

    const Offset operator*(int i) const { return Offset(dx*i, dy*i); }

  private:
    const int dx;
    const int dy;

  public:
    // standard 4 'Rook'   directions
    inline const static std::array<const Offset, 4> Rdirection{ Offset(+1, +0), Offset(+0, +1),
                                                               Offset(+0, -1), Offset(-1, +0) };
    // standard 4 'Bishop' directions
    inline const static std::array<const Offset, 4> Bdirection{ Offset(+1, +1), Offset(+1, -1),
                                                               Offset(-1, -1), Offset(-1, +1) };
    // standard 8 'Queen'  directions
    inline const static std::array<const Offset, 8> Qdirection{ Offset(+1, +1), Offset(+1, +0), Offset(+1, -1),
                                                               Offset(+0, +1),                 Offset(+0, -1),
                                                               Offset(-1, +1), Offset(-1, +0), Offset(-1, -1) };
  };


  class Location final
  {
  public:
    Location(unsigned int xx, unsigned int yy) : x{ xx }, y{ yy } {}

    inline bool operator==(const Location& l) const { return l.x == x && l.y == y; }
    inline bool operator!=(const Location& l) const { return !(l == *this); }
    inline Location operator+(const Offset& o) const { Location l(*this); return l += o; }
    inline Location& operator+=(const Offset& o) { x += o.dx, y += o.dy; return *this; }

    inline bool Valid(unsigned int sizeX, unsigned int sizeY) const { return x >= 0 && x < sizeX && y >= 0 && y < sizeY; }
    inline unsigned int Index(unsigned int /*sizeX*/, unsigned int sizeY) const { return x * sizeY + y; }

    // can't be protected, as the values need to be used in many places
    // can't be const, or assignments between Locations wouldn't work.
  public:
    unsigned int x;
    unsigned int y;
  };


  class Field final // combines a Location and a Piece on it
  {
  public:
    Field(const Location l, const Piece* p) : l{ l }, p{ p } {}
    inline bool operator==(const Field& f) const { return l == f.l && p == f.p; }
    inline bool operator!=(const Field& f) const { return !(f == *this); }

    inline const Location GetLocation(void) const { return l; }
    inline const Piece* GetPiece(void) const { return p; }

  private:
    Location l;
    const Piece* p;
  };


  class Step final // define one hop in a complete move
  {
  public:
    enum StepType {
      Normal = 0x01,
      Take = 0x02,
      Jump = 0x04,
      Promote = 0x08,
    };
  public:
    Step(const Field& Fr, const Field& To, StepType Ty, const std::vector<Field>& Take) : from{ Fr }, to{ To }, type{ Ty }, take{ Take } {}
    inline bool operator == (const Step& s) const { return from == s.from && to == s.to && type == s.type && take == s.take; }
    inline bool operator != (const Step& s) const { return !(s == *this); }

    inline const Field& GetFr(void) const { return from; }
    inline const Field& GetTo(void) const { return to; }
    inline const StepType GetType(void) const { return type; }
    inline const std::vector<Field>& GetTake() const { return take; }
    inline bool IsTake(void) const { return type & StepType::Take; }

  private:
    Field from;
    Field to;
    StepType type;
    std::vector<Field> take;
  };


  class Move
  {
  public:
    typedef int PositionValue;
    constexpr const static PositionValue win{ INT_MAX / 8 };

  public:
    Move(const Field Fr, const Field To, Step::StepType Ty = Step::StepType::Normal, const std::vector<Field>& Take = {}) :
      step{ Step{Fr,To,Ty,Take} } {}
    Move(const std::vector<Step> St) : step{ St } {}
    Move(const Move& move) = default;
    Move& operator =(const Move& m) = default;
    Move&& operator =(Move&& m);
    virtual ~Move(void) {}
    virtual bool operator==(const Move& m) { return step == m.step && value == m.value; }

    virtual const std::vector<Step> GetSteps(void) const { return step; }
    virtual bool IsTake(void) const;
    virtual const Location GetFr(void) const { return step.front().GetFr().GetLocation(); }
    virtual const Location GetTo(void) const { return step.back().GetTo().GetLocation(); }
    virtual const std::vector<Location> GetJumped(void) const;  // return list of jumped-over locations
    virtual void SetValue(PositionValue v) { value = v; }
    virtual PositionValue GetValue(void) const { return value; }
    virtual bool operator <(const Move& rhs) const { return value > rhs.value; }

  private:
    std::vector<Step> step;
    PositionValue value{ 0 };
  };


  class TileColor final
  {
  private:
    TileColor(char f) : tilecolor{ f } {}
  public:
    inline void Serialize(CArchive& ar) const { ar << tilecolor; }

  private:
    const char tilecolor;

  public:  // the only instances ever to exists; handed around by pointer
    inline const static TileColor Light{ 'L' };
    inline const static TileColor Dark{ 'D' };
    inline const static TileColor Small{ 'l' };
  };


  class Color final
  {
  private:
    Color(char c) : color{ c } {}
  public:
    inline size_t GetHash(void) const { return std::hash<char>()(color); }
    inline const Color* operator !(void) const { return color == 'W' ? &Black : &White; }
    inline void Serialize(CArchive& ar) const { ar << color; }

  private:
    const char color;

  public:  // the only instances ever to exists; handed around by pointer
    inline const static Color NoColor{ 'X' };
    inline const static Color White{ 'W' };
    inline const static Color Black{ 'B' };
  };


  class Kind
  {
  protected:
    Kind(char k) : kind{ k } {}
  public:
    virtual bool operator==(const Kind& k) const { return k.kind == kind; }
    virtual size_t GetHash(void) const { return std::hash<char>()(kind); }
    virtual void CollectMoves(const MainPosition&, const Location&, std::vector<Move>&) const {};
    virtual unsigned int GetValue(void) const { return 0; }
    virtual void Serialize(CArchive& ar) const { ar << kind; }
  private:
    const char kind;

  public:
    inline const static Kind NoKind{ 'X' };
  };


  class PlayerType final
  {
  private:
    PlayerType(char p) : playertype{ p } {}
  public:
    inline void Serialize(CArchive& ar) const { ar << playertype; }

  private:
    const char playertype;

  public:  // the only instances ever to exists; handed around by pointer
    inline const static PlayerType Human{ 'H' };
    inline const static PlayerType Computer{ 'C' };
  };


  class Piece
  {
  protected:
    Piece(const Kind* k, const Color* c, UINT nID_l, UINT nID_d, UINT nID_s)
      : kind{ k }, color{ c }, ID_l{ nID_l }, ID_d{ nID_d }, ID_s{ nID_s } {}
  public:
    Piece(const Piece&) = delete;            // delete copy constructor
    Piece& operator=(const Piece&) = delete; // delete assignment operator
    virtual ~Piece(void) { }
    virtual size_t GetHash(void) const { return kind->GetHash() + color->GetHash(); }
    virtual void Serialize(CArchive& ar) const { color->Serialize(ar); kind->Serialize(ar); }
    virtual void CollectMoves(const MainPosition& p, const Location l, std::vector<Move>& m) const { kind->CollectMoves(p, l, m); }
    virtual unsigned int GetValue(void) const { return kind->GetValue(); }
    virtual bool IsKind(const Kind& k) const { return k == *kind; }
    virtual bool IsColor(const Color* c) const { return c == color; }
    virtual const Color* GetColor(void) const { return color; }
    virtual bool IsBlank(void) const { return color == &Color::NoColor && kind == &Kind::NoKind; }
    virtual bool IsPromotable(void) const { return false; }
    virtual const Piece* Promote(bool /*up*/) const { return this; };                         // by default, no promotions
    virtual void Draw(CDC* pDC, const CRect& r, const TileColor* f) const;

  public:
    inline const static Piece NoTile{ &Kind::NoKind, &Color::NoColor, 0, 0, 0 };               // nothing exists there, don't draw the tile at all
    inline const static Piece NoPiece{ &Kind::NoKind, &Color::NoColor, IDB_XXL, IDB_XXD, 0 };   // no piece on the tile, but still draw it

  protected:
    const Kind* kind;   // points to a static shared object ('Singleton') - we don't own it
    const Color* color; // points to a static shared object ('Singleton') - we don't own it

  private:
    UINT ID_l;   // bitmap ID for light tile
    UINT ID_d;   // bitmap ID for dark tile
    UINT ID_s;   // bitmap ID for small tile
    mutable CBitmap cb_l;  // mutable to allow 'lazy' fill - also, Windows doesn't allow filling before main()
    mutable CBitmap cb_d;
    mutable CBitmap cb_s;
  };


  class Position
  {
  public:
    Position(unsigned int x, unsigned int y, const Piece* init = &Piece::NoPiece) : sizeX(x), sizeY(y), pieces{ x*y,init } {}
    virtual ~Position() {}
    virtual bool operator ==(const Position* p) const { return pieces == p->pieces; }
    virtual std::size_t GetHash(void) const;
    virtual void Serialize(CArchive& ar) const { for (auto& p : pieces) p->Serialize(ar); }
    virtual const Piece* GetPiece(const Location& l) const { return l.Valid(sizeX, sizeY) ? pieces[l.Index(sizeX, sizeY)] : nullptr; }
    virtual const Piece* SetPiece(const Location& l, const Piece* p) { hash = 0; return (pieces)[l.Index(sizeX, sizeY)] = p; }

  protected:
    const unsigned int sizeX;
    const unsigned int sizeY;
  private:
    std::vector<const Piece*> pieces;
    mutable std::size_t hash{};
  };


  class MainPosition : public Position
  {
  public:
    //typedef std::shared_ptr<const MainPosition> pMP;
    typedef MainPosition* pMP;

    struct Hash { std::size_t operator()(const pMP p) const { return p->GetHash(); } };
    struct Equality { bool operator()(const pMP Lhs, const pMP Rhs) const { return *Lhs == *Rhs; } };
    typedef std::unordered_set< pMP, Hash, Equality> PList;

  public:
    MainPosition(unsigned int x, unsigned int y) : Position(x, y) {}
    virtual ~MainPosition() {}
    virtual MainPosition* Clone(void) const = 0;
    virtual bool operator ==(const MainPosition& p) const { return Position::operator==(&p); }

    virtual void SetOnTurn(const Color* c) { onTurn = c; }
    virtual const Color* OnTurn(void) const { return onTurn; }
    virtual void NextPlayer(void);
    virtual void PreviousPlayer(void);  // needed for Undo

    virtual void GetAllMoves(void);  // generate all moves and save list
    virtual std::vector<Move> CollectMoves(void) const { std::vector<Move> m{}; return m; }
    virtual bool AddIfLegal(std::vector<Move> &, const Location, const Location) const { return false; };
    virtual Move::PositionValue EvaluateStatically(void);
    virtual Move::PositionValue Evaluate(MainPosition::PList& plist, const Color* on, int alpha, int beta, unsigned int plies);
    virtual MainPosition* GetPosition(MainPosition::PList& plist, Move* m = nullptr);               // execute move, maintain in PList
    virtual Move::PositionValue Rate(Move::PositionValue z, unsigned int v) const { return z + (Move::PositionValue)(v / 2); }
    virtual const std::vector<const Piece*> Execute(const Move& m);
    virtual void Undo(const Move& m);

  public:
    std::vector<Move> sequence{};
    mutable std::vector<Move> movelistW{};
    mutable std::vector<Move> movelistB{};
    mutable Move::PositionValue value{};

  protected:
    const Color* onTurn{ &Color::White };
  };

  class TakenPosition : public Position
  {
  public:
    TakenPosition(unsigned int x, unsigned int y) : Position(x, y, &Piece::NoTile) {}
    virtual ~TakenPosition() {}
    void Push(unsigned int player, const std::vector<const Piece*>& p);
  };

  class StockPosition : public Position
  {
  public:
    StockPosition(unsigned int x, unsigned int y) : Position(x, y) {}
    virtual ~StockPosition() {}
  };


  class Player final
  {
  public:
    Player(const PlayerType* p = &PlayerType::Human, const Color* c = &Color::NoColor) : playertype{ p }, color{ c } {}    // constructor
    Player(const Player& p) : playertype{ p.playertype }, color{ p.color } {}                                              // copy constructor
    Player& operator=(const Player& p) { return *this = Player(p.playertype, p.color); }                               // copy assignment
    Player(Player&& p) : playertype{ std::move(p.playertype) }, color{ std::move(p.color) } {}                             // move constructor
    Player& operator=(Player&& p) { std::swap(playertype, p.playertype); std::swap(color, p.color); return *this; }    // move assignment

    inline const Color* SetColor(const Color* c) { return color = c; }
    inline const Color* GetColor(void) const { return color; }
    inline const PlayerType* SetPlayerType(const PlayerType* p) { return playertype = p; }
    inline const PlayerType* GetPlayerType(void) const { return playertype; }
    inline bool Is(const PlayerType* p) const { return playertype == p; }

  private:
    const std::string name{};
    const PlayerType* playertype;
    const Color* color;
  };


  class Layout;
  class TakenLayout;
  class StockLayout;


  class Game
  {
  private:
    Game(void) = delete;
  public:
    Game(MainPosition* p, TakenPosition* t, StockPosition* s, Layout* l, TakenLayout* tl, StockLayout* sl);
    virtual ~Game(void);
    virtual void Serialize(CArchive& ar) { pos->Serialize(ar); }
    virtual void AddToStock(const Location& l, const Piece* p) { spos->SetPiece(l, p); }
    virtual void ShowStock(bool show) { showStock = show; }
    virtual void Draw(CDC* pDC) const;
    virtual bool React(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) { return false; };   // react to keyboard input (not menu shortcuts, but typing)
    virtual bool React(UINT command);                                                          // react to button/menu command
    virtual bool React(UINT event, UINT nFlags, const CPoint& p);                              // react to mouse events
    virtual void React(CCmdUI* pCmdUI);                                                        // react to UI events (allows to set buttons greyed, etc.)
    virtual void DragTo(const CPoint& point) { dragPoint = point; }
    virtual void DragStart(const CPoint&);
    virtual void DragEnd(const CPoint&);
    virtual void Select(const CPoint & point);
    virtual void Unselect(void) { moves.clear(); }
    virtual void AIAction(void) { while (IsAlive() && CurrentPlayer()->Is(&PlayerType::Computer)) SetAlive(AIMove()); }  // execute computer moves while it is its turn
    virtual bool IsAlive(void) const { return !gameover; }
    virtual void SetAlive(bool a) { gameover = !a; }
    virtual void AddPlayer(Player* p) { players.push_back(p); }
    virtual void SetCurrentPlayer(unsigned int p) { current = p; }
    virtual Player* CurrentPlayer(void) const { return players[current]; }
    virtual Player* NextPlayer(void) { current = ++current % players.size(); return players[current]; }
    virtual unsigned int Plies(unsigned int /*z*/) const { return plies; }
    virtual bool AIMove(void);
    virtual void Execute(const Move& m);
    //    virtual std::vector<Move> GetAllMoves(void) const { return pos->GetAllMoves(); }

  protected:
    MainPosition * pos;               // logical position on the main playing board
    TakenPosition* tpos;              // taken pieces
    StockPosition* spos;              // piece list for editing

    Layout* lay;                      // physical layout of the main playing board
    TakenLayout* tlay;                // physical layout of the taken pieces
    StockLayout* slay;                // physical layout of the piece list

    MainPosition::PList plist{};      // collected positions with their evaluations
    bool showStock{ false };          // flag to show/hide the stock
    bool moveTaken{ false };          // flag to allow moves from taken pieces

  private:
    unsigned int plies{ 4 };          // standard number of plies
    bool editing{ false };            // edit mode allows to change the main playing board
    std::vector<Player*> players{};   // list of players
    unsigned int current{ 0 };        // current player
    std::vector<Move> moves{};        // will contain all allowed moves once a start piece is selected
    bool gameover{ false };           // once game is over, moves are disallowed

    bool dragging{ false };           // currently dragging a piece
    const Piece* dragPiece{};         // currently dragged piece
    CPoint dragPoint{};               // point the piece is dragged to
  };

}
