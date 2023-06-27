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

  class CheckersBoard abstract : public Board
  {
  public:
    CheckersBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : Board(v, d) {}
    virtual void SetStartingBoard() noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    virtual bool AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, PieceIndex p, const Location& fr) const noexcept;
  protected:
    void SetDefaultStock() noexcept;
    void SetQueensStock() noexcept;
    virtual bool CanPromote(const Location& l, const PieceIndex p) const noexcept;
    virtual Rule GetRule() const noexcept = 0;

  private://protected:
    bool HasRule(const Rule r) const noexcept { return GetRule() & r; }
  };

  class CheckersGame : public Game
  {
  private:
    CheckersGame() = delete;
    static Board* GetNewBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept;

  public:
    inline CheckersGame(const VariantChosen& v, const std::vector<PieceIndex>& list, const BoardPartDimensions& d) noexcept : Game(v, list, GetNewBoard(v, d)) {}
    static void Register() noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}

#include "CheckersPieces.h"
#include "CheckersVariants.h"
