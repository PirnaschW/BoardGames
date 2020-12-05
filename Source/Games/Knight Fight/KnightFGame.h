
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
    constexpr Checker(void) noexcept : Kind('0') {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

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
    ~KnightFPiece(void) override {}

  public:
    static const KnightFPiece KnightFPieceW;
    static const KnightFPiece KnightFPieceB;
  };


  class KnightFPosition : public MainPosition
  {
  public:
    KnightFPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept;
    virtual MainPosition* Clone(void) const noexcept override { return new KnightFPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
  private:
    std::vector<FieldValue> fValue_{};
  };


  class KnightFGame : public Game
  {
  private:
    KnightFGame(void) = delete;

  public:
    KnightFGame(VariantCode c, const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new KnightFPosition(c, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions& GetDimensions(const VariantChosen& v) noexcept;
  };

}
