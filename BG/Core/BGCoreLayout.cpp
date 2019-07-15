#include "stdafx.h"

namespace BoardGamesCore
{

  Layout::Layout(const Dimension& d, const BoardPart b, const LayoutType lt) noexcept : dim(d), ltype(lt), tiles{ 1ULL * d.xCount*d.yCount,nullptr }
  {
    unsigned int z = 0;
    for (Coordinate i = 0; i < dim.xCount; i++)
      for (Coordinate j = 0; j < dim.yCount; j++, z++)
      {
        const TileColor* f{};
        switch (ltype)
        {
          case LayoutType::Light:       f = &TileColor::Light;                                     break;
          case LayoutType::Dark:        f = &TileColor::Dark;                                      break;
          case LayoutType::Small:       f = &TileColor::Small;                                     break;
          case LayoutType::Alternating: f = ((i + j) % 2) ? &TileColor::Dark : &TileColor::Light;  break;
        }

        const CRect r{
          (int)(dim.lEdge + dim.xDim * i + dim.xSkip * i),
          (int)(dim.tEdge + dim.yDim * j + dim.ySkip * j),
          (int)(dim.lEdge + dim.xDim * (i + 1U) + dim.xSkip * i),
          (int)(dim.tEdge + dim.yDim * (j + 1U) + dim.ySkip * j) };
        (tiles)[z] = new Tile(Location(b, i, j), r, f);
      }
  }

  void Layout::Draw(CDC* pDC, const Position* pos, _Mode mode) const
  {
    for (auto& t : tiles)
      t->Draw(pDC, pos->GetPiece(t->GetLocation()));
  }

  void Layout::DrawSelected(CDC* pDC, const Location& l) const
  {
    for (auto& t : tiles)
      if (t->GetLocation() == l)
      {
        CRect r = t->GetRect();
        r.DeflateRect(1, 1, 0, 0);
        pDC->SelectStockObject(NULL_BRUSH);
        CPen pen;
        pen.CreatePen(PS_SOLID, 2, RGB(0, 128, 255));
        pDC->SelectObject(pen);
        pDC->Rectangle(r);
      }
  }

  bool Layout::GetLocation(const CPoint& p, Location& l) const noexcept
  {
    for (auto& t : tiles)
      if (t->InRect(p)) {
        l = t->GetLocation();
        return true;
      }
    return false;
  }

  void MainLayout::Draw(CDC* pDC, const MainPosition* pos, _Mode mode) const
  {
    // frame around the board (needs to be drawn first)
    for (unsigned int z = 4; z > 0; z--)
    {
      if (z != 2)
        pDC->Rectangle((int)(dim.lEdge - z), (int)(dim.tEdge - z), (int)(dim.lEdge + dim.xCount * (dim.xDim + dim.xSkip) + z), (int)(dim.tEdge + dim.yCount * (dim.yDim + dim.ySkip) + z));
    }
    Layout::Draw(pDC, pos, mode);

    if (mode.IsSet(Mode::ShowStock) || mode.IsSet(Mode::Editing))
    {
      _stock.Draw(pDC, &(pos->_stock), mode);
    }
    _taken.Draw(pDC, &(pos->_taken), mode);
  }


  void Game::Draw(CDC* pDC) const
  {
    lay->Draw(pDC, pos, _mode);

    // markup selectable tiles
    
    if (_mode.IsSet(Mode::SelectTo))
    {
      for (auto& m : moves)
      {
        lay->DrawSelected(pDC, m->GetFrL());
        lay->DrawSelected(pDC, m->GetToL());
      }
    }

    if (_mode.IsSet(Mode::Dragging))
    {
      const CRect r(dragPoint, SIZE{32,32});  // doesn't work for all games! - some have 18x20
      dragPiece->Draw(pDC, r, &TileColor::Small);
    }

    if (true) {
      CString s;
      int h = 10;
      const char* v = static_cast<const char*>(pos->GetValue(pos->OnTurn() == &Color::White));
      s = "Depth";                pDC->TextOutW(800, h += 20, s);  s.Format(_T("%d"),  pos->GetDepth());       pDC->TextOutW(1000, h, s);
      s = "Value";                pDC->TextOutW(800, h += 20, s);  s = v;                                      pDC->TextOutW(1000, h, s);
      s = "PList size";           pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), plist->size());         pDC->TextOutW(1000, h, s);
      s = "free mem";             pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), plist->freemem);        pDC->TextOutW(1000, h, s);
      s = "sizeof(Position)";     pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(Position));      pDC->TextOutW(1000, h, s);
      s = "sizeof(MainPosition)"; pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(MainPosition));  pDC->TextOutW(1000, h, s);
      s = "sizeof(vector<Move>)"; pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(Moves));         pDC->TextOutW(1000, h, s);
      s = "sizeof(Move)";         pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(Move));          pDC->TextOutW(1000, h, s);
      s = "sizeof(Action)";       pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(Action));        pDC->TextOutW(1000, h, s);
      s = "sizeof(ActionTake)";   pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(ActionTake));    pDC->TextOutW(1000, h, s);
      s = "sizeof(ActionJump)";   pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(ActionJump));    pDC->TextOutW(1000, h, s);
      s = "sizeof(ActionPlace)";  pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(ActionPlace));   pDC->TextOutW(1000, h, s);
      //s = "sizeof(StepPlace)";    pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(StepPlace));     pDC->TextOutW(1000, h, s);
      //s = "sizeof(StepSimple)";   pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(StepSimple));    pDC->TextOutW(1000, h, s);
      //s = "sizeof(StepComplex)";  pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(StepComplex));   pDC->TextOutW(1000, h, s);
      //s = "sizeof(SimpleMove)";   pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(SimpleMove));    pDC->TextOutW(1000, h, s);
      //s = "sizeof(ComplexMove)";  pDC->TextOutW(800, h += 20, s);  s.Format(_T("%lu"), sizeof(ComplexMove));   pDC->TextOutW(1000, h, s);
    }
  }

  void Game::React(CCmdUI* pCmdUI)                                        // react to UI events (allows to set buttons greyed, etc.)
  {
    switch (pCmdUI->m_nID)
    {
      case ID_EDIT_MOVE:   if (_mode.IsSet(Mode::GameOver) || !CurrentPlayer()->IsAI()) pCmdUI->Enable(FALSE);     break;
      case ID_EDIT_BOARD:  if (_mode.IsSet(Mode::Editing)) pCmdUI->SetCheck();                                     break;
      case ID_EDIT_PASTE:  if (!_mode.IsSet(Mode::Editing)) pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT)); break;
      case ID_LEVEL_PLUS:  pCmdUI->Enable(FALSE);                                                                  break;
      case ID_LEVEL_MINUS: pCmdUI->Enable(FALSE);                                                                  break;
      default:             pCmdUI->Enable(FALSE);                                                                  break;
    }
  }


  bool Game::React(UINT command)                                          // react to button/menu command
  {
    switch (command)
    {
      case ID_EDIT_MOVE:
        if (!_mode.IsSet(Mode::GameOver))
        {
          AIAction();  // execute computer move if it is its turn
          return true; // update all views
        }
        break;
      case ID_EDIT_BOARD:
        // toggle Editing
        if (_mode.IsSet(Mode::Editing))
        {
          _mode.Del(Mode::Editing);
          _mode.Set(Mode::SelectFr);
        }
        else
        {
          _mode.Del(Mode::SelectFr);
          _mode.Del(Mode::SelectTo);
          _mode.Set(Mode::Editing);
        }        
        return true;
      case ID_EDIT_PASTE:
        pos->SetPosition(Piece::ListFromHTML(ClipBoard::GetClipboardText(),GetHTMLPieceMap()));
        return true;
//    case ID_LEVEL_PLUS: plies++; break;
//    case ID_LEVEL_MINUS: if (plies > 1) plies--; break;
      default:
        break;
    }
    return false;  // no view update needed
  }

  bool Game::React(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/)     // react to keyboard input (not menu shortcuts, but keyboard typing)
  {
    return false;  // no view update needed
  }

  bool Game::React(UINT event, UINT /*nFlags*/, const CPoint& p)          // react to mouse events
  {
    switch (event)
    {
      case WM_LBUTTONDOWN:
        if (_mode.IsSet(Mode::Editing)) DragStart(p);
        break;
      case WM_LBUTTONUP:
        if (_mode.IsSet(Mode::Dragging)) DragEnd(p);
        else Select(p);
        break;
      case WM_LBUTTONDBLCLK: return false;    // that will skip updating all views
      case WM_RBUTTONDOWN:   return false;    // that will skip updating all views
      case WM_RBUTTONUP:
        Unselect();
        break;
      case WM_RBUTTONDBLCLK: return false;    // that will skip updating all views
      case WM_MOUSEMOVE:
        if (_mode.IsSet(Mode::Dragging)) DragTo(p);
        else return false;                    // that will skip updating all views
        break;                               
      default: return false;                  // that will skip updating all views
    }
    return true;                              // update all views
  }

  void Game::DragStart(const CPoint& point)
  {
    Location l{ BoardPart::Main, 0U,0U};
    if (!lay->GetLocation(point, l)) return; // clicked somewhere invalid
    dragPoint = point;
    dragPiece = pos->GetPiece(l);
    _mode.Set(Mode::Dragging);
  }

  void Game::DragEnd(const CPoint& point)
  {
    Location l{ BoardPart::Main, 0U,0U};
    if (lay->GetLocation(point, l))
    {
      if (l._b != BoardPart::Stock)
        pos->SetPiece(l, dragPiece); // dropped on a valid target
    }
    dragPoint = {};
    dragPiece = nullptr;
    _mode.Del(Mode::Dragging);
  }

  void Game::Select(const CPoint& point)
  {
    if (_mode.IsSet(Mode::GameOver) || CurrentPlayer()->IsAI()) return;

    Location l{ BoardPart::Main, 0U,0U };
    if (!lay->GetLocation(point, l)) return;       // user clicked somewhere outside

    if (_mode.IsSet(Mode::SelectFr))  // new selection starts
    {
      assert(moves.empty());
      MainPosition* p{ pos->GetPosition(plist) };  // need to get ALL legal moves (this piece might not be allowed to move because another one has a mandatory jump)
      for (const auto& m : p->GetMoveList(pos->OnTurn() == &Color::White))   // filter moves of the selected piece into 'moves'
      {
        const Location& lf = m->GetFrL();
        if (lf == l || (lf._b == BoardPart::Stock && m->GetToL() == l)) moves.push_back(m);
      }
      if (!moves.empty())
      {
        _mode.Del(Mode::SelectFr);
        _mode.Set(Mode::SelectTo);
      }
    }
    else  // starting point was already defined
    {
      for (const auto& m : moves)                  // check through allowed moves
      {
        if (m->GetToL() == l)
        {
          Execute(*m);
          moves.clear();
          _mode.Del(Mode::SelectTo);
          _mode.Set(Mode::SelectFr);
          return;
        }
      }
    }
  }

}
