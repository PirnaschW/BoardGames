#include "pch.h"

#include "TankBGame.h"

namespace TankB
{
  const TBTileColor TBTileColor::Green{ 'G', IDB_XXG };
  const TBTileColor TBTileColor::Blue { 'B', IDB_XXB };
  const TBTileColor TBTileColor::Red  { 'R', IDB_XXR };
                                          
  const Wall  Wall ::TheWall {};
  const NTank NTank::TheNTank{};
  const CTank CTank::TheCTank{};

  const TankBWall TankBWall::TheWall { Wall::TheWall,  PieceColor::Void,  IDB_WALL   };

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
        const CRect r{
          (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * i),
          (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * j),
          (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * (i + 1U)),
          (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * (j + 1U)) };

        tiles_[z] = new Tile(Location(BoardPart::Main, i, j), r, FC(i, j));
      }
  }

  const TileColor& TankBLayout::FC(Coordinate i, Coordinate j) const noexcept
  {
    if (i + j <  5)                       return TBTileColor::Green;
    if (i + j > 15)                       return TBTileColor::Blue;
    if (i > 3 && i < 7 && j > 3 && j < 7) return TBTileColor::Red;
    return TileColor::Light;
  }


  TankBPosition::TankBPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
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

    // Walls
    SetPiece(Location(BoardPart::Main,  0U,  5U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  1U,  1U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  1U,  9U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  2U,  3U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  2U,  5U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  2U,  7U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  3U,  2U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  3U,  8U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  5U,  0U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  5U,  2U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  5U,  8U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  5U, 10U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  7U,  2U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  7U,  8U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  8U,  3U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  8U,  5U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  8U,  7U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  9U,  1U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main,  9U,  9U), TankBWall::TheWall);
    SetPiece(Location(BoardPart::Main, 10U,  5U), TankBWall::TheWall);
  }


  bool TankBPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);                                      // piece to move
    if (pf == Piece::NoTile) return false;                               // out of board
    if (pf.IsBlank()) return false;                                      // tile not occupied

    const Piece& pt = GetPiece(to);                                      // piece on target field
    if (pt == Piece::NoTile) return false;                               // out of board
    if (pt == TankBWall::TheWall) return false;                          // wall
    if (IsContaminated(to)) return false;                                // contaminated zone
    if (!pt.IsBlank() && pt.IsColor(pf.GetColor())) return false;        // own piece

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

  bool TankBPosition::IsContaminated(const Location& l) const noexcept
  {
    return false;
  }


  const VariantList& TankBGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 11, 11, nullptr, 2, 20 } } };
    return v;
  }

  const PieceMapP& TankBGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(NTankPiece::NTankW);
    p->Add(NTankPiece::NTankB);
    p->Add(CTankPiece::CTankW);
    p->Add(CTankPiece::CTankB);
    p->Add(TankBWall::TheWall);
    return p;
  }

  const Dimensions& TankBGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(15, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(3 * x, 2, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * x - FieldSizeSY * 4),
    };
    return d;
  }
}
