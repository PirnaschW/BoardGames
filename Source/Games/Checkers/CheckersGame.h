namespace Checkers
{

  // Display Dimensions
  constexpr unsigned int FieldSizeX = 50;   // pixels per tile
  constexpr unsigned int FieldSizeY = 50;   // pixels per tile

  constexpr unsigned int FieldSizeSX = 20;  // pixels per small tile
  constexpr unsigned int FieldSizeSY = 18;  // pixels per small tile

  constexpr unsigned int BoardStartX = 30;  // offset for drawing the board
  constexpr unsigned int BoardStartY = 30;  // offset for drawing the board

  using namespace BoardGamesCore;

  class CheckersPosition : public MainPosition
  {
  public:
    CheckersPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(v, p, d) {}
    virtual inline MainPosition* Clone(void) const noexcept override { return new CheckersPosition(*this); }
    virtual void SetStartingPosition() noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual void GetAllMoves(void) const noexcept override;
    virtual PositionValue EvaluateStatically(void) const noexcept override;
// extensions
  public:
    virtual bool AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, const Piece& p, const Location& fr) const noexcept;
  protected:
    virtual bool CanPromote(const Location& l, const Piece& p) const noexcept;
    virtual Rule GetRule() const noexcept;
    bool HasRule(Rule r) const noexcept { return GetRule() & r; }
  };

  class CheckersGame : public Game
  {
  private:
    CheckersGame(void) = delete;
    static MainPosition* GetNewPosition(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept;

  public:
    inline CheckersGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, GetNewPosition(v, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}

#include "CheckersPieces.h"
#include "CheckersVariants.h"
#include "Turkish.h"
