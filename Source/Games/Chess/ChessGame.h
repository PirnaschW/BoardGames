#include "ChessVariants.h"
#include "ChessPieces.h"

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
    ChessBoard(const VariantChosen& v, const PieceMapP& p, const BoardPartDimensions& d) noexcept : Board(v, p, d) {};
    virtual void SetStartingBoard() noexcept override;
    virtual void GetAllMoves() const noexcept override;
    virtual bool AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept override;
    constexpr virtual int GetMoveCountFactor() const noexcept override { return 20; }
    [[ nodiscard ]] virtual PositionValue EvaluateStatically() const noexcept override;


    // extensions
  protected:
    void SetPawns(Coordinate row, const PieceColor& c) noexcept;
    void SetPiecesPSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept;  // use point symmetry
    void SetPiecesHSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept;  // use horizontal mirror symmetry
    Side PickRandomPiece() const noexcept;
  };

  // this class has no function, but is needed as base for the variant templates
  class ChessLayout : public MainLayout
  {
  public:
    ChessLayout(const BoardPartDimensions& d, LayoutType lt = LayoutType::Alternating) noexcept : MainLayout(d,lt) {}
    virtual ~ChessLayout() noexcept {}
    virtual void Draw(DC* pDC, const Board* board_, Mode mode_) const { MainLayout::Draw(pDC, board_, mode_); }
  };


  class ChessGame : public Game
  {
  private:
    ChessGame() = delete;
    static Board* GetNewBoard(const VariantChosen& v, const PieceMapP& m, const BoardPartDimensions& d) noexcept;
    static MainLayout* GetNewLayout(const VariantChosen& v, const BoardPartDimensions& d) noexcept;

  public:
    ChessGame(const VariantChosen& v, const PieceMapP& m, const BoardPartDimensions& d) noexcept : Game(v, m, GetNewBoard(v, m, d), GetNewLayout(v,d)) {}
    static const VariantList& GetVariants() noexcept;
    static const PieceMapP& GetPieces(const VariantChosen& v) noexcept;
    static const BoardPartDimensions GetDimensions(const VariantChosen& v) noexcept;
  };

}
