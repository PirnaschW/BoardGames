
namespace Logik
{

  // Game Dimensions
  using CountColors = unsigned char;   // colors to choose from
  using CountPegs = unsigned char;   // number of spaces to fill
  using CountTries = unsigned char;   // number of tries per player

  constexpr const inline static CountColors MaxColors{ 8U };
  constexpr const inline static CountPegs   MaxPegs{ 5U };
  constexpr const inline static CountTries  MaxTries{ 7U };

  constexpr unsigned int BoardSizeX = 2 * MaxPegs; // tiles on the board
  constexpr unsigned int BoardSizeY = MaxTries;    // tiles on the board

    // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 27;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 27;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  constexpr unsigned int BoardFrameX = 5;


  using namespace BoardGamesCore;


  template <char k>
  class Peg final : public Kind
  {
  private:
    constexpr inline Peg<k>(void) noexcept : Kind(k) {}

  public:
    constexpr virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 1; }

  public:
    static const Peg<k> ThePeg;
  };

  template <char k>
  const Peg<k> Peg<k>::ThePeg{};


  class LogikPiece final : public Piece
  {
  private:
    inline LogikPiece(const Kind& k, const Color& c, UINT l, UINT s) noexcept : Piece(k, c, l, l, s) {}
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
        default:assert(1 == 0); return LogikPiece::LPiece1;
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


  using PlayCode = unsigned int;                                          // code that contains a complete play (sequence of Pegs)
  using PlayCfg = std::array<unsigned char, MaxPegs>;                     // array that contains a complete play (sequence of Pegs)
  class Play final                                                        // holds a Play / 'move' = a set of pegs
  {
    friend class Plays;                                                   // Plays are only created from within class Plays
  private:
    constexpr inline Play(const PlayCode& c) noexcept : code_(c), peg_(Convert(c)) {}

  public:
    constexpr inline operator PlayCode() const noexcept { return code_; }
    constexpr inline unsigned char operator[](unsigned int z) const noexcept { return peg_[z]; }

  private:
    const PlayCode code_{};
    const PlayCfg peg_{};

  private:
    constexpr inline static PlayCode Convert(const PlayCfg& p) noexcept
    {
      PlayCode c{};
      for (unsigned int i = 0; i < MaxPegs; ++i)
      {
        c *= MaxColors;
        c += p[MaxPegs - 1 - i];
      }
      return c;
    }
    constexpr inline static PlayCfg Convert(const PlayCode& c) noexcept
    {
      PlayCfg p{};
      PlayCode z{ c };
      for (unsigned int i = 0; i < MaxPegs; ++i)
      {
        p[i] = z % MaxColors;
        z /= MaxColors;
      }
      return p;
    }
  };

  class Plays final
  {
  public:
    inline Plays(void) noexcept { for (PlayCode c = 0; c < Max; ++c) plays_[c] = new Play(c); }
    inline ~Plays(void) noexcept { for (PlayCode c = 0; c < Max; ++c) delete plays_[c]; }
    constexpr inline const Play& operator[](const PlayCode& c) const noexcept { return *plays_[c]; }
    constexpr inline const Play& operator[](const PlayCfg& p) const noexcept { return *plays_[Play::Convert(p)]; }

  public:
    constexpr const static PlayCode Max = Math::ipow(MaxColors, MaxPegs);

  private:
    std::array<Play*, Max> plays_{ nullptr };
  };


  using ResultCode = unsigned char;                                       // code that contains a Result
  using MarkerCount = unsigned char;                                      // marker count (for black and white markers)
  class Result final                                                      // holds a potential result (so many Black and so many White markers)
  {
  public:
    constexpr inline Result(void) noexcept {}
    constexpr inline Result(MarkerCount b, MarkerCount w) noexcept : code_(Convert(b, w)) {}       // get the result from marker counts
    Result(const Play& p1, const Play& p2) noexcept;                                               // get the result from comparing two plays_
    constexpr inline bool operator==(const Result& r) const noexcept { return r.code_ == this->code_; }
    constexpr inline bool operator!=(const Result& r) const noexcept { return !(*this == r); }
    constexpr inline operator unsigned int() const noexcept { return code_; }                      // used as index in collection array
    constexpr inline MarkerCount GetMarker(bool m) const noexcept                                  // number of black / white markers
    {
      ResultCode z{ code_ };
      if (z == Result::RMax() - 1) return m ? MaxPegs : 0;
      for (unsigned int i = 0; i < MaxPegs; ++i)
      {
        if (z <= MaxPegs - i) return m ? i : z;
        z -= MaxPegs - i + 1;
      }
      assert(false);
      return 0;
    }
  public:
    constexpr inline static ResultCode RMax(void) noexcept { return (MaxPegs + 1) * (MaxPegs + 2) / 2 - 1; }

  private:
    ResultCode code_{};

  private:
    constexpr inline static ResultCode Convert(MarkerCount b, MarkerCount w) noexcept
    {
      ResultCode c{ w };
      if (b == MaxPegs) c = Result::RMax() - 1;
      else for (unsigned int i = 0; i < b; ++i) c += MaxPegs - i + 1;
      return c;
    }
  };


  class LogikLayout final : public MainLayout
  {
  public:
    LogikLayout(const Dimensions& d) noexcept;
    virtual void Draw(CDC* pDC, const MainPosition* pos, _Mode mode) const override;

  private:
    inline const TileColor& FC(Coordinate i, Coordinate j) const noexcept;
  };


  class LogikPosition final : public MainPosition
  {
  public:
    inline LogikPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d) {}
    virtual inline MainPosition* Clone(void) const noexcept override { return new LogikPosition(*this); }

    bool SetFirstFreePeg(const Piece& p) noexcept;                        // returns if it could successfully set the Peg
    bool SetFirstFreeMarker(const Piece& p) noexcept;                     // returns if it could successfully set the Marker
    void ReadPosition(void) noexcept;
    PlayCode GetBestMove(unsigned int nThreads = std::thread::hardware_concurrency()) const;
    void Execute(const PlayCode& p);

  private:
    unsigned int EvaluatePlay(const PlayCode& c) const noexcept;
    void CollectBestPlays(const PlayCode& c, unsigned int lmax) const noexcept;

  private:
    unsigned int prevc_{};                                                // number of moves already made
    PlayCode previ_[MaxTries]{};                                          // indices of moves already made
    Result prevR_[MaxTries]{ {} };                                        // results of already made moves
    mutable std::mutex* mx{};                                             // mutex for multithreaded evaluation (locks gmin and bestI)
    mutable unsigned int gmin{ Plays::Max };                              // best found count
    mutable std::vector<PlayCode> bestI{};                                // moves that produce the best count found
                                                                          
    static inline const Plays plays_;                                     // all possible plays
  };


  class LogikGame final : public Game
  {
  private:
    LogikGame(void) = delete;
  public:
    inline LogikGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new LogikPosition(m, d), new LogikLayout(d)) {}

    virtual bool React(UINT nChar, UINT nRepCnt, UINT nFlags) override;  // react to keyboard input (not menu shortcuts, but typing)
    virtual bool AIMove(void) override;

    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
