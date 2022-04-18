#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{
  const TurkChecker TurkChecker::TheTurkChecker{};

  const TurkCheckersPiece TurkCheckersPiece::TurkCheckersPieceW{ TurkChecker::TheTurkChecker, PieceColor::White, &CheckersPiece::CheckersQueenW, IDB_WPL };
  const TurkCheckersPiece TurkCheckersPiece::TurkCheckersPieceB{ TurkChecker::TheTurkChecker, PieceColor::Black, &CheckersPiece::CheckersQueenB, IDB_BPL };

  void TurkChecker::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    const TurkCheckersBoard& board_ = dynamic_cast<const TurkCheckersBoard&>(p);
    //const int dy = board_.GetPiece(l).IsColor(PieceColor::White) ? -1 : 1;
    //board_.AddIfLegal(moves, l, l + Offset(1, dy));                          // check for slide moves
    //board_.AddIfLegal(moves, l, l + Offset(0, dy));
    //board_.AddIfLegal(moves, l, l + Offset(-1, dy));
    //board_.AddIfLegalJump(moves, false, Actions{}, board_.GetPiece(l), l);        // check for jump moves
  }

  void TurkCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 1; j < sizeY_ / 4 + 1; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    SetPiece(Location(BoardPartID::Main, i, j             ), TurkCheckersPiece::TurkCheckersPieceB);
    //    SetPiece(Location(BoardPartID::Main, i, sizeY_ - 1 - j), TurkCheckersPiece::TurkCheckersPieceW);
    //  }
    //}
  }

// p is the piece that is moving, but it doesn't necessarily come from fr - it could be a continuation jump
  bool TurkCheckersBoard::AddIfLegalJump(Moves& m, bool longjumps, const Actions& a, const Piece& p, const Location& fr) const noexcept
  {
    bool any{ false };                                                    // were any more jumps possible?

    //for (const auto& d : Offset::QDirection)                              // jumps are allowed in any direction, including backwards
    //{
    //  for (int z1 = 1; longjumps || z1 == 1; z1++)                        // for longjumps, allow any z, otherwise only 1 step
    //  {
    //    // look for an opponent's piece to jump over
    //    const Location l1{ fr + d * z1 };                                 // location to jump over
    //    if (!IsValid(l1)) break;                                          // tile is not existing, can't jump over it - or any further
    //    if (IsFree(l1)) continue;                                         // tile is not occupied, keep looking further (it's not a jump yet)
    //    if (IsMyPiece(l1, p.GetColor())) break;                           // can't jump own pieces, and not any further either
    //    
    //    // now we look for free spaces to land on
    //    for (int z2 = 1; longjumps || z2 == 1; z2++)                      // for longjumps, allow any z, otherwise only 1 step
    //    {
    //      const Location l2{ l1 + d * z2 };                               // location to land on
    //      if (!IsValid(l2)) break;                                        // tile is not existing, can't jump to it - or any further
    //      if (!IsFree(l2)) break;                                         // tile is occupied, can't jump there or any further

    //      // check if the same jump was done before (can't jump back and forth or in circles, and can't jump the same opponent piece twice)
    //      if (a.IsRepeat(l1, d)) break;                                   // don't allow the same piece to be jumped again
    //      if (a.IsRepeat(l1, d.Rotate(true))) break;                      // don't allow the same piece to be jumped again orthogonally either

    //      // a legal jump was found !
    //      any = true;

    //      // add the jump to the StepSimple list
    //      Actions a0{ a };
    //      const Piece& p1 = GetPiece(l1);
    //      assert(!p1.IsBlank());
    //      a0.push_back(std::make_shared<ActionLift>(fr, p));              // pick piece up
    //      a0.push_back(std::make_shared<ActionJump>(l1, p1));             // jumped over piece
    //      a0.push_back(std::make_shared<ActionLift>(l1, p1));             // pick jumped piece up
    //      a0.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(p1.GetColor()), p1)); // place it in taken
    //      a0.push_back(std::make_shared<ActionDrop>(l2, p));              // place jumping piece it on target
    //      //a0.Log();
    //      if (!AddIfLegalJump(m, longjumps, a0, p, l2))                   // collect potential further jumps
    //      {
    //        if (CanPromote(l2, p))                                       // could piece be promoted here?
    //        {
    //          a0.pop_back();                                              // remove unpromoted placement
    //          a0.push_back(std::make_shared<ActionDrop>(l2, p.Promote(true)));  // place promoted piece on target
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

}
