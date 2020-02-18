
namespace Espionage
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;


  class ETileColor : public TileColor
  {
  protected:
    inline ETileColor(const char& f, UINT ID_) noexcept : TileColor(f, ID_) {}

  public:
    static const ETileColor Volcano;
  };


  template <unsigned char z> // standard Soldiers 1 to 5
  class Soldier : public Kind
  {
  private:
    constexpr inline Soldier(void) noexcept : Kind(z) {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100 * (z - '0'); }

  public:
    static const Soldier TheSoldier;
  };

  class Spy : public Kind
  {
  private:
    constexpr inline Spy(void) noexcept : Kind('S') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 350; }

  public:
    static const Spy TheSpy;
  };

  class Sapper : public Kind
  {
  private:
    constexpr inline Sapper(void) noexcept : Kind('s') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 250; }

  public:
    static const Sapper TheSapper;
  };

  class Mine : public Kind
  {
  private:
    constexpr inline Mine(void) noexcept : Kind('B') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Mine TheMine;
  };

  class Headquarter : public Kind
  {
  private:
    constexpr inline Headquarter(void) noexcept : Kind('H') {}
  public:
    virtual inline unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Headquarter TheHeadquarter;
  };


  class EPiece : public Piece
  {
  private:
    inline EPiece(const Kind& k, const PieceColor& c, UINT ID) noexcept : Piece(k, c, ID) {}
    EPiece(const EPiece&) = delete;
    EPiece& operator=(const EPiece&) = delete;

  public:
    static const EPiece WSoldier1;
    static const EPiece BSoldier1;
    static const EPiece WSoldier2;
    static const EPiece BSoldier2;
    static const EPiece WSoldier3;
    static const EPiece BSoldier3;
    static const EPiece WSoldier4;
    static const EPiece BSoldier4;
    static const EPiece WSoldier5;
    static const EPiece BSoldier5;
    static const EPiece WSpy;
    static const EPiece BSpy;
    static const EPiece WSapper;
    static const EPiece BSapper;
    static const EPiece WMine;
    static const EPiece BMine;
    static const EPiece WHeadquarter;
    static const EPiece BHeadquarter;

    static const EPiece USoldier1;
    static const EPiece USoldier2;
    static const EPiece USoldier3;
    static const EPiece USoldier4;
    static const EPiece USoldier5;
    static const EPiece USpy;
    static const EPiece USapper;
    static const EPiece UMine;
    static const EPiece UHeadquarter;
  };

  class EspionageLayout final : public MainLayout
  {
  public:
    EspionageLayout(const Dimensions& d) noexcept;
  };

  class EspionagePosition : public MainPosition
  {
  public:
    EspionagePosition(const PieceMapP& p, const Dimensions& d) noexcept;
    virtual inline MainPosition* Clone(void) const noexcept override { return new EspionagePosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
  };


  class EspionageGame : public Game
  {
  private:
    EspionageGame(void) = delete;

  public:
    inline EspionageGame(const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new EspionagePosition(m, d), new EspionageLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(void) noexcept;
    static const Dimensions& GetDimensions(Coordinate x, Coordinate y) noexcept;
  };

}
