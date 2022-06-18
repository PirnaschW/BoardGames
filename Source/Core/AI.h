
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
    ~AI() = default;

    void Insert(Board* board) noexcept { memory_.Insert(board); /*return board; */ }
    size_t GetSize() const noexcept { return memory_.GetSize(); }
    void SetCallback(std::function<void()> cb) { if (cb) callback_ = cb; else throw std::exception("cannot callback nullptr"); }

    PositionValue MakeMove(Board*& b);
    PositionValue Evaluate(Board* board, AIMethod method, Depth maxdepth, double timeLimit);

  private:
    PositionValue EvaluateDeeper(Board* board, AIMethod method, bool whiteonplay, PositionValue alpha, PositionValue beta, unsigned int plies) noexcept;

  private:
    inline static const std::function<void(Board*)> preinsert_{ [](Board* b) -> void { b->GetAllMoves(); b->EvaluateStatically(); } };
    PositionMemory<Board> memory_{ preinsert_ };
    std::function<void()> callback_{ nullptr };      // pointer to CDocument's callback function (to update window)
  };

}
