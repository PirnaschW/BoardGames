
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
    constexpr LoAPeg() noexcept : Kind('L') {}

  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 0; } // in LoA, pieces have no value
    virtual void CollectMoves(const Board& board_, const Location& l, Moves& moves) const noexcept override;

// extensions
  public:
    std::vector<const Piece*> CollectAlong(const Board& board_, Location l, const Offset& o) const;
    void CollectMoves(const Board& board_, const Location& l, Moves& moves, int dx, int dy) const;

  public:
    static const LoAPeg ThePeg;
  };


  class LoAPiece : public Piece
  {
  private:
    LoAPiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    LoAPiece(const LoAPiece&) = delete;
    LoAPiece& operator=(const LoAPiece&) = delete;

  public:
    static const LoAPiece LoAPieceW;
    static const LoAPiece LoAPieceB;
  };


  class LoABoard : public Board
  {
  public:
    LoABoard(const VariantChosen& v, const PieceMapP& p, const BoardPartDimensions& d) noexcept : Board(v, p, d) {}
    virtual Board* Clone() const noexcept override { return new LoABoard(*this); }
    virtual void SetStartingBoard() noexcept override;
    virtual const Piece& SetPieceIndex(const Location& l, const Piece& p) noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically() const noexcept override;

    // extensions
  protected:
    bool IsConnected(bool t) const noexcept;

  private:
    struct Peg
    {
    public:
      constexpr Peg(const Location& ll) noexcept : l(ll) {}
    public:
      const Location l;
      bool mutable connected{false};
      bool mutable checked{false};
    };

  protected:
    std::list<Peg> llw{};
    std::list<Peg> llb{};
  };


  class LoAGame : public Game
  {
  private:
    LoAGame() = delete;

  public:
    LoAGame(const VariantChosen& v, const PieceMapP& m, const BoardPartDimensions& d) noexcept : Game(v, m, new LoABoard(v, m, d), new MainLayout(d, Layout::LayoutType::Light)) {}
    static const VariantList& GetVariants() noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
