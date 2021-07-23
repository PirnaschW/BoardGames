

namespace Chess
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;  // pixels per tile
  constexpr unsigned int FieldSizeY = 50;  // pixels per tile

  constexpr unsigned int FieldSizeSX = 18;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 20;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class ChessPosition : public MainPosition
  {
  public:
    ChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept;
    virtual MainPosition* Clone(void) const noexcept override { return new ChessPosition(*this); }
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual unsigned int GetMoveCountFactor(void) const noexcept override { return 1000; }

// extensions
  private:
    virtual bool PlaceRandomly(const Piece& p);
  };

  class ChessGame : public Game
  {
  private:
    ChessGame(void) = delete;

  public:
    ChessGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, new ChessPosition(v, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
