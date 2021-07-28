#include "Games.h"

#include "ChessGame.h"

namespace Chess
{

  void ChessPosition::GetAllMoves(void) const noexcept
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    movesW_.reserve(20);
    movesB_.reserve(20);
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Location& l{ Location(BoardPart::Main, i, j) };
        const Piece& p = GetPiece(l);
        if (!p.IsBlank()) p.CollectMoves(*this, Location(BoardPart::Main, i, j), p.IsColor(PieceColor::White) ? movesW_ : movesB_);
        else if (HasRule(DropTakenPieces))
        { // loop through all taken pieces and try to drop them here
          for (Coordinate i1 = 0; i1 < taken_.GetSizeX(); i1++)
          {
            for (Coordinate j1 = 0; j1 < taken_.GetSizeY(); j1++)
            {
              const Location& l1{ Location(BoardPart::Taken, i1, j1) };
              const Piece& p1 = GetPiece(l1);
              if (!p1.IsBlank()) AddIfLegal(p1.IsColor(PieceColor::White) ? movesW_ : movesB_, l1, l);
            }
          }
        }
      }
    }
  }

  bool ChessPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);
    assert(pf != Piece::NoTile);                                                    // start field must exist
    assert(!pf.IsBlank());                                                          // start field must be a piece

    const Piece& pt = GetPiece(to);
    if (pt == Piece::NoTile) return false;                                          // out of board
    if (pt.IsBlank())                                                               // moving onto free field
    {
      if (HasRule(AllowMoves))
      {
        Actions a{};
        a.push_back(std::make_shared<ActionLift>(fr, pf));                          // pick piece up
        a.push_back(std::make_shared<ActionDrop>(to, pf));                          // and place it on target
        m.push_back(std::make_shared<Move>(a));                                     // add move to move list
      }
      return true;                                                                  // keep trying this direction
    }

    if (HasRule(AllowTakes))
    {
      if (pt.GetColor() == pf.GetColor() && !HasRule(TakeOwn)) return false;        // own piece; don't keep trying this direction

      // valid take move, save into collection
      Actions a{};
      a.push_back(std::make_shared<ActionLift>(fr, pf));                            // pick piece up
      a.push_back(std::make_shared<ActionLift>(to, pt));                            // pick opponent piece up
      a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pf.GetColor()), pt));  // place it in Taken
      a.push_back(std::make_shared<ActionDrop>(to, pf));                            // and place it on target
      m.push_back(std::make_shared<Move>(a));                                       // add move to move list
    }

    return false;                                                                   // don't keep trying this direction
  };

  void ChessPosition::SetStartingPosition() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    SetPiece({ BoardPart::Main,               0U,          0U }, ChessPiece::BR);
    SetPiece({ BoardPart::Main,               1U,          0U }, ChessPiece::BN);
    SetPiece({ BoardPart::Main,               2U,          0U }, ChessPiece::BB);
    SetPiece({ BoardPart::Main, sizeX_ - 3U,          0U }, ChessPiece::BB);
    SetPiece({ BoardPart::Main, sizeX_ - 2U,          0U }, ChessPiece::BN);
    SetPiece({ BoardPart::Main, sizeX_ - 1U,          0U }, ChessPiece::BR);
    SetPiece({ BoardPart::Main, sizeX_ / 2U - 1U,          0U }, ChessPiece::BQ);  // set Queen and King last!
    SetPiece({ BoardPart::Main, sizeX_ / 2U     ,          0U }, ChessPiece::BK);  // this is useful for smaller boards

    SetPiece({ BoardPart::Main,               0U, sizeY_ - 1U }, ChessPiece::WR);
    SetPiece({ BoardPart::Main,               1U, sizeY_ - 1U }, ChessPiece::WN);
    SetPiece({ BoardPart::Main,               2U, sizeY_ - 1U }, ChessPiece::WB);
    SetPiece({ BoardPart::Main, sizeX_ - 3U, sizeY_ - 1U }, ChessPiece::WB);
    SetPiece({ BoardPart::Main, sizeX_ - 2U, sizeY_ - 1U }, ChessPiece::WN);
    SetPiece({ BoardPart::Main, sizeX_ - 1U, sizeY_ - 1U }, ChessPiece::WR);
    SetPiece({ BoardPart::Main, sizeX_ / 2U - 1U, sizeY_ - 1U }, ChessPiece::WQ);  // set Queen and King last!
    SetPiece({ BoardPart::Main, sizeX_ / 2U     , sizeY_ - 1U }, ChessPiece::WK);  // this is useful for smaller boards
  }

  void ChessPosition::SetPawns(Coordinate row, const PieceColor& c) noexcept
  {
    const Piece& p{ c == PieceColor::White ? ChessPiece::WP : ChessPiece::BP };
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      const Location& l{ BoardPart::Main, i, row };
      SetPiece(l, p);
    }
  }

  void ChessPosition::SetPiecesPSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept
  {
    SetPiece({ BoardPart::Main,               x,               y }, b);
    SetPiece({ BoardPart::Main, sizeX_ - 1U - x, sizeY_ - 1U - y }, w);
  }
  void ChessPosition::SetPiecesHSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept
  {
    SetPiece({ BoardPart::Main,               x,               y }, b);
    SetPiece({ BoardPart::Main,               x, sizeY_ - 1U - y }, w);
  }


  PositionValue ChessPosition::EvaluateStatically(void) const noexcept { return MainPosition::EvaluateStatically(); }

  const Dimensions ChessGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(11, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (v.y + 1), FieldSizeX, FieldSizeY),
       Dimension(v.x * v.y / 2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
