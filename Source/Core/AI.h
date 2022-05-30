
namespace BoardGamesCore
{


  enum class AIMethod : unsigned char
  {
    Statically,
    BruteForce,
    AlphaBetaPruning,
  };

  class AI
  {

  public:
    AI() = default;
    ~AI() { for (Board* p : map_) delete p; }

    Board* Remember(Board* board) noexcept     // remember Board
    {
      auto pl0 = map_.find(board);  // check if we evaluated this position before
      if (pl0 != map_.end())
      {
        if (*pl0 == board) return board;  // did we find ourselves?
        assert(**pl0 == *board); // for now, do a full compare anyway, to verify all our hashing works as it should
        delete board;    // as a better (=already evaluated) version is in the map; delete this one
        return *pl0;  // return the version found in the map
      }

      board->GetAllMoves();
      board->EvaluateStatically();
      auto entry = map_.insert(board);
      return *(entry.first);  // return the pointer to the new entry
    }
    PositionValue MakeMove(Board*& b);
    PositionValue Evaluate(Board* board, AIMethod method, Depth maxdepth, double timeLimit);
    void SetCallback(std::function<void()> cb) { assert(cb != nullptr); callback_ = cb; }
    void Purge(const Moves& sequence_) noexcept;  // 'forget' all boards that don't start with this move sequence
    size_t GetSize() const noexcept { return map_.size(); }

  private:
    PositionValue EvaluateDeeper(Board* board, AIMethod method, bool whiteonplay, PositionValue alpha, PositionValue beta, unsigned int plies) noexcept;
    Board* MoveAndRemember(Board* board, const Move& m) noexcept     // execute move and remember result
    { 
      auto b = board->Clone();
      b->Execute(m);
      return Remember(b);
    }

  private:
    struct AIContextHelper
    {
      using pB = Board*;
      struct Hash { std::size_t operator()(const pB p) const noexcept { return p->GetHash(); } };
      struct Equality { bool operator()(const pB Lhs, const pB Rhs) const noexcept { return *Lhs == *Rhs; } };
    };
  private:
    std::unordered_set< Board*, AIContextHelper::Hash, AIContextHelper::Equality> map_{};
    std::function<void()> callback_{ nullptr };      // pointer to CDocument's callback function (to update window)

  };



  //class AIContext final
  //{
  //private:
  //  struct AIContextHelper
  //  {
  //    using pB = Board*;
  //    struct Hash { std::size_t operator()(const pB p) const noexcept { return p->GetHash(); } };
  //    struct Equality { bool operator()(const pB Lhs, const pB Rhs) const noexcept { return *Lhs == *Rhs; } };
  //  };
  //  using BoardSet = std::unordered_set< Board*, AIContextHelper::Hash, AIContextHelper::Equality>;

  //public:
  //  AIContext() = default;
  //  ~AIContext() { for (Board* p : map_) delete p; }

  //  void Insert(Board* b) noexcept 
  //  {
  //    auto pl0 = find(b);
  //    assert(pl0 == end());
  //  }

  //  BoardSet::const_iterator find(Board* p) const noexcept { return map_.find(p); }
  //  BoardSet::const_iterator begin() const noexcept { return map_.begin(); }
  //  BoardSet::const_iterator end() const noexcept { return map_.end(); }
  //  size_t erase(Board* p) noexcept { return map_.erase(p); }
  //  BoardSet::const_iterator erase(BoardSet::const_iterator it) noexcept { return map_.erase(it); }
  //  size_t size() const noexcept { return map_.size(); }
  //  Board* insert(Board* p) noexcept
  //  { 
  //    std::pair<std::unordered_set<Board*>::iterator, bool> res = map_.insert(p);
  //    assert(res.second);
  //    return *(res.first);  // return the pointer to the new entry
  //  }


  //  std::function<void()> callback;          // pointer to CDocument's callback function (to update window)
  //  size_t freemem{};
  //  void Purge(const Moves& sequence_) noexcept;
  //private:
  //  BoardSet map_{};
  //};

//  class Evaluator
//  {
//  public:
//    Evaluator(AIContext& plist, Board* b, AIMethod method) noexcept : plist_{ plist }, board_{ b }, method_{ method } {}
//
//  public:
//    constexpr void SetValue(bool whiteonplay, PositionValue v) const noexcept { board_->value_ = v.Relative(whiteonplay); }
//    PositionValue Evaluate(Depth depth, double timeLimit);
//    Move GetBestMove() { return *(board_->GetMoveList(*board_->onTurn_ == PieceColor::White)[0]); }
//
//  private:
//    PositionValue EvaluateDeeper(bool whiteonplay, PositionValue alpha, PositionValue beta, unsigned int plies) noexcept;
//
//    virtual TBoardP Execute(const Move& m) noexcept { auto b = board_->Clone(); b->Execute(m); return b; }    // execute move and return a new (temporary) Board* 
////    virtual void Undo(const Move& m) noexcept;
//
//
//  private:
//    AIContext& plist_;   // AI's 'memory' - persistant between moves
//    Board* board_;       // the board we are evaluating
//    AIMethod method_;    // evaluation method
//  };

}
