
namespace Cam
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class Pawn : public Kind
  {
  private:
    constexpr inline Pawn(void) noexcept : Kind('P') {}

  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 100; };
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const Pawn ThePawn;
  };

  class Knight : public Kind
  {
  private:
    constexpr inline Knight(void) noexcept : Kind('N') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location /*l*/) const noexcept override { return 400; }
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const override;

  public:
    static const Knight TheKnight;
  };


  class CamPiece : public Piece
  {
  private:
    inline CamPiece(const Kind* k, const Color* c, UINT l, UINT d, UINT s) noexcept : Piece(k, c, l, d, s) {}
    CamPiece(const CamPiece&) = delete;
    CamPiece& operator=(const CamPiece&) = delete;
    virtual unsigned int GetValue(const MainPosition& p, const Location l) const noexcept override;

  public:
    static const CamPiece WP;
    static const CamPiece WN;
    static const CamPiece BP;
    static const CamPiece BN;
  };


  class CamPosition : public MainPosition
  {
  public:
    CamPosition(const PieceMapP& p, const Dimensions& d) noexcept;
    virtual inline MainPosition* Clone(void) const override { return new CamPosition(*this); }
    virtual void GetAllMoves(void) override;

    // extensions:
  public:
    bool CollectJumps(const Location& fr, const Steps& s, bool charge, const Color* c, Moves& m) const;
  };

  class CamGame : public Game
  {
  private:
    CamGame(void) = delete;
  public:
    inline CamGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new CamPosition(m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
