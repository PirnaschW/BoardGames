
namespace CFour
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class Checker : public Kind
  {
  private:
    constexpr inline Checker(void) noexcept : Kind('0') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 0; }

  public:
    static const Checker TheChecker;
  };

  class CFourPiece : public Piece
  {
  private:
    inline CFourPiece(const Kind& k, const PieceColor& c, UINT ID) noexcept : Piece(k, c, ID) {}
    CFourPiece(const CFourPiece&) = delete;
    CFourPiece& operator=(const CFourPiece&) = delete;
  public:
    ~CFourPiece(void) override {}

  public:
    static const CFourPiece CFourPieceW;
    static const CFourPiece CFourPieceB;
  };


  class CFourPosition : public MainPosition
  {
  public:
    inline CFourPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d) {}
    virtual inline MainPosition* Clone(void) const noexcept override { return new CFourPosition(*this); }
    virtual void GetAllMoves(void) const noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
    virtual unsigned int GetChainValue(unsigned int z) const noexcept override;
  };


  class CFourGame : public Game
  {
  private:
    CFourGame(void) = delete;

  public:
    inline CFourGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new CFourPosition(m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
