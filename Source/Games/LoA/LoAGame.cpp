#include "Games.h"

#include "LoAResource.h"
#include "LoAGame.h"

namespace LoA
{
  const LoAPeg LoAPeg::ThePeg{};
  const LoAPiece LoAPiece::LoAPieceW{ LoAPeg::ThePeg, PieceColor::White, IDB_W0L };
  const LoAPiece LoAPiece::LoAPieceB{ LoAPeg::ThePeg, PieceColor::Black, IDB_B0L };

  std::vector<const Piece*> LoAPeg::CollectAlong(const Board& board_, Location l, const Offset& o) const
  {
    std::vector<const Piece*> along{};
    const Piece* p{};
    while ((p = &board_.GetPieceIndex(l = l + o)) != &Piece::NoTile) along.push_back(p);
    return along;
  }

  void LoAPeg::CollectMoves(const Board& board_, const Location& l, Moves& moves, int dx, int dy) const
  {
    unsigned int s{ board_.GetPieceIndex(l).IsBlank() ? 0U : 1U };
    std::vector<const Piece*> a1 = CollectAlong(board_, l, Offset(dx, dy));
    std::vector<const Piece*> a2 = CollectAlong(board_, l, Offset(-dx, -dy));

    for (auto& p : a1) if (!p->IsBlank()) s++;
    for (auto& p : a2) if (!p->IsBlank()) s++;

    if (a1.size() >= s) // will a move even fit on the board?
    {
      bool free{ true };
      for (unsigned int i = 0; free && i < s; i++)
      {
        if (i == s - 1) // last = target place needs to be empty or opponent's
        {
          if (!a1[i]->IsBlank() && a1[i]->IsColor(board_.OnTurn())) free = false;
        }
        else  // intermediate places need to be empty or own
        {
          if (!a1[i]->IsBlank() && !a1[i]->IsColor(board_.OnTurn())) free = false;
        }
      }
      if (free) board_.AddIfLegal(moves, l, l + Offset(dx * s, dy * s));
    }

    if (a2.size() >= s) // will a move even fit on the board?
    {
      bool free{ true };
      for (unsigned int i = 0; free && i < s; i++)
      {
        if (i == s - 1) // last = target place needs to be empty or opponent's
        {
          if (!a2[i]->IsBlank() && a2[i]->IsColor(board_.OnTurn())) free = false;
        }
        else  // intermediate places need to be empty or own
        {
          if (!a2[i]->IsBlank() && !a2[i]->IsColor(board_.OnTurn())) free = false;
        }
      }
      if (free) board_.AddIfLegal(moves, l, l + Offset(-dx * s, -dy * s));
    }
  }

  void LoAPeg::CollectMoves(const Board& board_, const Location& l, Moves& moves) const noexcept
  {
    CollectMoves(board_, l, moves, 1, 0); // check horizontal moves
    CollectMoves(board_, l, moves, 0, 1); // check vertical moves
    CollectMoves(board_, l, moves, 1, -1); // check diagonal '/' moves
    CollectMoves(board_, l, moves, 1, 1); // check diagonal '\' moves
  }


  void LoABoard::SetStartingBoard() noexcept
  {
    for (Coordinate i = 0; i < sizeX_; i++)
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        if (((i == 0) || (i == sizeX_ - 1)) && (j != 0) && (j != sizeY_ - 1))  // left or right border, but not top or bottom corner
        {
          SetPieceIndex(Location(BoardPartID::Stage, i, j), LoAPiece::LoAPieceW);
        }
        else if (((j == 0) || (j == sizeY_ - 1)) && (i != 0) && (i != sizeX_ - 1))  // top or bottom border, but not left or right corner
        {
          SetPieceIndex(Location(BoardPartID::Stage, i, j), LoAPiece::LoAPieceB);
        }
        else
          SetPieceIndex(Location(BoardPartID::Stage, i, j), Piece::NoPiece);
      }
  }

  const Piece& LoABoard::SetPieceIndex(const Location& l, const Piece& p) noexcept
  {
    try
    {
      for (auto it = llw.begin(); it != llw.end(); ++it) if (it->l == l) { llw.erase(it); break; }
      for (auto it = llb.begin(); it != llb.end(); ++it) if (it->l == l) { llb.erase(it); break; }

      if (p == LoAPiece::LoAPieceW) llw.push_back(l);
      else if (p == LoAPiece::LoAPieceB) llb.push_back(l);
    }
    catch (...) {};
    return Board::SetPieceIndex(l, p);
  }

  bool LoABoard::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPieceIndex(fr);
    const Piece& pt = GetPieceIndex(to);
    if (pt == Piece::NoTile) return false;  // out of board
    if (pt.IsColor(OnTurn())) return false;  // own piece

    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                    // pick piece up
    if (!pt.IsBlank())
    {
      a.push_back(std::make_shared<ActionLift>(to, pt));                  // pick piece up
      a.push_back(std::make_shared<ActionDrop>(GetNextFreeTakenLocation(pt.GetColor()), pt));  // and place it on target
    }
    a.push_back(std::make_shared<ActionDrop>(to, pf));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                               // add move to move list
    return false;
  }

  bool LoABoard::IsConnected(bool t) const noexcept
  {
    const std::list<Peg>& lp = (t ^ (OnTurn() == PieceColor::White) ? llb : llw);
    if (lp.empty()) return true;

    for (auto& p : lp) p.connected = p.checked = false; // reset the flags
    lp.front().connected = true;                        // seed the first one

    bool grown{ false };
    do  // try to spread the connection
    {
      grown = false;
      for (auto& p : lp)
      {
        if (!p.connected) continue;
        if (p.checked) continue;
        for (auto& q : lp)
        {
          if (q.connected) continue;
          const int dx = p.l.x_ - q.l.x_;
          const int dy = p.l.y_ - q.l.y_;
          if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1)
          {
            q.connected = true;
            grown = true;
          }
        }
        p.checked = true;
      }

      // check if all connected
      bool connected{ true };
      for (auto& p : lp)
      {
        if (!p.connected)
        {
          connected = false;
          break;
        }
      }
      if (connected) return true;
    } while (grown);
    return false;
  }

  PositionValue LoABoard::EvaluateStatically() const noexcept
  {
    if (IsConnected(false)) return PositionValue::PValueType::Lost;  // opponent is connected -> loss
    if (IsConnected(true))  return PositionValue::PValueType::Won;   // player is connected -> win

    GetAllMoves();                                                        // fill the move lists
    if (onTurn_ == &PieceColor::White && movesW_.empty()) return PositionValue::PValueType::Lost;        // if no more moves, game over
    if (onTurn_ == &PieceColor::Black && movesB_.empty()) return PositionValue::PValueType::Won;
    PositionValue value{ 0 };
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      for (Coordinate j = 0; j < sizeY_; j++)
      {
        const Piece& p = GetPieceIndex(Location{ BoardPartID::Stage, i,j });
        if (p.IsColor(PieceColor::NoColor)) continue;
        int d{ 0 };
        for (Coordinate z = 0; z < (sizeX_ - 1) / 2; z++)
        {
          if (i > z&& i < sizeX_ - 1 - z) d++;
          if (j > z&& j < sizeY_ - 1 - z) d++;
        }
        int v{ 0 };
        switch (d)
        {
          case 0: v = -5; break;
          case 1: v = -2; break;
          case 2: v = -1; break;
          case 3: v = 0; break;
          case 4: v = 1; break;
          case 5: v = 2; break;
          case 6: v = 4; break;
          default: v = 6; break;
        }
        value += (p.IsColor(PieceColor::White) ? v : -v);
      }
    }
    return value;
  }


  const VariantList& LoAGame::GetVariants() noexcept
  {
    static VariantList v{
      { Variant{ nullptr, 'A', 8, 8, 3, 20 } }
    };
    return v;
  }

  const PieceMapP& LoAGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(LoAPiece::LoAPieceW);
    p->Add(LoAPiece::LoAPieceB);
    return p;
  }

  const BoardPartDimensions LoAGame::GetDimensions(const VariantChosen& v) noexcept
  {
    BoardPartDimensions d{
       BoardPartDimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       BoardPartDimension(3, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (v.y - 2), FieldSizeX, FieldSizeY),
       BoardPartDimension(2 * v.x, 2, FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
