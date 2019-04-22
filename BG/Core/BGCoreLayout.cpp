#include "stdafx.h"

namespace BoardGamesCore
{

  void Piece::Draw(CDC* pDC, const CRect& r, const TileColor* f) const
  {
    if (ID_l == 0 && ID_d == 0) return;
    if (cb_l.m_hObject == 0) cb_l.LoadBitmap(ID_l);
    if (cb_d.m_hObject == 0) cb_d.LoadBitmap(ID_d);
    if (cb_s.m_hObject == 0 && ID_s != 0) cb_s.LoadBitmap(ID_s);

    CBitmap& cb = (f == &TileColor::Light) ? cb_l : ((f == &TileColor::Dark) ? cb_d : cb_s);

    if (cb.m_hObject != 0)
    {
      BITMAP bm;
      cb.GetObject(sizeof(BITMAP), &bm);
      if ((bm.bmWidth != r.Width()) || (bm.bmHeight != r.Height()))
      {
        //throw ("rectangle has wrong size!");
      }
    }
    CDC dcMemory;
    dcMemory.CreateCompatibleDC(pDC);
    dcMemory.SelectObject(&cb);
    pDC->BitBlt(r.TopLeft().x, r.TopLeft().y, r.Width(), r.Height(), &dcMemory, 0, 0, SRCCOPY);
  }


  Layout::Layout(const Dimension& d, LayoutType lt) : dim(d), ltype(lt), tiles{ 1ULL * d.xCount*d.yCount,nullptr }
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
        (tiles)[z] = new Tile(Location(i, j), r, f);
      }
  }

  void Layout::Draw(CDC* pDC, const Position* pos) const
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

  bool Layout::GetLocation(const CPoint& p, Location& l) const
  {
    for (auto& t : tiles)
      if (t->InRect(p)) {
        l = t->GetLocation();
        return true;
      }
    return false;
  }

  void MainLayout::Draw(CDC* pDC, const Position* pos) const
  {
    // frame around the board (needs to be drawn first)
    for (unsigned int z = 4; z > 0; z--)
    {
      if (z != 2)
        pDC->Rectangle((int)(dim.lEdge - z), (int)(dim.tEdge - z), (int)(dim.lEdge + dim.xCount * dim.xDim + z), (int)(dim.tEdge + dim.yCount * dim.yDim + z));
    }
    Layout::Draw(pDC, pos);
  }


  void Game::Draw(CDC* pDC) const
  {
    lay->Draw(pDC, pos);
    // markup selectable tiles
    for (auto& m : moves)
    {
      lay->DrawSelected(pDC, m->GetFr().GetLocation());
      lay->DrawSelected(pDC, m->GetTo().GetLocation());
    }

    if (showStock || editing)
    {
      if (slay != nullptr) slay->Draw(pDC, spos);
    }

    if (tlay != nullptr) tlay->Draw(pDC, tpos);

    if (dragging)
    {
      const CRect r(dragPoint, SIZE{32,32});  // doesn't work for all games! - some have 18x20
      dragPiece->Draw(pDC, r, &TileColor::Small);
    }

    if (true) {
      CString s;
      static char buffer[2000];
      sprintf_s(buffer, "Depth = %d (value = %d) [PList size = %zd] free mem:%zd, sizeof(Position/MainPosition/vector<Move>/Move/SimpleMove/ComplexMove/SimpleStep) = %zd/%zd/%zd/%zd/%zd/%zd/%zd",
        pos->GetDepth(), static_cast<int>(pos->GetValue(pos->OnTurn() == &Color::White)),
        plist.size(), plist.freemem, sizeof(Position), sizeof(MainPosition),sizeof(Moves), sizeof(Move), sizeof(SimpleMove), sizeof(ComplexMove), sizeof(SimpleStep));
      s = buffer;
      pDC->TextOutW(500, 20, s);
    }
  }

  void Game::React(CCmdUI* pCmdUI)                                        // react to UI events (allows to set buttons greyed, etc.)
  {
    switch (pCmdUI->m_nID)
    {
      case ID_EDIT_MOVE:   if (!IsAlive() || !CurrentPlayer()->Is(&PlayerType::Computer)) pCmdUI->Enable(FALSE);     break;
      case ID_EDIT_BOARD:  if (editing) pCmdUI->SetCheck();                                                          break;
      case ID_EDIT_PASTE:  if (!editing) pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));                      break;
      case ID_LEVEL_PLUS:  pCmdUI->Enable(FALSE);                                                                    break;
      case ID_LEVEL_MINUS: pCmdUI->Enable(FALSE);                                                                    break;
      default:             pCmdUI->Enable(FALSE);                                                                    break;
    }
  }


  bool Game::React(UINT command)                                          // react to button/menu command
  {
    switch (command)
    {
      case ID_EDIT_MOVE:
        if (IsAlive())
        {
          AIAction();  // execute computer move if it is its turn
          return true; // update all views
        }
        break;
      case ID_EDIT_BOARD:
        editing ^= true;
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
        if (editing) DragStart(p);
        break;
      case WM_LBUTTONUP:
        if (dragging) DragEnd(p);
        else Select(p);
        break;
      case WM_LBUTTONDBLCLK: return false;    // that will skip updating all views
      case WM_RBUTTONDOWN:   return false;    // that will skip updating all views
      case WM_RBUTTONUP:
        Unselect();
        break;
      case WM_RBUTTONDBLCLK: return false;    // that will skip updating all views
      case WM_MOUSEMOVE:
        if (dragging) DragTo(p);
        else return false;                    // that will skip updating all views
        break;                               
      default: return false;                  // that will skip updating all views
    }
    return true;                              // update all views
  }

  void Game::DragStart(const CPoint& point)
  {
    Location l{0,0};
    if (slay->GetLocation(point, l)) { dragPiece = spos->GetPiece(l); }
    else return; // clicked somewhere outside

    dragPoint = point;
    dragging = true;
  }

  void Game::DragEnd(const CPoint& point)
  {
    Location l{0,0};
    if (lay->GetLocation(point, l)) pos->SetPiece(l, dragPiece); // dropped on a valid target
    dragging = false;
    dragPoint = {};
    dragPiece = nullptr;
  }

  void Game::Select(const CPoint& point)
  {
    if (!IsAlive() || CurrentPlayer()->Is(&PlayerType::Computer)) return;

    Location l{0,0};
    if (!lay->GetLocation(point, l)) return;       // user clicked somewhere outside

    if (moves.empty())  // new selection starts
    {
      MainPosition* p{ pos->GetPosition(plist) };  // need to get ALL legal moves (this piece might not be allowed to move because another one has a mandatory jump)
      for (const auto& m : p->GetMoveList(pos->OnTurn() == &Color::White))   // filter moves of the selected piece into 'moves'
      {
        if (m->GetFr().GetLocation() == l) moves.push_back(m);
      }
    }
    else  // starting point was already defined
    {
      for (const auto& m : moves)                  // check through allowed moves
        if (m->GetTo().GetLocation() == l)
        {
          Execute(m);
          moves.clear();
          return;
        }
    }
  }

}
