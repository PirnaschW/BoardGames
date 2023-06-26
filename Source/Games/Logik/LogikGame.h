
#ifdef MSVC_ERROR
class A { friend class B; constexpr A() {} };
class B { public: constexpr B() { A a; } };
B b;
#endif // MSVC_ERROR



#include <mutex>
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

  enum LogikVariant : VariantCode   // recognized variants of Logik
  {
    Logik8 = 0x08,  // 8 colors
    Logik6 = 0x06,  // 6 colors
    Logik4 = 0x04,  // 4 colors
  };

  template <char k>
  class Peg final : public Kind
  {
  private:
    constexpr inline Peg<k>() noexcept : Kind(k) {}

  public:
    virtual PositionValue GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 1; }

  public:
    static const Peg<k> ThePeg;
  };

  template <char k>
  const Peg<k> Peg<k>::ThePeg{};


  class LogikPiece final : public Piece
  {
  private:
    inline LogikPiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    LogikPiece(const LogikPiece&) = delete;
    LogikPiece& operator=(const LogikPiece&) = delete;

  public:
    static const PieceIndex GetPieceIndex(unsigned int z) noexcept
    {
      switch (z)
      {
        case 0: return PMap[LogikPiece::LPiece1];
        case 1: return PMap[LogikPiece::LPiece2];
        case 2: return PMap[LogikPiece::LPiece3];
        case 3: return PMap[LogikPiece::LPiece4];
        case 4: return PMap[LogikPiece::LPiece5];
        case 5: return PMap[LogikPiece::LPiece6];
        case 6: return PMap[LogikPiece::LPiece7];
        case 7: return PMap[LogikPiece::LPiece8];
        default:assert(1 == 0); return PMap[LogikPiece::LPiece1];
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
    /*constexpr*/ inline Plays() noexcept { for (PlayCode c = 0; c < Max; ++c) plays_[c] = new Play(c); }
    inline ~Plays() noexcept { for (PlayCode c = 0; c < Max; ++c) delete plays_[c]; }
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
    constexpr inline Result() noexcept {}
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
    constexpr inline static ResultCode RMax() noexcept { return (MaxPegs + 1) * (MaxPegs + 2) / 2 - 1; }

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


  class LogikBoard final : public Board
  {
  public:
    enum class LogikBoardPartID : unsigned char {
      ResultL = 0x04,  // left result area
      ResultR = 0x05,  // rightresult area
    };

  public:
    LogikBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept;
    virtual Board* Clone() const noexcept override { return new LogikBoard(*this); }
    virtual void SetStartingBoard() noexcept override {};

    bool SetFirstFreePeg(PieceIndex pI) noexcept;                        // returns true if it could successfully set the Peg
    bool SetFirstFreeMarker(PieceIndex pI) noexcept;                     // returns true if it could successfully set the Marker
    void ReadBoard() noexcept;
    PlayCode GetBestMove(unsigned int nThreads = std::thread::hardware_concurrency()) const;
    void Execute(const PlayCode& p);

    void Draw(DC* dc, bool showstock) const override;                    // draw the complete board

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
                                                                          
  private:
    BoardPart resultL_;                                                   // left result display
    BoardPart resultR_;                                                   // right result display

    static inline const Plays plays_;                                     // all possible plays
  };


  class LogikGame final : public Game
  {
  private:
    LogikGame() = delete;
  public:
    LogikGame(const VariantChosen& v, const std::vector<PieceIndex>& list, const BoardPartDimensions& d) noexcept : Game(v, list, new LogikBoard(v, d)) {}
    static void Register() noexcept;

    virtual bool React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) override;  // react to keyboard input (not menu shortcuts, but typing)
    virtual bool React(unsigned int command) override;                                           // react to button/menu command
    virtual bool AIMove();

    static const VariantList& GetVariants() noexcept;
    //static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
