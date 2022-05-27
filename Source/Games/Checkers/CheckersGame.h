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

  class CheckersBoard : public Board
  {
  public:
    CheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : Board(v, d) {}
    virtual inline Board* Clone() const noexcept override { return new CheckersBoard(*this); }
    virtual void SetStartingBoard() noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    //virtual void GetAllMoves() const noexcept override;
    //virtual void EvaluateStatically() const noexcept override;
// extensions
  public:
    virtual bool AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, PieceIndex p, const Location& fr) const noexcept;
  protected:
    /*[[deprecated]] */virtual bool CanPromote(const Location& l, const Piece& p) const noexcept;
    virtual bool CanPromote(const Location& l, PieceIndex p) const noexcept;
    virtual Rule GetRule() const noexcept;
    bool HasRule(Rule r) const noexcept { return GetRule() & r; }
  };

  class CheckersGame : public Game
  {
  private:
    CheckersGame() = delete;
    static Board* GetNewBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept;

  public:
    inline CheckersGame(const VariantChosen& v, const BoardPartDimensions& d) noexcept : Game(v, GetNewBoard(v, d)) {}
    static void Register() noexcept;
    static const VariantList& GetVariants() noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}

#include "CheckersPieces.h"
#include "CheckersVariants.h"
#include "Turkish.h"
