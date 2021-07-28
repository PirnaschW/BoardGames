
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
}

#include "ChessVariants.h"
#include "ChessPieces.h"

namespace Chess
{

  class ChessPosition abstract: public MainPosition
  {
  public:
    ChessPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(v, p, d) {};
    virtual void SetStartingPosition() noexcept override;
    virtual void GetAllMoves(void) const noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    constexpr virtual unsigned int GetMoveCountFactor(void) const noexcept override { return 20; }
    [[ nodiscard ]] virtual PositionValue EvaluateStatically(void) const noexcept override;


    // extensions
  protected:
    void SetPawns(Coordinate row, const PieceColor& c) noexcept;
    void SetPiecesPSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept;  // use point symmetry
    void SetPiecesHSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept;  // use horizontal mirror symmetry
  };

  class ChessGame : public Game
  {
  private:
    ChessGame(void) = delete;
    static MainPosition* GetNewPosition(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept;

  public:
    ChessGame(const VariantChosen& v, const PieceMapP& m, const Dimensions& d) noexcept : Game(v, m, GetNewPosition(v, m, d), new MainLayout(d)) {}
    static const VariantList& GetVariants(void) noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const Dimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
