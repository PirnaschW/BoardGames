#include "stdafx.h"

namespace BoardGamesCore
{

  Layout::Layout(const Dimension& d, const BoardPart b, const LayoutType lt) noexcept : dim_(d), ltype_(lt), tiles_{ 1ULL * d.xCount_*d.yCount_,nullptr }
  {
    if (ltype_ != LayoutType::None)  // None means the Game is handling its own tile creation
    {
      unsigned int z = 0;
      for (Coordinate i = 0; i < dim_.xCount_; i++)
        for (Coordinate j = 0; j < dim_.yCount_; j++, z++)
        {
          const TileColor* f{nullptr};
          switch (ltype_)
          {
            case LayoutType::Light:       f = &TileColor::Light;                                     break;
            case LayoutType::Dark:        f = &TileColor::Dark;                                      break;
            case LayoutType::Small:       f = &TileColor::Small;                                     break;
            case LayoutType::Alternating: f = ((i + j) % 2) ? &TileColor::Dark : &TileColor::Light;  break;
          }
          assert(f != nullptr);

          const CRect r{
            (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * i),
            (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * j),
            (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * (i + 1U)),
            (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * (j + 1U)) };
          (tiles_)[z] = new Tile(Location(b, i, j), r, *f);
        }
    }
  }

  void Layout::Draw(CDC* pDC, const Position* pos, _Mode mode) const
  {
    for (const auto& t : tiles_)
      t->Draw(pDC, pos->GetPiece(t->GetLocation()));
  }

  void Layout::DrawSelected(CDC* pDC, const Location& l) const
  {
    for (auto& t : tiles_)
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
    for (auto& t : tiles_)
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
        pDC->Rectangle((int)(dim_.lEdge_ - z), (int)(dim_.tEdge_ - z), (int)(dim_.lEdge_ + dim_.xCount_ * (dim_.xDim_ + dim_.xSkip_) + z), (int)(dim_.tEdge_ + dim_.yCount_ * (dim_.yDim_ + dim_.ySkip_) + z));
    }
    Layout::Draw(pDC, pos, mode);

    if (mode.IsSet(Mode::ShowStock) || mode.IsSet(Mode::Editing))
    {
      stock_.Draw(pDC, &(pos->stock_), mode);
    }
    taken_.Draw(pDC, &(pos->taken_), mode);
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
      const CRect r(dragPoint_, SIZE{32,32});  // doesn't work for all games! - some have 18x20
      dragPiece_->Draw(pDC, r, TileColor::Small);
    }

    if (true) {
      CString s;
      int h = 10;
      const char* v = static_cast<const char*>(pos->GetValue(pos->OnTurn() == Color::White));
      s = "Depth";                pDC->TextOutW(800, h += 20, s);  s.Format(_T("%d"),  pos->GetDepth());       pDC->TextOutW(1000, h, s);
      s = "Value";                pDC->TextOutW(800, h += 20, s);  s = v;                                      pDC->TextOutW(1000, h, s);
      s = "PList size";           pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), plist->size());         pDC->TextOutW(1000, h, s);
      s = "free mem";             pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), plist->freemem);        pDC->TextOutW(1000, h, s);
      s = "sizeof(Position)";     pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(Position));      pDC->TextOutW(1000, h, s);
      s = "sizeof(MainPosition)"; pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(MainPosition));  pDC->TextOutW(1000, h, s);
      s = "sizeof(vector<Move>)"; pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(Moves));         pDC->TextOutW(1000, h, s);
      s = "sizeof(Move)";         pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(Move));          pDC->TextOutW(1000, h, s);
      s = "sizeof(Action)";       pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(Action));        pDC->TextOutW(1000, h, s);
      s = "sizeof(ActionTake)";   pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(ActionTake));    pDC->TextOutW(1000, h, s);
      s = "sizeof(ActionJump)";   pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(ActionJump));    pDC->TextOutW(1000, h, s);
      s = "sizeof(ActionPlace)";  pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(ActionPlace));   pDC->TextOutW(1000, h, s);
    //s = "sizeof(StepPlace)";    pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(StepPlace));     pDC->TextOutW(1000, h, s);
    //s = "sizeof(StepSimple)";   pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(StepSimple));    pDC->TextOutW(1000, h, s);
    //s = "sizeof(StepComplex)";  pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(StepComplex));   pDC->TextOutW(1000, h, s);
    //s = "sizeof(SimpleMove)";   pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(SimpleMove));    pDC->TextOutW(1000, h, s);
    //s = "sizeof(ComplexMove)";  pDC->TextOutW(800, h += 20, s);  s.Format(_T("%zu"), sizeof(ComplexMove));   pDC->TextOutW(1000, h, s);
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
    dragPoint_ = point;
    dragPiece_ = &(pos->GetPiece(l));
    _mode.Set(Mode::Dragging);
  }

  void Game::DragEnd(const CPoint& point)
  {
    Location l{ BoardPart::Main, 0U,0U};
    if (lay->GetLocation(point, l))
    {
      if (l.b_ != BoardPart::Stock)
        pos->SetPiece(l, *dragPiece_); // dropped on a valid target
    }
    dragPoint_ = {};
    dragPiece_ = nullptr;
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
      for (const auto& m : p->GetMoveList(pos->OnTurn() == Color::White))   // filter moves of the selected piece into 'moves'
      {
        const Location& lf = m->GetFrL();
        if (lf == l || (lf.b_ == BoardPart::Stock && m->GetToL() == l)) moves.push_back(m);
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
