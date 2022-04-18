
namespace KnightF
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  using FieldValue = unsigned char;

  class Checker : public Kind
  {
  private:
    constexpr Checker() noexcept : Kind('0') {}
  public:
    virtual unsigned int GetValue(const Board& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Checker TheChecker;
  };

  class KnightFPiece : public Piece
  {
  private:
    KnightFPiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    KnightFPiece(const KnightFPiece&) = delete;
    KnightFPiece& operator=(const KnightFPiece&) = delete;
  public:
    ~KnightFPiece() override {}

  public:
    static const KnightFPiece KnightFPieceW;
    static const KnightFPiece KnightFPieceB;
  };


  class KnightFBoard : public Board
  {
  public:
    KnightFBoard(const VariantChosen& v, const PieceMapP& p, const BoardPartDimensions& d) noexcept : Board(v, p, d) {};
    virtual Board* Clone() const noexcept override { return new KnightFBoard(*this); }
    virtual void SetStartingBoard() noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically() const noexcept override;
  private:
    std::vector<FieldValue> fValue_{};
  };


  class KnightFGame : public Game
  {
  private:
    KnightFGame() = delete;

  public:
    KnightFGame(const VariantChosen& v, const PieceMapP& m, const BoardPartDimensions& d) noexcept : Game(v, m, new KnightFBoard(v, m, d)) {}
    static const VariantList& GetVariants() noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
