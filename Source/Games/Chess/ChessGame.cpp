#include "Games.h"

#include "ChessGame.h"

namespace Chess
{

  void ChessBoard::GetAllMoves() const noexcept
  {
  //  assert(movesW_.empty());
  //  assert(movesB_.empty());

  //  movesW_.reserve(20);
  //  movesB_.reserve(20);
  //  for (Coordinate i = 0; i < sizeX_; i++)
  //  {
  //    for (Coordinate j = 0; j < sizeY_; j++)
  //    {
  //      const Location& l{ Location(BoardPartID::Stage, i, j) };
  //      const Piece& p = GetPieceIndex(l);
  //      if (!p.IsBlank()) p.CollectMoves(*this, Location(BoardPartID::Stage, i, j), p.IsColor(PieceColor::White) ? movesW_ : movesB_);
  //      else if (HasRule(DropTakenPieces))
  //      { // loop through all taken pieces and try to drop them here
  //        for (Coordinate i1 = 0; i1 < taken_.GetSizeX(); i1++)
  //        {
  //          for (Coordinate j1 = 0; j1 < taken_.GetSizeY(); j1++)
  //          {
  //            const Location& l1{ Location(BoardPartID::Taken, i1, j1) };
  //            const Piece& p1 = GetPieceIndex(l1);
  //            if (!p1.IsBlank()) AddIfLegal(p1.IsColor(PieceColor::White) ? movesW_ : movesB_, l1, l);
  //          }
  //        }
  //      }
  //    }
  //  }
  }

  bool ChessBoard::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    //const Piece& pf = GetPieceIndex(fr);
    //assert(pf != Piece::NoTile);                                                    // start field must exist
    //assert(!pf.IsBlank());                                                          // start field must be a piece

    //const Piece& pt = GetPieceIndex(to);
    //if (pt == Piece::NoTile) return false;                                          // out of board
    //if (pt.IsBlank())                                                               // moving onto free field
    //{
    //  if (HasRule(AllowMoves))
    //  {
    //    Actions a{};
    //    a.push_back(std::make_shared<ActionLift>(fr, pf));                          // pick piece up
    //    a.push_back(std::make_shared<ActionDrop>(to, pf));                          // and place it on target
    //    m.push_back(std::make_shared<Move>(a));                                     // add move to move list
    //  }
    //  return true;                                                                  // keep trying this direction
    //}

    //if (HasRule(AllowTakes))
    //{
    //  if (pt.GetColor() == pf.GetColor() && !HasRule(TakeOwn)) return false;        // own piece; don't keep trying this direction

    //  // valid take move, save into collection
    //  Actions a{};
    //  a.push_back(std::make_shared<ActionLift>(fr, pf));                            // pick piece up
    //  a.push_back(std::make_shared<ActionLift>(to, pt));                            // pick opponent piece up
    //  a.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(pf.GetColor()), pt));  // place it in Taken
    //  a.push_back(std::make_shared<ActionDrop>(to, pf));                            // and place it on target
    //  m.push_back(std::make_shared<Move>(a));                                       // add move to move list
    //}

    return false;                                                                   // don't keep trying this direction
  }

  void ChessBoard::SetStartingBoard() noexcept
  {
    const Coordinate sx = stage_.GetSizeX();
    const Coordinate sy = stage_.GetSizeY();

    SetPawns(1U, PieceColor::Black);
    SetPawns(stage_.GetSizeY() - 2U, PieceColor::White);

    stage_.SetPieceIndex(PMap[ChessPiece::BR],      0U,           0U);
    stage_.SetPieceIndex(PMap[ChessPiece::BN],      1U,           0U);
    stage_.SetPieceIndex(PMap[ChessPiece::BB],      2U,           0U);
    stage_.SetPieceIndex(PMap[ChessPiece::BB], sx - 3U,           0U);
    stage_.SetPieceIndex(PMap[ChessPiece::BN], sx - 2U,           0U);
    stage_.SetPieceIndex(PMap[ChessPiece::BR], sx - 1U,           0U);
    stage_.SetPieceIndex(PMap[ChessPiece::BQ], sx / 2U - 1U,      0U);  // set Queen and King last!
    stage_.SetPieceIndex(PMap[ChessPiece::BK], sx / 2U,           0U);  // this is useful for smaller boards
    stage_.SetPieceIndex(PMap[ChessPiece::WR],      0U,      sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WN],      1U,      sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WB],      2U,      sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WB], sx - 3U,      sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WN], sx - 2U,      sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WR], sx - 1U,      sy - 1U);
    stage_.SetPieceIndex(PMap[ChessPiece::WQ], sx / 2U - 1U, sy - 1U);  // set Queen and King last!
    stage_.SetPieceIndex(PMap[ChessPiece::WK], sx / 2U,      sy - 1U);  // this is useful for smaller boards
  }

  void ChessBoard::SetDefaultStock() noexcept   // default: fill stock with standard chess set
  {
    int i = 0;
    auto append = [this, &i](const Piece& p) { stock_.SetPieceIndex(PMap[p], i / 2U, i % 2U); ++i; };
    append(ChessPiece::WP);  // White Pawn
    append(ChessPiece::BP);  // Black Pawn
    append(ChessPiece::WN);  // White Knight
    append(ChessPiece::BN);  // Black Knight
    append(ChessPiece::WB);  // White Bishop
    append(ChessPiece::BB);  // Black Bishop
    append(ChessPiece::WR);  // White Rook 
    append(ChessPiece::BR);  // Black Rook
    append(ChessPiece::WQ);  // White Queen
    append(ChessPiece::BQ);  // Black Queen
    append(ChessPiece::WK);  // White King 
    append(ChessPiece::BK);  // Black King
  }

  void ChessBoard::SetPawns(Coordinate row, const PieceColor& c) noexcept
  {
    const PieceIndex pI{ PMap[c == PieceColor::White ? ChessPiece::WP : ChessPiece::BP] };
    for (Coordinate i = 0; i < stage_.GetSizeX(); i++)
    {
      stage_.SetPieceIndex(pI, i, row);
    }
  }

  void ChessBoard::SetPiecesPSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept
  {
    const Coordinate sx = stage_.GetSizeX();
    const Coordinate sy = stage_.GetSizeY();
    stage_.SetPieceIndex(PMap[b],           x,           y );
    stage_.SetPieceIndex(PMap[w], sx - 1U - x, sy - 1U - y );
  }
  void ChessBoard::SetPiecesHSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept
  {
    const Coordinate sy = stage_.GetSizeY();
    stage_.SetPieceIndex(PMap[b],x,           y);
    stage_.SetPieceIndex(PMap[w],x, sy - 1U - y);
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

    auto Find = [&](const PieceIndex pI) -> int { for (int i = 0; i < 6; i++)  if (PMap[pI].IsKind(*Kinds[i])) return i; return -1; };

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
       BoardPartDimension(v.x, v.y, LayoutType::Alternating, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(11, 2, LayoutType::Light, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (v.y + 1), FieldSizeX, FieldSizeY),
       BoardPartDimension(v.x * v.y / 2, 2, LayoutType::Small, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
