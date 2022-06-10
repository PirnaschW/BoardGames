
namespace BoardGamesCore
{


  enum class AIMethod : unsigned char
  {
    Statically,
    BruteForce,
    AlphaBetaPruning,
  };

  template <class T>
  class PositionMemory
  {
  public:
    ~PositionMemory() { for (auto& b : objList_) delete b.first; }

  public:
    size_t GetSize() const noexcept { return objList_.size(); }
    T* FindPos(T* t)
    {
      auto pHash{ std::hash<const T*>()(t) };
      auto pos = posList_.find(pHash);
      if (pos == posList_.end()) return nullptr;

      assert(pos->second != t);
      assert(*pos->second == *t); // for now, do a full compare anyway, to verify all our hashing works as it should
      delete t;                   // as a better (=already evaluated) version is in the map; delete this one
      return pos->second;         // return the version found in the map
    }
    void AddBoard(T* t)
    {
      auto pHash{ std::hash<const T*>()(t) };
      auto entry1 = objList_.insert({ t,pHash });
      auto entry2 = posList_.insert({ pHash,t });
      assert(entry1.second);
      assert(entry2.second);
    }
    void RemoveBoard(T* t)        // remove if found, ignore if not found
    {
      auto entry = objList_.find(t);
      if (entry != objList_.end())
      {
        posList_.erase(entry->second);
        objList_.erase(entry);
      }
    }
    void Purge(const auto& purge)
    {
      for (auto it = objList_.begin(); it != objList_.end();)
      {
        auto p = it->first;
        if (purge(p))
        {
          RemoveBoard(p);
          delete p;
        }
        else it++;
      }
    }

  private:
    std::unordered_map<T*, size_t> objList_{};
    std::unordered_map<size_t, T*> posList_{};
  };

  class AI
  {

  public:
    AI() = default;
    ~AI() = default;

    Board* Remember(Board* board) noexcept     // remember given Board
    {
      // if board exists, remove it - ours brings a newer position and hash
      memory_.RemoveBoard(board);

      // check if position exists
      Board* pos = memory_.FindPos(board);
      if (pos) return pos;  // return the buffered position found in the map

      // it's a new position
      board->GetAllMoves();
      board->EvaluateStatically();
      memory_.AddBoard(board);
      return board;  // return the board pointer
    }

    PositionValue MakeMove(Board*& b);
    PositionValue Evaluate(Board* board, AIMethod method, Depth maxdepth, double timeLimit);
    void SetCallback(std::function<void()> cb) { assert(cb != nullptr); callback_ = cb; }
    void Purge(const Moves& sequence_) noexcept;  // 'forget' all boards that don't start with this move sequence
    size_t GetSize() const noexcept { return memory_.GetSize(); }

  private:
    PositionValue EvaluateDeeper(Board* board, AIMethod method, bool whiteonplay, PositionValue alpha, PositionValue beta, unsigned int plies) noexcept;
    Board* MoveAndRemember(Board* board, const Move& m) noexcept     // execute move and remember result
    {
      auto b = board->Clone();
      b->Execute(m);
      return Remember(b);
    }

  private:
    PositionMemory<Board> memory_{};
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
