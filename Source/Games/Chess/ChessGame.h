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

  class ChessBoard abstract: public Board
  {
  public:
    ChessBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept : Board(v, d) {}
    virtual void SetStartingBoard() noexcept override;  // should stay separate, to allow resetting a board
    virtual void GetAllMoves() const noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    constexpr virtual int GetMoveCountFactor() const noexcept override { return 20; }
    virtual void EvaluateStatically() const noexcept override;

    // extensions
  protected:
    bool HasRule(const Rule r) const noexcept { return GetRule() & r; }
    virtual Rule GetRule() const noexcept = 0;
    void SetDefaultStock() noexcept;
    void SetPawns(Coordinate row, const PieceColor& c) noexcept;
    void SetPiecesPSymmetrical(Coordinate x, Coordinate y, const PieceIndex b, const PieceIndex w) noexcept;  // use point symmetry
    void SetPiecesHSymmetrical(Coordinate x, Coordinate y, const PieceIndex b, const PieceIndex w) noexcept;  // use horizontal mirror symmetry
    Side PickRandomPiece() const noexcept;
  };

  class ChessGame : public Game
  {
  private:
    ChessGame() = delete;
    static Board* GetNewBoard(const VariantChosen& v, const BoardPartDimensions& d) noexcept;
//    static MainLayout* GetNewLayout(const VariantChosen& v, const BoardPartDimensions& d) noexcept;

  public:
    ChessGame(const VariantChosen& v, const std::vector<PieceIndex>& list, const BoardPartDimensions& d) noexcept : Game(v, list, GetNewBoard(v, d)) {}
    static void Register() noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}

#include "ChessPieces.h"
#include "ChessVariants.h"
