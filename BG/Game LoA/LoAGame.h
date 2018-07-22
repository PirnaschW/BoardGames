
namespace LoA
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 32;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 32;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class LoAPeg : public Kind
  {
  private:
    constexpr LoAPeg(void) noexcept : Kind('L') {}

  public:
    inline std::vector<const Piece*> CollectAlong(const MainPosition& pos, Location l, const Offset& o) const
    {
      std::vector<const Piece*> along{};
      const Piece* p{};
      while ((p = pos.GetPiece(l = l + o)) != nullptr) along.push_back(p);
      return along;
    }

    void CollectMoves(const MainPosition& pos, const Location& l, std::vector<Move>& moves, int dx, int dy) const
    {
      unsigned int s{pos.GetPiece(l)->IsBlank() ? 0U : 1U};
      std::vector<const Piece*> a1 = CollectAlong(pos, l, Offset(dx, dy));
      std::vector<const Piece*> a2 = CollectAlong(pos, l, Offset(-dx, -dy));

      for (auto& p : a1) if (!p->IsBlank()) s++;
      for (auto& p : a2) if (!p->IsBlank()) s++;

      if (a1.size() >= s) // will a move even fit on the board?
      {
        bool free{true};
        for (unsigned int i = 0; free && i < s; i++)
        {
          if (i == s - 1) // last = target place needs to be empty or opponent's
          {
            if (!a1[i]->IsBlank() && a1[i]->IsColor(pos.OnTurn())) free = false;
          }
          else  // intermediate places need to be empty or own
          {
            if (!a1[i]->IsBlank() && !a1[i]->IsColor(pos.OnTurn())) free = false;
          }
        }
        if (free) pos.AddIfLegal(moves, l, l + Offset(dx * s, dy*s));
      }

      if (a2.size() >= s) // will a move even fit on the board?
      {
        bool free{true};
        for (unsigned int i = 0; free && i < s; i++)
        {
          if (i == s - 1) // last = target place needs to be empty or opponent's
          {
            if (!a2[i]->IsBlank() && a2[i]->IsColor(pos.OnTurn())) free = false;
          }
          else  // intermediate places need to be empty or own
          {
            if (!a2[i]->IsBlank() && !a2[i]->IsColor(pos.OnTurn())) free = false;
          }
        }
        if (free) pos.AddIfLegal(moves, l, l + Offset(-dx * s, -dy * s));
      }
    }

    void CollectMoves(const MainPosition& pos, const Location& l, std::vector<Move>& moves) const override
    {
      CollectMoves(pos, l, moves, 1, 0); // check horizontal moves
      CollectMoves(pos, l, moves, 0, 1); // check vertical moves
      CollectMoves(pos, l, moves, 1, -1); // check diagonal '/' moves
      CollectMoves(pos, l, moves, 1, 1); // check diagonal '\' moves
    }

    unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 0; } // in LoA, pieces have no value


  public:
    inline const static LoAPeg ThePeg{};
  };


  class LoAPiece : public Piece
  {
  private:
    LoAPiece(const Kind* k, const Color* c, UINT l, UINT s) noexcept : Piece(k, c, l, l, s) {}
    LoAPiece(const LoAPiece&) = delete;
    LoAPiece& operator=(const LoAPiece&) = delete;

  public:
    static const LoAPiece& GetPiece(unsigned int z) noexcept
    {
      switch (z)
      {
        case 1: return LoAPiece::LoAPieceW;
        case 2: return LoAPiece::LoAPieceB;
      }
    }

    static const LoAPiece LoAPieceB;
    static const LoAPiece LoAPieceW;
  };


  class LoAPosition : public MainPosition
  {
  public:
    LoAPosition(Coordinate x, Coordinate y);
    MainPosition* Clone(void) const override { return new LoAPosition(*this); }
    virtual const Piece* SetPiece(const Location& l, const Piece* p) noexcept override;
    bool AddIfLegal(std::vector<Move>& m, const Location fr, const Location to) const override;
    void EvaluateStatically(void) override;

  protected: // extensions to base class
    bool IsConnected(bool t) const noexcept;

  private:
    struct Peg
    {
    public:
      constexpr inline Peg(const Location& ll) noexcept : l(ll) {}
    public:
      const Location l;
      bool mutable connected{false};
      bool mutable checked{false};
    };

  protected:
    std::list<Peg> llw;
    std::list<Peg> llb;
  };


  class LoALayout : public MainLayout
  {
  public:
    LoALayout(unsigned int x, unsigned int y) noexcept;
  };

  class LoATakenLayout : public TakenLayout
  {
  public:
    LoATakenLayout(unsigned int x, unsigned int y) noexcept;
  };

  class LoAStockLayout : public StockLayout
  {
  public:
    LoAStockLayout(unsigned int x, unsigned int y) noexcept;
  };


  class LoAGame : public Game
  {
  private:
    LoAGame(LoAPosition* p, TakenPosition* t, StockPosition* s,
      LoALayout* l, LoATakenLayout* tl, LoAStockLayout* sl) noexcept : Game{p,t,s,l,tl,sl}
    {
      AddToStock(Location(0, 0), &LoAPiece::LoAPieceW);
      AddToStock(Location(1, 0), &LoAPiece::LoAPieceB);
    }

  public:
    LoAGame(unsigned int x, unsigned int y) : LoAGame(
      new LoAPosition(x, y), new TakenPosition(2 * x, 2), new StockPosition(3, 1),
      new LoALayout(x, y), new LoATakenLayout(x, y), new LoAStockLayout(x, y)) {}
    inline static const VariantList& GetVariants(void) noexcept { static VariantList v{ { Variant{ 8, 8, nullptr, 3, 20 } } }; return v; }
  };

}
