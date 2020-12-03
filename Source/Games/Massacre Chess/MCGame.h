

namespace MassacreChess
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;  // pixels per tile
  constexpr unsigned int FieldSizeY = 50;  // pixels per tile

  constexpr unsigned int FieldSizeSX = 18;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 20;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class MCPosition : public MainPosition
  {
  public:
    MCPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept;
    virtual MainPosition* Clone(void) const noexcept override { return new MCPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual unsigned int GetMoveCountFactor(void) const noexcept override { return 1000; }

// extensions
  private:
    virtual bool PlaceRandomly(const Piece& p);
  };

  class MCGame : public Game
  {
  private:
    MCGame(void) = delete;

  public:
    MCGame(VariantCode c, const PieceMapP& m, const Dimensions& d) noexcept : Game(m, new MCPosition(c, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(VariantCode c) noexcept;
    static const Dimensions& GetDimensions(const VariantChosen& v) noexcept;
  };

}
