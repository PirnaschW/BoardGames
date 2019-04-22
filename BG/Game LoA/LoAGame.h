
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
    constexpr inline LoAPeg(void) noexcept : Kind('L') {}

  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 0; } // in LoA, pieces have no value
    virtual void CollectMoves(const MainPosition& pos, const Location& l, Moves& moves) const override;

// extensions
  public:
    std::vector<const Piece*> CollectAlong(const MainPosition& pos, Location l, const Offset& o) const;
    void CollectMoves(const MainPosition& pos, const Location& l, Moves& moves, int dx, int dy) const;

  public:
    static const LoAPeg ThePeg;
  };


  class LoAPiece : public Piece
  {
  private:
    inline LoAPiece(const Kind* k, const Color* c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
    LoAPiece(const LoAPiece&) = delete;
    LoAPiece& operator=(const LoAPiece&) = delete;

  public:
    static const LoAPiece LoAPieceW; //{ &LoAPeg::ThePeg, &Color::White, IDB_WCL, IDB_WCD, IDB_WCS };
    static const LoAPiece LoAPieceB; //{ &LoAPeg::ThePeg, &Color::Black, IDB_BCL, IDB_BCD, IDB_BCS };
  };


  class LoAPosition : public MainPosition
  {
  public:
    LoAPosition(Coordinate x, Coordinate y);
    virtual inline MainPosition* Clone(void) const override { return new LoAPosition(*this); }
    virtual const Piece* SetPiece(const Location& l, const Piece* p) noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location fr, const Location to) const override;
    virtual void EvaluateStatically(void) override;

    // extensions
  protected:
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
    std::list<Peg> llw{};
    std::list<Peg> llb{};
  };


  class LoALayout : public MainLayout
  {
  public:
    LoALayout(Coordinate x, Coordinate y) noexcept;
  };

  class LoATakenLayout : public TakenLayout
  {
  public:
    LoATakenLayout(Coordinate x, Coordinate y) noexcept;
  };

  class LoAStockLayout : public StockLayout
  {
  public:
    LoAStockLayout(Coordinate x, Coordinate y) noexcept;
  };


  class LoAGame : public Game
  {
  private:
    LoAGame(void) = delete;
    LoAGame(LoAPosition* p, TakenPosition* t, StockPosition* s, LoALayout* l, LoATakenLayout* tl, LoAStockLayout* sl) noexcept;

  public:
    LoAGame(Coordinate x, Coordinate y) noexcept;
    static const VariantList& GetVariants(void) noexcept;
  };

}
