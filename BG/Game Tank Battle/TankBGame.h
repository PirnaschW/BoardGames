
namespace TankB
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;


  class TBTileColor : public TileColor
  {
  protected:
    inline TBTileColor(const char& f, UINT ID_) noexcept : TileColor(f,ID_) {}

  public:
    static const TBTileColor Green;
    static const TBTileColor Blue;
    static const TBTileColor Red;
  };


  class Wall : public Kind  // Wall
  {
  private:
    constexpr inline Wall(void) noexcept : Kind('W') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 0; }

  public:
    static const Wall TheWall;
  };

  class NTank : public Kind  // Normal Tank
  {
  private:
    constexpr inline NTank(void) noexcept : Kind('N') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }
    virtual inline void CollectMoves(const MainPosition& /*p*/, const Location& /*l*/, Moves& /*m*/) const noexcept override;                    // useful default implementation: No moves possible

  public:
    static const NTank TheNTank;
  };
  
  class CTank : public Kind // Command Tank
  {
  private:
    constexpr inline CTank(void) noexcept : Kind('C') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 400; }

  public:
    static const CTank TheCTank;
  };


  class TankBWall : public Piece
  {
  private:
    inline TankBWall(const Kind& k, const PieceColor& c, UINT ID) noexcept : Piece(k, c, ID) {}
  public:
    static const TankBWall TheWall;
  };

  class NTankPiece : public Piece
  {
  protected:
    inline NTankPiece(const Kind& k, const PieceColor& c, UINT ID) noexcept : Piece(k, c, ID) {}
  public:
    static const NTankPiece NTankW;
    static const NTankPiece NTankB;
  };

  class CTankPiece : public NTankPiece
  {
  private:
    inline CTankPiece(const Kind& k, const PieceColor& c, UINT ID) noexcept : NTankPiece(k, c, ID) {}
  public:
    static const CTankPiece CTankW;
    static const CTankPiece CTankB;
  };


  class TankBLayout final : public MainLayout
  {
  public:
    TankBLayout(const Dimensions& d) noexcept;

  private:
    const TileColor& FC(Coordinate i, Coordinate j) const noexcept;
  };

  class TankBPosition : public MainPosition
  {
  public:
    TankBPosition(const PieceMapP& p, const Dimensions& d) noexcept;
    virtual inline MainPosition* Clone(void) const noexcept override { return new TankBPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;

  private:
    bool IsContaminated(const Location& l) const noexcept;
  };


  class TankBGame : public Game
  {
  private:
    TankBGame(void) = delete;

  public:
    inline TankBGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new TankBPosition(m, d), new TankBLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
