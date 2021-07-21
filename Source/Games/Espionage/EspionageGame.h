
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
    ETileColor(const char& f, unsigned int ID_) noexcept : TileColor(f, ID_) {}

  public:
    static const ETileColor Volcano;
  };


  class EKind : public Kind  // helper class as all Espionage kinds move the same way
  {
  protected:
    constexpr EKind(unsigned char z) noexcept : Kind(z) {}
  public:
    virtual void CollectMoves(const MainPosition&, const Location&, Moves&) const noexcept override;
    virtual bool CanTake(const Kind&) const noexcept override;
  };

  template <unsigned char z> // standard Generals 1 to 5
  class General : public EKind
  {
  private:
    constexpr General(void) noexcept : EKind(z) {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100 * (z - '0'); }

  public:
    static const General TheGeneral;
  };

  class Spy : public EKind
  {
  private:
    constexpr Spy(void) noexcept : EKind('S') {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 350; }

  public:
    static const Spy TheSpy;
  };

  class Sapper : public EKind
  {
  private:
    constexpr Sapper(void) noexcept : EKind('s') {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 250; }

  public:
    static const Sapper TheSapper;
  };

  class Mine : public Kind
  {
  private:
    constexpr Mine(void) noexcept : Kind('B') {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Mine TheMine;
  };

  class Headquarter : public Kind
  {
  private:
    constexpr Headquarter(void) noexcept : Kind('H') {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 100; }

  public:
    static const Headquarter TheHeadquarter;
  };

  class Volcano : public Kind
  {
  private:
    constexpr Volcano(void) noexcept : Kind('V') {}
  public:
    virtual unsigned int GetValue(const MainPosition& /*p*/, const Location& /*l*/) const noexcept override { return 0; }

  public:
    static const Volcano TheVolcano;
  };


  class EPiece : public Piece
  {
  private:
    EPiece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : Piece(k, c, ID) {}
    EPiece(const EPiece&) = delete;
    EPiece& operator=(const EPiece&) = delete;

  public:
    static const EPiece WGeneral1;
    static const EPiece WGeneral2;
    static const EPiece WGeneral3;
    static const EPiece WGeneral4;
    static const EPiece WGeneral5;
    static const EPiece WSpy;
    static const EPiece WSapper;
    static const EPiece WMine;
    static const EPiece WHeadquarter;

    static const EPiece BGeneral1;
    static const EPiece BGeneral2;
    static const EPiece BGeneral3;
    static const EPiece BGeneral4;
    static const EPiece BGeneral5;
    static const EPiece BSpy;
    static const EPiece BSapper;
    static const EPiece BMine;
    static const EPiece BHeadquarter;

    static const EPiece UGeneral1;
    static const EPiece UGeneral2;
    static const EPiece UGeneral3;
    static const EPiece UGeneral4;
    static const EPiece UGeneral5;
    static const EPiece USpy;
    static const EPiece USapper;
    static const EPiece UMine;
    static const EPiece UHeadquarter;

    static const EPiece Volcano;
  };

  class EspionageLayout final : public MainLayout
  {
  public:
    EspionageLayout(const Dimensions& d) noexcept;

  private:
    const TileColor& FC(Coordinate i, Coordinate j) const noexcept;
  };

  class EspionagePosition : public MainPosition
  {
  public:
    EspionagePosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept;
    virtual MainPosition* Clone(void) const noexcept override { return new EspionagePosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
  };


  class EspionageGame : public Game
  {
  private:
    EspionageGame(void) = delete;

  public:
    enum class FieldType { Standard, Volcano };
    static FieldType GetFieldType(const Coordinate& sizeX, const Coordinate& sizeY, const Coordinate& x, const Coordinate& y) noexcept;


  public:
    EspionageGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, new EspionagePosition(v, m, d), new EspionageLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
