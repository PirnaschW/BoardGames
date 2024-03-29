#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{

  void CheckersBoard::SetStartingBoard() noexcept
  {
    const PieceIndex pWC = PMap[CheckersPiece::CheckersPieceW];
    const PieceIndex pBC = PMap[CheckersPiece::CheckersPieceB];

    for (Coordinate y = 0; y < stage_.GetSizeY() / 2 - 1; ++y)
    {
      for (Coordinate x = 0; x < stage_.GetSizeX(); ++x)
      {
        if ((x + y) % 2)
          stage_.SetPieceIndex(pBC, x, y);
        else
          stage_.SetPieceIndex(pWC, x, stage_.AbsoluteY(- 1 - y));
      }
    }
  }

  void CheckersBoard::SetDefaultStock() noexcept   // default: fill stock with standard checkers pieces
  {
    int i = 0;
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(CheckersPiece::CheckersPieceW);
    append(CheckersPiece::CheckersPieceB);
    append(CheckersPiece::CheckersKingW);
    append(CheckersPiece::CheckersKingB);
  }
  void CheckersBoard::SetQueensStock() noexcept
  {
    int i = 0;
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(CheckersPiece::CheckersPieceW);
    append(CheckersPiece::CheckersPieceB);
    append(CheckersPiece::CheckersQueenW);
    append(CheckersPiece::CheckersQueenB);
  }

  bool CheckersBoard::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    if (fr.b_ != BoardPartID::Stage || to.b_ != BoardPartID::Stage) return false;  // only moves from and to the main board are legal

    if (!stage_.IsValid(to.x_, to.y_)) return false;

    PieceIndex pfI = GetPieceIndex(fr.x_, fr.y_, fr.b_);
    assert(pfI != PMap[Piece::NoPiece]);
    assert(pfI != PMap[Piece::NoTile]);

    PieceIndex ptI = GetPieceIndex(to.x_, to.y_, to.b_);
    if (ptI != PMap[Piece::NoPiece]) return false;  // can only move onto empty fields

    const PieceIndex ptI2 = CanPromote(to, pfI) ? PMap[PMap[pfI].Promote(true)] : pfI;

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pfI));     // pick piece up
    a.push_back(std::make_shared<ActionDrop>(to, ptI2));    // place it on target
    m.push_back(std::make_shared<Move>(a));                 // add move to move list
    return true;
  }


  bool CheckersBoard::AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, PieceIndex pI, const Location& fr) const noexcept
  {
    //const Location l0{ fr };
    //const Piece& p0 = p == Piece::NoTile ? GetPieceIndex(fr) : p;
    //assert(p0 != Piece::NoTile);

    bool any{ false };                                                    // were any more jumps possible?

    //for (const auto& d : Offset::BDirection)                              // jumps are allowed in any diagonal direction, including backwards
    //{
    //  for (int z1 = 1; longjumps || z1 == 1; z1++)                        // for longjumps, allow any z, otherwise only 1 step
    //  {
    //    // look for an opponent's piece to jump over
    //    const Location l1{ fr + d * z1 };                                 // location to jump over

    //                                                                      // check the jumped-over tile                                       
    //    const Piece& p1 = GetPieceIndex(l1);                                   // what is on the tile to jump over?
    //    if (p1 == Piece::NoTile) break;                                   // tile is not existing, can't jump over it - or any further
    //    if (p1.IsBlank()) continue;                                       // tile is not occupied, keep looking further (it's not a jump yet)

    //    const PieceColor& c1 = p1.GetColor();                                 // color of jumped-over piece
    //    if (p0.IsColor(c1)) break;                                        // can't jump own pieces, and not any further either

    //    // now we look for free spaces to land on
    //    for (int z2 = 1; longjumps || z2 == 1; z2++)                      // for longjumps, allow any z, otherwise only 1 step
    //    {
    //      const Location l2{ l1 + d * z2 };                               // location to land on

    //      // check the land-on tile                                       
    //      const Piece& p2 = GetPieceIndex(l2);                                 // what is on the jump-to tile
    //      if (p2 == Piece::NoTile) break;                                 // tile is not existing, can't jump there or any further
    //      if (!p2.IsBlank()) break;                                       // tile is occupied, can't jump there or any further

    //      // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
    //      if (a.IsRepeat(l1, d)) break;                                    // don't allow the same piece to be jumped again

    //      // a legal jump was found !
    //      any = true;

    //      // add the jump to the StepSimple list
    //      Actions a0{ a };
    //      a0.push_back(std::make_shared<ActionLift>(fr, p0));             // pick piece up
    //      a0.push_back(std::make_shared<ActionJump>(l1, p1));             // jumped over piece
    //      a0.push_back(std::make_shared<ActionLift>(l1, p1));             // pick jumped piece up
    //      a0.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(p1.GetColor()), p1)); // place it in taken
    //      a0.push_back(std::make_shared<ActionDrop>(l2, p0));             // place jumping piece it on target
    //      if (!AddIfLegalJump(m, longjumps, a0, p0, l2))                  // collect potential further jumps
    //      {
    //        if (CanPromote(l2, p0))                                       // could piece be promoted here?
    //        {
    //          a0.pop_back();                                              // remove unpromoted placement
    //          a0.push_back(std::make_shared<ActionDrop>(l2, p0.Promote(true)));  // place promoted piece on target
    //        }
    //        m.push_back(std::make_shared<Move>(a0));                      // add the action list as a move
    //      }
    //      // and keep trying for longer jumps
    //    }
    //    break;
    //  }
    //}
    //// if any longer jumps were found in deeper recursions, don't save (shorter) moves in the caller; longest jump is mandatory 
    return any;
  }

  //void CheckersBoard::EvaluateStatically() const noexcept
  //{
  //  Board::EvaluateStatically();
  //  // ...
  //}

  //void CheckersBoard::GetAllMoves() const noexcept                       // collect all moves for all pieces
  //{
  //  Board::GetAllMoves();
  //  JumpsOnly(movesW_);          // if there are any jumps, remove all non-jumps - jumping is mandatory
  //  JumpsOnly(movesB_);          // if there are any jumps, remove all non-jumps - jumping is mandatory
  //}

  bool CheckersBoard::CanPromote(const Location& l, const PieceIndex p) const noexcept
  {
    return (PMap.IsColor(p,PieceColor::White) && l.y_ == 0) || (PMap.IsColor(p,PieceColor::Black) && l.y_ == stage_.GetSizeY() -1);
  }

  const BoardPartDimensions CheckersGame::GetDimensions(const VariantChosen& v) noexcept
  {
    const Coordinate stackX = (v.c == Parachute ? 4 : 3);
    const BoardPartDimensions d{
       BoardPartDimension(v.x, v.y, LayoutType::Alternating, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(stackX, 2, LayoutType::Light, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (v.y - 2), FieldSizeX, FieldSizeY),
       BoardPartDimension(2 * v.x, 2, LayoutType::Small, FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
