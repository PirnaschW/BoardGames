#include "Games.h"

#include "TankBResource.h"
#include "TankBGame.h"

namespace TankB
{
  const TBTileColor TBTileColor::Green{ 'G', IDB_XXG };
  const TBTileColor TBTileColor::Blue { 'B', IDB_XXB };
  const TBTileColor TBTileColor::Red  { 'R', IDB_XXR };
  const TBTileColor TBTileColor::Wall { 'W', IDB_WALL };
                                          
  const NTank NTank::TheNTank{};
  const CTank CTank::TheCTank{};

  const NTankPiece NTankPiece::NTankW{ NTank::TheNTank, PieceColor::White, IDB_NTANKW };
  const NTankPiece NTankPiece::NTankB{ NTank::TheNTank, PieceColor::Black, IDB_NTANKB };
  const CTankPiece CTankPiece::CTankW{ CTank::TheCTank, PieceColor::White, IDB_CTANKW };
  const CTankPiece CTankPiece::CTankB{ CTank::TheCTank, PieceColor::Black, IDB_CTANKB };


  void NTank::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, +z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+0, -z)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(+z, +0)); z++);
    for (int z = 1; p.AddIfLegal(moves, l, l + Offset(-z, -0)); z++);
  }

  TankBLayout::TankBLayout(const Dimensions& d) noexcept : MainLayout(d, LayoutType::None)
  {
    unsigned int z = 0;
    for (Coordinate i = 0; i < dim_.xCount_; i++)
      for (Coordinate j = 0; j < dim_.yCount_; j++, z++)
      {
        const Rect r{
          (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * i),
          (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * j),
          (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * (i + 1U)),
          (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * (j + 1U)) };

        tiles_[z] = new Tile(Location(BoardPart::Main, i, j), r, FC(i, j));
      }
  }

  const TileColor& TankBLayout::FC(Coordinate i, Coordinate j) const noexcept
  {
    switch (TankBGame::GetFieldType(dim_.xCount_, dim_.yCount_, i, j))
    {
      case TankBGame::FieldType::Standard:     return TBTileColor::Light;
      case TankBGame::FieldType::HomeWhite:    return TBTileColor::Green;
      case TankBGame::FieldType::HomeBlack:    return TBTileColor::Blue;
      case TankBGame::FieldType::Contaminated: return TBTileColor::Red;
      case TankBGame::FieldType::Wall:         return TBTileColor::Wall;
      default:                                 return TBTileColor::Light;
    }
  }


  TankBPosition::TankBPosition(VariantCode c, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(c, p, d)
  {
    SetPiece(Location(BoardPart::Main, 10U,  6U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main, 10U,  7U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main, 10U,  8U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main, 10U,  9U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main, 10U, 10U), CTankPiece::CTankW);
    SetPiece(Location(BoardPart::Main,  9U,  7U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main,  9U,  8U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main,  9U, 10U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main,  8U,  8U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main,  8U,  9U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main,  8U, 10U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main,  7U,  9U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main,  7U, 10U), NTankPiece::NTankW);
    SetPiece(Location(BoardPart::Main,  6U, 10U), NTankPiece::NTankW);

    SetPiece(Location(BoardPart::Main,  0U,  0U), CTankPiece::CTankB);
    SetPiece(Location(BoardPart::Main,  0U,  1U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  0U,  2U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  0U,  3U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  0U,  4U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  1U,  0U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  1U,  2U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  1U,  3U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  2U,  0U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  2U,  1U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  2U,  2U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  3U,  0U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  3U,  1U), NTankPiece::NTankB);
    SetPiece(Location(BoardPart::Main,  4U,  0U), NTankPiece::NTankB);
  }


  bool TankBPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);                                      // piece to move
    if (pf == Piece::NoTile) return false;                               // out of board
    if (pf.IsBlank()) return false;                                      // tile not occupied

    const Piece& pt = GetPiece(to);                                      // piece on target field
    if (pt == Piece::NoTile) return false;                               // out of board
    if (!pt.IsBlank() && pt.IsColor(pf.GetColor())) return false;        // own piece

    switch (TankBGame::GetFieldType(sizeX_, sizeY_, to.x_, to.y_))
    {
      case TankBGame::FieldType::Contaminated: return false;             // contaminated zone
      case TankBGame::FieldType::Wall:         return false;             // Wall
      default:;                                                          // fine
    }

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                   // pick piece up
    if (!pt.IsBlank())
    {
      a.push_back(std::make_shared<ActionLift>(to, pt));                 // pick opponent piece up
      a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pf.GetColor()), pt));                   // place it in Taken
    }
    a.push_back(std::make_shared<ActionDrop>(to, pf));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                              // add move to move list
    return pt.IsBlank();
  };

  PositionValue TankBPosition::EvaluateStatically(void) const noexcept
  {
    return MainPosition::EvaluateStatically();
    // ...
  }


  const std::vector<Location> TankBGame::walls_{
  {BoardPart::Main,  0U,  5U },
  {BoardPart::Main,  1U,  1U },
  {BoardPart::Main,  1U,  9U },
  {BoardPart::Main,  2U,  3U },
  {BoardPart::Main,  2U,  5U },
  {BoardPart::Main,  2U,  7U },
  {BoardPart::Main,  3U,  2U },
  {BoardPart::Main,  3U,  8U },
  {BoardPart::Main,  5U,  0U },
  {BoardPart::Main,  5U,  2U },
  {BoardPart::Main,  5U,  8U },
  {BoardPart::Main,  5U, 10U },
  {BoardPart::Main,  7U,  2U },
  {BoardPart::Main,  7U,  8U },
  {BoardPart::Main,  8U,  3U },
  {BoardPart::Main,  8U,  5U },
  {BoardPart::Main,  8U,  7U },
  {BoardPart::Main,  9U,  1U },
  {BoardPart::Main,  9U,  9U },
  {BoardPart::Main, 10U,  5U },
  };

  TankBGame::FieldType TankBGame::GetFieldType(const Coordinate& sizeX, const Coordinate& sizeY, const Coordinate& x, const Coordinate& y) noexcept
  {
    assert(sizeX == 11 && sizeY == 11); // for now, we only support 11x11

    if (x + y < 5)                                                return TankBGame::FieldType::HomeBlack;
    if (x + y > 15)                                               return TankBGame::FieldType::HomeWhite;
    if (x > 3 && x < 7 && y > 3 && y < 7)                         return TankBGame::FieldType::Contaminated;
    for (const Location& w : walls_) if (w.x_ == x && w.y_ == y)  return TankBGame::FieldType::Wall;
    return                                                               TankBGame::FieldType::Standard;
  }

  const VariantList& TankBGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ nullptr, 'A', 11, 11 } },
    };
    return v;
  }

  const PieceMapP& TankBGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(NTankPiece::NTankW);
    p->Add(NTankPiece::NTankB);
    p->Add(CTankPiece::CTankW);
    p->Add(CTankPiece::CTankB);
    return p;
  }

  const Dimensions& TankBGame::GetDimensions(const VariantChosen& v) noexcept
  {
    static Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(15, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(3 * v.x, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.x - FieldSizeSY * 4),
    };
    return d;
  }
}
