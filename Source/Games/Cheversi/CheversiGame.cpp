#include "Games.h"

#include "CheversiGame.h"
#include "../Chess/ChessGame.h"

namespace Cheversi
{
  CheversiPosition::CheversiPosition(const VariantChosen& v, const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(v, p, d)
  {
    SetPiece(Location{ BoardPart::Taken, 0, 1 }, Chess::ChessPiece::WK);
    SetPiece(Location{ BoardPart::Taken, 1, 1 }, Chess::ChessPiece::WQ);
    SetPiece(Location{ BoardPart::Taken, 2, 1 }, Chess::ChessPiece::WR);
    SetPiece(Location{ BoardPart::Taken, 3, 1 }, Chess::ChessPiece::WR);
    SetPiece(Location{ BoardPart::Taken, 4, 1 }, Chess::ChessPiece::WB);
    SetPiece(Location{ BoardPart::Taken, 5, 1 }, Chess::ChessPiece::WB);
    SetPiece(Location{ BoardPart::Taken, 6, 1 }, Chess::ChessPiece::WN);
    SetPiece(Location{ BoardPart::Taken, 7, 1 }, Chess::ChessPiece::WN);
    SetPiece(Location{ BoardPart::Taken, 0, 0 }, Chess::ChessPiece::BK);
    SetPiece(Location{ BoardPart::Taken, 1, 0 }, Chess::ChessPiece::BQ);
    SetPiece(Location{ BoardPart::Taken, 2, 0 }, Chess::ChessPiece::BR);
    SetPiece(Location{ BoardPart::Taken, 3, 0 }, Chess::ChessPiece::BR);
    SetPiece(Location{ BoardPart::Taken, 4, 0 }, Chess::ChessPiece::BB);
    SetPiece(Location{ BoardPart::Taken, 5, 0 }, Chess::ChessPiece::BB);
    SetPiece(Location{ BoardPart::Taken, 6, 0 }, Chess::ChessPiece::BN);
    SetPiece(Location{ BoardPart::Taken, 7, 0 }, Chess::ChessPiece::BN);
  }

  void CheversiPosition::GetAllMoves(void) const noexcept // collect all moves for all pieces
  {
    assert(movesW_.empty());
    assert(movesB_.empty());

    movesW_.reserve(20);
    movesB_.reserve(20);

    if (sequence_.size() == 0)  // first move?
    {
    }

    for (Coordinate i = 0; i < sizeX_; i++)
    {
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Piece& p = GetPiece(Location(BoardPart::Main, i, j));
        if (!p.IsKind(noKind::NoKind))  // skip blank fields as well as nonexisting tiles
        {
          p.CollectMoves(*this, Location(BoardPart::Main, i, j), p.IsColor(PieceColor::White) ? movesW_ : movesB_);
        }
      }
    }
  }


  bool CheversiPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);                                       // piece to move
    if (pf == Piece::NoTile) return false;                               // out of board
    if (pf.IsBlank()) return false;                                      // tile not occupied

    const Piece& pt = GetPiece(to);                                       // piece on target field
    if (pt == Piece::NoTile) return false;                               // out of board

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                    // pick piece up
    if (!pt.IsBlank())
    {
      a.push_back(std::make_shared<ActionLift>(to, pt));                    // pick opponent piece up
      a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pf.GetColor()), pt));                   // place it in Taken
    }
    a.push_back(std::make_shared<ActionDrop>(to, pf));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return false;
  };

  PositionValue CheversiPosition::EvaluateStatically(void) const noexcept
  {
    return MainPosition::EvaluateStatically();
    // ...
  }


  const VariantList& CheversiGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ nullptr, 'A', 8, 8, 2, 20 } },
    };
    return v;
  }

  const PieceMapP& CheversiGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(Chess::ChessPiece::WK);
    p->Add(Chess::ChessPiece::BK);
    p->Add(Chess::ChessPiece::WQ);
    p->Add(Chess::ChessPiece::BQ);
    p->Add(Chess::ChessPiece::WR);
    p->Add(Chess::ChessPiece::BR);
    p->Add(Chess::ChessPiece::WB);
    p->Add(Chess::ChessPiece::BB);
    p->Add(Chess::ChessPiece::WN);
    p->Add(Chess::ChessPiece::BN);
    return p;
  }

  const Dimensions CheversiGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(6, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + v.y * FieldSizeY + FieldSizeY / 2, FieldSizeX, FieldSizeY),
       Dimension(8, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.x - FieldSizeSY * 4),
    };
    return d;
  }
}
