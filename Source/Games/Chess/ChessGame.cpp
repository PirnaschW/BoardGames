#include "Games.h"

#include "ChessGame.h"

namespace Chess
{

  void ChessBoard::GetAllMoves() const noexcept
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    movesW_.reserve(20);
    movesB_.reserve(20);
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Location& l{ Location(BoardPartID::Stage, i, j) };
        const Piece& p = GetPieceIndex(l);
        if (!p.IsBlank()) p.CollectMoves(*this, Location(BoardPartID::Stage, i, j), p.IsColor(PieceColor::White) ? movesW_ : movesB_);
        else if (HasRule(DropTakenPieces))
        { // loop through all taken pieces and try to drop them here
          for (Coordinate i1 = 0; i1 < taken_.GetSizeX(); i1++)
          {
            for (Coordinate j1 = 0; j1 < taken_.GetSizeY(); j1++)
            {
              const Location& l1{ Location(BoardPartID::Taken, i1, j1) };
              const Piece& p1 = GetPieceIndex(l1);
              if (!p1.IsBlank()) AddIfLegal(p1.IsColor(PieceColor::White) ? movesW_ : movesB_, l1, l);
            }
          }
        }
      }
    }
  }

  bool ChessBoard::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPieceIndex(fr);
    assert(pf != Piece::NoTile);                                                    // start field must exist
    assert(!pf.IsBlank());                                                          // start field must be a piece

    const Piece& pt = GetPieceIndex(to);
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
      a.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(pf.GetColor()), pt));  // place it in Taken
      a.push_back(std::make_shared<ActionDrop>(to, pf));                            // and place it on target
      m.push_back(std::make_shared<Move>(a));                                       // add move to move list
    }

    return false;                                                                   // don't keep trying this direction
  };

  void ChessBoard::SetStartingBoard() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    SetPieceIndex({ BoardPartID::Stage,          0U,               0U }, ChessPiece::BR);
    SetPieceIndex({ BoardPartID::Stage,          1U,               0U }, ChessPiece::BN);
    SetPieceIndex({ BoardPartID::Stage,          2U,               0U }, ChessPiece::BB);
    SetPieceIndex({ BoardPartID::Stage, sizeX_ - 3U,               0U }, ChessPiece::BB);
    SetPieceIndex({ BoardPartID::Stage, sizeX_ - 2U,               0U }, ChessPiece::BN);
    SetPieceIndex({ BoardPartID::Stage, sizeX_ - 1U,               0U }, ChessPiece::BR);
    SetPieceIndex({ BoardPartID::Stage, sizeX_ / 2U - 1U,          0U }, ChessPiece::BQ);  // set Queen and King last!
    SetPieceIndex({ BoardPartID::Stage, sizeX_ / 2U     ,          0U }, ChessPiece::BK);  // this is useful for smaller boards

    SetPieceIndex({ BoardPartID::Stage,          0U,      sizeY_ - 1U }, ChessPiece::WR);
    SetPieceIndex({ BoardPartID::Stage,          1U,      sizeY_ - 1U }, ChessPiece::WN);
    SetPieceIndex({ BoardPartID::Stage,          2U,      sizeY_ - 1U }, ChessPiece::WB);
    SetPieceIndex({ BoardPartID::Stage, sizeX_ - 3U,      sizeY_ - 1U }, ChessPiece::WB);
    SetPieceIndex({ BoardPartID::Stage, sizeX_ - 2U,      sizeY_ - 1U }, ChessPiece::WN);
    SetPieceIndex({ BoardPartID::Stage, sizeX_ - 1U,      sizeY_ - 1U }, ChessPiece::WR);
    SetPieceIndex({ BoardPartID::Stage, sizeX_ / 2U - 1U, sizeY_ - 1U }, ChessPiece::WQ);  // set Queen and King last!
    SetPieceIndex({ BoardPartID::Stage, sizeX_ / 2U     , sizeY_ - 1U }, ChessPiece::WK);  // this is useful for smaller boards
  }

  void ChessBoard::SetPawns(Coordinate row, const PieceColor& c) noexcept
  {
    const Piece& p{ c == PieceColor::White ? ChessPiece::WP : ChessPiece::BP };
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      const Location& l{ BoardPartID::Stage, i, row };
      SetPieceIndex(l, p);
    }
  }

  void ChessBoard::SetPiecesPSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept
  {
    SetPieceIndex({ BoardPartID::Stage,               x,               y }, b);
    SetPieceIndex({ BoardPartID::Stage, sizeX_ - 1U - x, sizeY_ - 1U - y }, w);
  }
  void ChessBoard::SetPiecesHSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept
  {
    SetPieceIndex({ BoardPartID::Stage,               x,               y }, b);
    SetPieceIndex({ BoardPartID::Stage,               x, sizeY_ - 1U - y }, w);
  }

  Side ChessBoard::PickRandomPiece() const noexcept
  {
    static const std::array<const Kind*, 6> Kinds{
      &Pawn::ThePawn,
      &Knight::TheKnight,
      &Bishop::TheBishop,
      &Rook::TheRook,
      &Queen::TheQueen,
      &King::TheKing,
    };

    if (movesW_.size() == 0) return 0;

    std::array<Moves, 6> moves_{};

    auto Find = [&](const Piece& p) -> int { for (int i = 0; i < 6; i++)  if (p.IsKind(*Kinds[i])) return i; return -1; };

    for (auto m : movesW_)
    {
      int z = Find(m->GetActions()[0]->GetPieceIndex());
      assert(z >= 0);
      moves_[z].push_back(m);
    }

    int z{};
    do z = Math::D6() - 1; while (moves_[z].size() == 0);  // find a random Kind to move

    movesW_ = moves_[z];
    return z;
  }


  void ChessBoard::EvaluateStatically() const noexcept { Board::EvaluateStatically(); }

  const BoardPartDimensions ChessGame::GetDimensions(const VariantChosen& v) noexcept
  {
    BoardPartDimensions d{
       BoardPartDimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(11, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (v.y + 1), FieldSizeX, FieldSizeY),
       BoardPartDimension(v.x * v.y / 2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
