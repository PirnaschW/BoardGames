#include "stdafx.h"

#include "LoAResource.h"
#include "LoAGame.h"

namespace LoA
{
  inline const LoAPeg LoAPeg::ThePeg{};
  inline const LoAPiece LoAPiece::LoAPieceW{ &LoAPeg::ThePeg, &Color::White, IDB_WCL, IDB_WCD, IDB_WCS };
  inline const LoAPiece LoAPiece::LoAPieceB{ &LoAPeg::ThePeg, &Color::Black, IDB_BCL, IDB_BCD, IDB_BCS };

  std::vector<const Piece*> LoAPeg::CollectAlong(const MainPosition& pos, Location l, const Offset& o) const
  {
    std::vector<const Piece*> along{};
    const Piece* p{};
    while ((p = pos.GetPiece(l = l + o)) != nullptr) along.push_back(p);
    return along;
  }

  void LoAPeg::CollectMoves(const MainPosition& pos, const Location& l, Moves& moves, int dx, int dy) const
  {
    unsigned int s{ pos.GetPiece(l)->IsBlank() ? 0U : 1U };
    std::vector<const Piece*> a1 = CollectAlong(pos, l, Offset(dx, dy));
    std::vector<const Piece*> a2 = CollectAlong(pos, l, Offset(-dx, -dy));

    for (auto& p : a1) if (!p->IsBlank()) s++;
    for (auto& p : a2) if (!p->IsBlank()) s++;

    if (a1.size() >= s) // will a move even fit on the board?
    {
      bool free{ true };
      for (unsigned int i = 0; free && i < s; i++)
      {
        if (i == s - 1) // last = target place needs to be empty or opponent's
        {
          if (!a1[i]->IsBlank() && a1[i]->IsColor(pos.OnTurn())) free = false;
        }
        else  // intermediate places need to be empty or own
        {
          if (!a1[i]->IsBlank() && !a1[i]->IsColor(pos.OnTurn())) free = false;
        }
      }
      if (free) pos.AddIfLegal(moves, l, l + Offset(dx * s, dy*s));
    }

    if (a2.size() >= s) // will a move even fit on the board?
    {
      bool free{ true };
      for (unsigned int i = 0; free && i < s; i++)
      {
        if (i == s - 1) // last = target place needs to be empty or opponent's
        {
          if (!a2[i]->IsBlank() && a2[i]->IsColor(pos.OnTurn())) free = false;
        }
        else  // intermediate places need to be empty or own
        {
          if (!a2[i]->IsBlank() && !a2[i]->IsColor(pos.OnTurn())) free = false;
        }
      }
      if (free) pos.AddIfLegal(moves, l, l + Offset(-dx * s, -dy * s));
    }
  }

  void LoAPeg::CollectMoves(const MainPosition& pos, const Location& l, Moves& moves) const
  {
    CollectMoves(pos, l, moves, 1, 0); // check horizontal moves
    CollectMoves(pos, l, moves, 0, 1); // check vertical moves
    CollectMoves(pos, l, moves, 1, -1); // check diagonal '/' moves
    CollectMoves(pos, l, moves, 1, 1); // check diagonal '\' moves
  }

  
  LoAPosition::LoAPosition(const PieceMapP& p, const Dimensions& d) noexcept : MainPosition(p, d)
  {
    for (Coordinate i = 0; i < d[0].xCount; i++)
      for (Coordinate j = 0; j < d[0].yCount; j++)
      {
        if (((i == 0) || (i == d[0].xCount - 1)) && (j != 0) && (j != d[0].yCount - 1))  // left or right border, but not top or bottom corner
        {
          SetPiece(Location(BoardPart::Main, i, j), &LoAPiece::LoAPieceW);
        }
        else if (((j == 0) || (j == d[0].yCount - 1)) && (i != 0) && (i != d[0].xCount - 1))  // top or bottom border, but not left or right corner
        {
          SetPiece(Location(BoardPart::Main, i, j), &LoAPiece::LoAPieceB);
        }
        else
          SetPiece(Location(BoardPart::Main, i, j), &Piece::NoPiece);
      }
  }

  const Piece* LoAPosition::SetPiece(const Location& l, const Piece* p) noexcept
  {
    try
    {
      for (auto it = llw.begin(); it != llw.end(); ++it) if (it->l == l) { llw.erase(it); break; }
      for (auto it = llb.begin(); it != llb.end(); ++it) if (it->l == l) { llb.erase(it); break; }

      if (p == &LoAPiece::LoAPieceW) llw.push_back(l);
      else if (p == &LoAPiece::LoAPieceB) llb.push_back(l);
    }
    catch (...) {};
    return MainPosition::SetPiece(l, p);
  }

  bool LoAPosition::AddIfLegal(Moves& m, const Location fr, const Location to) const
  {
    const Piece* p = GetPiece(to);
    if (p == nullptr) return false;  // out of board
    if (p->IsColor(OnTurn())) return false;  // own piece
    if (p->IsBlank())
    {
      m.push_back(std::make_shared<SimpleMove>(std::make_shared<StepSimple>(Field{ fr,GetPiece(fr) }, Field{ to,GetPiece(fr) })));
    }
    else
    {
      m.push_back(std::make_shared<SimpleMove>(std::make_shared<StepSimple>( Field{ fr,GetPiece(fr) },Field{ to,GetPiece(fr) },StepSimple::StepType::Take/*,Fields{Field{ to,GetPiece(to) }}*/ )));
    }
    return false;
  }

  bool LoAPosition::IsConnected(bool t) const noexcept
  {
    const std::list<Peg>& lp = (t ^ (OnTurn() == &Color::White) ? llb : llw);
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
          const int dx = p.l._x - q.l._x;
          const int dy = p.l._y - q.l._y;
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

  void LoAPosition::EvaluateStatically(void)
  {
    if (IsConnected(false))
    {
      value = PositionValue::PValueType::Lost;  // opponent is connected -> loss
      return;
    }
    if (IsConnected(true))
    {
      value = PositionValue::PValueType::Won;   // player is connected -> win
      return;
    }

    GetAllMoves();                                                        // fill the move lists
    if (onTurn == &Color::White && movelistW.empty()) value = PositionValue::PValueType::Lost;        // if no more moves, game over
    else if (onTurn == &Color::Black && movelistB.empty()) value = PositionValue::PValueType::Won;
    else
    {
      value = 0;
      for (Coordinate i = 0; i < sizeX; i++)
      {
        for (Coordinate j = 0; j < sizeY; j++)
        {
          const Piece* p = GetPiece(Location{BoardPart::Main, i,j });
          if (p->IsColor(&Color::NoColor)) continue;
          int d{ 0 };
          for (Coordinate z = 0; z < (sizeX - 1) / 2; z++)
          {
            if (i > z && i < sizeX - 1 - z) d++;
            if (j > z && j < sizeY - 1 - z) d++;
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
          value += (p->IsColor(&Color::White) ? v : -v);
        }
      }
    }
  }


  const VariantList& LoAGame::GetVariants(void) noexcept
  {
    static VariantList v{ { Variant{ 8, 8, nullptr, 3, 20 } } };
    return v;
  }

  const PieceMapP& LoAGame::GetPieces(void) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Add(&LoAPiece::LoAPieceW);
    p->Add(&LoAPiece::LoAPieceB);
    return p;
  }

  const Dimensions& LoAGame::GetDimensions(Coordinate x, Coordinate y) noexcept
  {
    static Dimensions d{
       Dimension(x, y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(3, 1, BoardStartX + FieldSizeX * (x + 1), BoardStartY + FieldSizeY / 2 + FieldSizeY * (y - 2), FieldSizeX, FieldSizeY),
       Dimension(2 * x, 2, FieldSizeX * (x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * y - FieldSizeSY * 4),
    };
    return d;
  }

}
