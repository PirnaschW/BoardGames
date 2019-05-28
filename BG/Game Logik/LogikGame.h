
namespace Logik
{

  // Game Dimensions
  constexpr unsigned int PegColors = 8; //8;    // colors to choose from
  constexpr unsigned int PegCount = 5;  //5;    // number of spaces to fill
  constexpr unsigned int MaxTries = 7;  //7;    // number of tries per player

  constexpr unsigned int BoardSizeX = 2 * PegCount;  // tiles on the board
  constexpr unsigned int BoardSizeY = MaxTries;      // tiles on the board

    // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  constexpr unsigned int BoardFrameX = 5;

  using namespace BoardGamesCore;

  template <char k> class Peg : public Kind
  {
  private:
    constexpr Peg<k>(void) noexcept : Kind(k) {}

  public:
    void CollectMoves(const MainPosition&, const Location, Moves&) {};
    unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 1; }

  public:
    static const Peg<k> ThePeg;
  };
  
  template <char k> inline const Peg<k> Peg<k>::ThePeg{};


  class LogikPiece : public Piece
  {
  private:
    LogikPiece(const Kind* k, const Color* c, UINT l, UINT s) noexcept : Piece(k, c, l, l, s) {}
    LogikPiece(const LogikPiece&) = delete;
    LogikPiece& operator=(const LogikPiece&) = delete;

  public:
    static const LogikPiece& GetPiece(unsigned int z) noexcept
    {
      switch (z)
      {
        case 0: return LogikPiece::LPiece1;
        case 1: return LogikPiece::LPiece2;
        case 2: return LogikPiece::LPiece3;
        case 3: return LogikPiece::LPiece4;
        case 4: return LogikPiece::LPiece5;
        case 5: return LogikPiece::LPiece6;
        case 6: return LogikPiece::LPiece7;
        case 7: return LogikPiece::LPiece8;
        default:return LogikPiece::LPiece1;
      }
    }

    static const LogikPiece LPieceB;
    static const LogikPiece LPieceW;
    static const LogikPiece LPiece1;
    static const LogikPiece LPiece2;
    static const LogikPiece LPiece3;
    static const LogikPiece LPiece4;
    static const LogikPiece LPiece5;
    static const LogikPiece LPiece6;
    static const LogikPiece LPiece7;
    static const LogikPiece LPiece8;
  };


  template<unsigned int BX, unsigned int BY, unsigned int BZ>   // PegColors, PegCount, MaxTries
  class Play                                                    // holds a set of pegs = one potential 'move' or play
  {
  public:
    Play(unsigned int z) noexcept : code(z) { for (unsigned int i = 0; i < BY; ++i) { peg[i] = z % BX; z /= BX; } };
    Play(const std::array<unsigned int, BY>& p) noexcept : peg(p) { for (unsigned int i = 0; i < BY; ++i) { code *= BX; code += p[BY - 1 - i]; } }
    bool operator==(const Play<BX, BY, BZ>& p) const noexcept { return p.code == code; }
    bool operator!=(const Play<BX, BY, BZ>& p) const noexcept { return !(*this == p); }
    operator unsigned int() const noexcept { return code; }
    unsigned int operator[](unsigned int z) const noexcept { return peg[z]; }
  private:
    unsigned int code{};
    std::array<unsigned int, BY> peg{};

  public:
    static void test(void)
    {
      for (unsigned int i = 0; i < ipow(BX, BY); i++)
      {
        Play p = Play(i);
        Play q = Play(p.peg);
        assert(i == q);
      }
    }
  };


  template<unsigned int BX, unsigned int BY, unsigned int BZ>   // PegColors, PegCount, MaxTries
  class Result                                                  // holds a potential result (so many Black and so many White markers)
  {
  public:
    Result(void) noexcept {}
    Result(unsigned int b, unsigned int w) noexcept             // get the result from marker counts
    {
      if (b == BY) code = Result<BX, BY, BZ>::RN() - 1;
      else { code = w; for (unsigned int i = 0; i < b; i++) code += BY - i + 1; }
    }
    Result(const Play<BX, BY, BZ>& p1, const Play<BX, BY, BZ>& p2) noexcept  // get the result from comparing two plays
    {
      bool u1[BY]{};
      bool u2[BY]{};
      unsigned int b{};
      unsigned int w{};
      for (unsigned int i = 0; i < BY; ++i)
      {
        if (p1[i] == p2[i])
        {
          b++;
          u1[i] = u2[i] = true;
        }
      }
      if (b == BY)
      {
        code = Result<BX, BY, BZ>::RN() - 1;
        return;
      }

      for (unsigned int i = 0; i < BY; ++i)
      {
        if (!u1[i])
        {
          for (unsigned int j = 0; j < BY; ++j)
          {
            if ((i != j) && (!u2[j]))
            {
              if (p1[i] == p2[j])
              {
                w++;
                u1[i] = u2[j] = true;
                break;
              }
            }
          }
        }
      }
      code = w;
      for (unsigned int i = 0; i < b; i++) code += BY - i + 1;
    }
    constexpr static unsigned int RN(void) noexcept { return (BY + 1) * (BY + 2) / 2 - 1; }
    bool operator==(const Result<BX, BY, BZ>& r) const noexcept { return r.code == this->code; }
    bool operator!=(const Result<BX, BY, BZ>& r) const noexcept { return !(*this == r); }
    operator unsigned int() const noexcept { return code; }
    unsigned int GetMarker(unsigned int m) const noexcept
    {
      unsigned int z{code};
      if (z == Result<BX, BY, BZ>::RN() - 1) return m ? 0 : BY;
      for (unsigned int i = 0; i < BY; i++)
      {
        if (z <= BY - i) return m ? z : i;
        z -= BY - i + 1;
      }
    }

  private:
    unsigned int code{};
  };


  template<unsigned int BX, unsigned int BY, unsigned int BZ>  // PegColors, PegCount, MaxTries
  class LMove : public SimpleMove
  {
  public:
    LMove(PositionValue mm) noexcept : SimpleMove(std::make_shared<SimpleStep>( Field{Location(0U, 0U),nullptr}, Field{Location(0U, 0U),nullptr} )), m((unsigned int)mm) {}
    unsigned int GetIndex(void) const  noexcept { return m; }
    ~LMove(void) {}

  private:
    unsigned int m{};
  };


  template <unsigned int BX, unsigned int BY, unsigned int BZ>  // PegColors, PegCount, MaxTries
  class LPosition : public MainPosition
  {
  public:
    LPosition<BX, BY, BZ>(void) noexcept : MainPosition(nullptr, 4 * BY, BZ)
    {
      for (unsigned int i = 0; i < 4 * BY; i++)
        for (unsigned int j = 0; j < BZ; j++)
        {
          SetPiece(Location(i, j), &Piece::NoPiece);
        }
    }
    ~LPosition<BX, BY, BZ>() {}
    virtual LPosition<BX, BY, BZ>* Clone(void) const override { return new LPosition<BX, BY, BZ>(*this); }

    virtual bool SetFirstFreePeg(const Piece* p) noexcept
    {
      for (unsigned int j = 0; j < BZ; j++)
        for (unsigned int i = BY; i < 2 * BY; i++)
          if (GetPiece(Location(i, j)) == &Piece::NoPiece)
          {
            SetPiece(Location(i, j), p);
            return true;
          }
      return false;
    }
    virtual bool SetFirstFreeMarker(const Piece* p) noexcept
    {
      for (unsigned int j = 0; j < BZ; j++)
        for (unsigned int i = BY; i < 2 * BY; i++)
          if (GetPiece(Location(i, j)) == &Piece::NoPiece)
          {
            if (i == BY && j > 0) j--; // use last full line, except if nothing is set at all
            for (unsigned int z = 0; z < BY; z++)
            {
              if (GetPiece(Location(z, j)) == &Piece::NoPiece)
              {
                SetPiece(Location(z, j), p);
                return true;
              }
            }
          }
      return false;
    }
    virtual void ReadPosition(void) noexcept;
    virtual PositionValue Evaluate(unsigned int) const;
    virtual const std::vector<const Piece*> Execute(MoveP m) override
    {
      std::vector<const Piece*> taken{};
      previ[prevc] = static_cast<LMove<BX, BY, BZ>*>(m.get())->GetIndex();
      const Play<BX, BY, BZ>& p = previ[prevc];
      for (unsigned int i = 0; i < BY; ++i)
      {
        SetPiece(Location(BY + i, prevc), &LogikPiece::GetPiece(p[i]));
      }
      prevc++;

      return taken;
    }

  private:
    unsigned int prevc{};                 // number of moves already made
    unsigned int previ[BZ]{};             // indices of moves already made
    Result<BX, BY, BZ> prevR[BZ]{ {} };       // results of already made moves
  };


  template<unsigned int BX, unsigned int BY, unsigned int BZ>  // PegColors, PegCount, MaxTries
  void LPosition<BX, BY, BZ>::ReadPosition(void) noexcept
  {
    prevc = 0; // number of moves already made
    for (unsigned int k = 0; k < BZ; ++k)
    {
      std::array<unsigned int, BY> peg{};
      for (unsigned int i = 0; i < BY; ++i)
      {
        const Piece* p = GetPiece(Location(BY + i, k));
        if      (p->IsKind(Peg<'1'>::ThePeg)) peg[i] = 0;
        else if (p->IsKind(Peg<'2'>::ThePeg)) peg[i] = 1;
        else if (p->IsKind(Peg<'3'>::ThePeg)) peg[i] = 2;
        else if (p->IsKind(Peg<'4'>::ThePeg)) peg[i] = 3;
        else if (p->IsKind(Peg<'5'>::ThePeg)) peg[i] = 4;
        else if (p->IsKind(Peg<'6'>::ThePeg)) peg[i] = 5;
        else if (p->IsKind(Peg<'7'>::ThePeg)) peg[i] = 6;
        else if (p->IsKind(Peg<'8'>::ThePeg)) peg[i] = 7;
        else return;   // ignore incomplete lines and stop reading
      }

      // read Markers
      unsigned int b{};
      unsigned int w{};
      for (unsigned int i = 0; i < BY; ++i)
      {
        const Piece* p = GetPiece(Location(i, k));
        if (p->IsKind(Peg<'B'>::ThePeg)) b++;
        if (p->IsKind(Peg<'W'>::ThePeg)) w++;
      }
      prevR[k] = Result<BX, BY, BZ>(b, w);
      previ[k] = Play<BX, BY, BZ>(peg);
      prevc++;                                    // log this line as valid play
    }
  }


  constexpr unsigned int ipow(unsigned int base, unsigned int exp, unsigned int result = 1)
  {
    return exp < 1 ? result : ipow(base*base, exp / 2, (exp % 2) ? result * base : result);
  }


  template<unsigned int BX, unsigned int BY, unsigned int BZ>  // PegColors, PegCount, MaxTries
  PositionValue LPosition<BX, BY, BZ>::Evaluate(unsigned int) const // Evaluate best next move
  {
    // prepare array of previous plays
    std::vector<Play<BX, BY, BZ>> pp0;
    for (unsigned int i = 0; i < prevc; ++i)
    {
      pp0.push_back(Play<BX, BY, BZ>{ previ[i] });
    }

    constexpr unsigned long perms = ipow(BX, BY);

    // array to collect worst possible result count for each potential play
    std::unique_ptr<std::array<unsigned int, perms>> max{new std::array<unsigned int, perms>{}};

    for (unsigned int i = 0; i < perms; ++i)                           // for all possible plays we could try
    {
      const Play<BX, BY, BZ> p1{i};                                  // create the play
      [&]
      {
        for (unsigned int k = 0; k < prevc; ++k)                       // compare play to all previous plays
        {
          if (p1 == pp0[k]) return;                                    // if we tried this before, don't try it again
          if (prevR[k] != Result<BX, BY, BZ>{p1, pp0[k]}) return;      // if the result doesn't match, that can't be the opponent's solutions, so don't even try it
        }
        unsigned int RCCount[Result<BX, BY, BZ>::RN()]{};                // counter for how often each of the results happened
        for (unsigned int j = 0; j < perms; ++j)                       // for all possible opponent's solutions
        {
          bool match{true};
          const Play<BX, BY, BZ> p2{j};                              // create assumed opponent play
          for (unsigned int k = 0; match && (k < prevc); ++k)          // compare play to all previous plays
          {
            match = (prevR[k] == Result<BX, BY, BZ>{p2, pp0[k]});      // if the result doesn't match, that can't be the opponent's solutions
          }

          if (match)
          {
            const Result<BX, BY, BZ> r(p1, p2);                        // create result for the current play
            RCCount[r]++;                                              // increment counter for this result
          }
        }

        unsigned int lmax = 0;                                         // worst count found
        for (unsigned int k = 0; k < Result<BX, BY, BZ>::RN(); ++k)
        {
          if (RCCount[k] > lmax)
          {
            lmax = RCCount[k];
          }
        }
        (*max)[i] = lmax;                                                 // save worst (highest) count
      }();
    }

    unsigned int gmin{perms};
    std::vector<long> bestI{};
    for (unsigned int i = 0; i < perms; ++i)                              // for all possible plays, find the best 'worst count'
    {
      if ((*max)[i] == 0);                                                // ignore useless moves
      else if ((*max)[i] < gmin)                                          // found a better move
      {
        gmin = (*max)[i];                                                 // save best 'worst count' and its index
        bestI.clear();
        bestI.push_back(i);
      }
      else if ((*max)[i] == gmin)                                         // found an equally good move, collect it
      {
        bestI.push_back(i);
      }
    }

    return bestI[rand() % bestI.size()];                                  // randomly pick one of the equally good tries
  }


  class LMarkerStockPosition : public StockPosition
  {
  public:
    LMarkerStockPosition(void) noexcept : StockPosition(nullptr, 2 + 1, 1)
    {
      SetPiece(Location(0U, 0U), &LogikPiece::LPieceB);
      SetPiece(Location(1U, 0U), &LogikPiece::LPieceW);
    }
  };


  class LLayout : public MainLayout
  {
  public:
    LLayout(Coordinate x, Coordinate y) noexcept;

  private:
    inline const TileColor* FC(unsigned int i, unsigned int j) noexcept
    {
      if ((i < dim.xCount / 4) || (i >= 3 * dim.xCount / 4)) return &TileColor::Light;
      if ((j < dim.yCount - 1) || (i < 2 * dim.xCount / 4)) return &TileColor::Dark;
      return &TileColor::Light;
    }
  };


  class LStockLayout : public StockLayout
  {
  public:
    LStockLayout(Coordinate x, Coordinate y, unsigned int z) noexcept;
  };


  template<unsigned int BX, unsigned int BY, unsigned int BZ>  // PegColors, PegCount, MaxTries
  class LGame : public Game
  {
  private:
    LGame<BX, BY, BZ>(LPosition<BX, BY, BZ>* p, TakenPosition* t, StockPosition* s,
      LLayout* l, TakenLayout* tl, LStockLayout* sl) noexcept : Game{nullptr,p,t,s,l,tl,sl}
    {
      AddToStock(Location(0U, 0U), &LogikPiece::LPieceB);
      AddToStock(Location(1U, 0U), &LogikPiece::LPieceW);
      if (BX > 0) AddToStock(Location(2U, 0U), &LogikPiece::LPiece1);
      if (BX > 1) AddToStock(Location(3U, 0U), &LogikPiece::LPiece2);
      if (BX > 2) AddToStock(Location(4U, 0U), &LogikPiece::LPiece3);
      if (BX > 3) AddToStock(Location(5U, 0U), &LogikPiece::LPiece4);
      if (BX > 4) AddToStock(Location(6U, 0U), &LogikPiece::LPiece5);
      if (BX > 5) AddToStock(Location(7U, 0U), &LogikPiece::LPiece6);
      if (BX > 6) AddToStock(Location(8U, 0U), &LogikPiece::LPiece7);
      if (BX > 7) AddToStock(Location(9U, 0U), &LogikPiece::LPiece8);
      ShowStock(true);
    }
  public:
    LGame<BX, BY, BZ>(void) noexcept : LGame<BX, BY, BZ>(new LPosition<BX, BY, BZ>(), nullptr, new StockPosition(nullptr, BX + 3, 1),
      new LLayout(4 * BY, BZ), nullptr, new LStockLayout(BX, BY, BZ)) {}
    inline static const VariantList& GetVariants(void) noexcept { static VariantList v{ { Variant{ 8, 5 } } }; return v; }
    virtual bool React(UINT nChar, UINT nRepCnt, UINT nFlags) override;  // react to keyboard input (not menu shortcuts, but typing)
    virtual bool AIMove(void) override;
    void Execute(MoveP m) override
    {
      pos->Execute(m);
      pos->SetOnTurn(NextPlayer()->GetColor());
    }
    virtual void EditMode(void) {}
    virtual void SwitchColors(void) {}
    virtual void EditPlayers(void) {}
    virtual void Draw(CDC* pDC) const override;
  };


  template<unsigned int BX, unsigned int BY, unsigned int BZ>  // PegColors, PegCount, MaxTries
  bool LGame<BX, BY, BZ>::React(UINT nChar, UINT, UINT)  // react to keyboard input (not menu shortcuts, but typing)
  {
    SetCurrentPlayer(1);
    switch (nChar)
    {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        if (nChar > '0' + BX) return false;
        {
          const unsigned int c = nChar - '1';
          return dynamic_cast<LPosition<BX, BY, BZ>*>(pos)->SetFirstFreePeg(&LogikPiece::GetPiece(c));
        }
      case 'B':
      case 'b':
        return dynamic_cast<LPosition<BX, BY, BZ>*>(pos)->SetFirstFreeMarker(&LogikPiece::LPieceB);
      case 'W':
      case 'w':
        return dynamic_cast<LPosition<BX, BY, BZ>*>(pos)->SetFirstFreeMarker(&LogikPiece::LPieceW);
      case '\r':
        AIMove();
        return true;
    }
    return false;
  }


  template<unsigned int BX, unsigned int BY, unsigned int BZ>  // PegColors, PegCount, MaxTries
  bool LGame<BX, BY, BZ>::AIMove(void)
  {
    // read all user input from screen (this allows to set up or modify existing moves and responses)
    dynamic_cast<LPosition<BX, BY, BZ>*>(pos)->ReadPosition();

    if (1 == 0) // to be done
    {
      ::AfxMessageBox(L"Player won!");
      return false;
    }

    const PositionValue e = pos->Evaluate(plist, pos->OnTurn(), PositionValue::PValueType::Lost, PositionValue::PValueType::Won, 1);

    if (1 == 0)
    {
      ::AfxMessageBox(L"Computer resigns - Player wins!");
    }
    if (1 == 0)
    {
      ::AfxMessageBox(L"Computer won!");
      return false;
    }
    if (1 == 0) {
      ::AfxMessageBox(L"You might as well resign - Computer will win!");
    }

    Execute(std::make_shared<LMove<BX, BY, BZ>>(e));
    return true;
  }

  template<unsigned int BX, unsigned int BY, unsigned int BZ>  // PegColors, PegCount, MaxTries
  void LGame<BX, BY, BZ>::Draw(CDC* pDC) const
  {
    // frame around the board (needs to be drawn first)
    for (unsigned int z = BoardFrameX; z > 0; z--)
    {
      if (z != 2)
        pDC->Rectangle(
          static_cast<int>(BoardStartX + (3 + 1 + 1 * BY)*FieldSizeX + 0 + BoardFrameX - z),
          static_cast<int>(BoardStartY - z),
          static_cast<int>(BoardStartX + (3 + 1 + 3 * BY)*FieldSizeX + 1 + BoardFrameX + z),
          static_cast<int>(BoardStartY + BZ * FieldSizeY + z));
    }
    Game::Draw(pDC);       // let the (generic) parent draw the board itself
  };

}