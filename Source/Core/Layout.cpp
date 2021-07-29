#include "Core.h"

using namespace BoardGamesMFC;

namespace BoardGamesCore
{
  void Tile::Draw(DC* pDC, const Piece& p) const
  {
    if (p == Piece::NoTile) return;
    tilecolor_.Draw(pDC, rect_);
    if (p == Piece::NoPiece) return;
    p.Draw(pDC, rect_);
  }

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
            default:                      f = &TileColor::Light;                                     break;
          }
          assert(f != nullptr);

          const Rect r{
            (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * i),
            (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * j),
            (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * (i + 1U)),
            (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * (j + 1U)) };
          (tiles_)[z] = new Tile(Location(b, i, j), r, *f);
        }
    }
  }

  void Layout::Draw(DC* dc, const Position* pos, _Mode /*mode*/) const
  {
    for (const auto& t : tiles_)
      t->Draw(dc, pos->GetPiece(t->GetLocation()));
  }

  void Layout::DrawSelected(DC* dc, const Location& l) const
  {
    for (auto& t : tiles_)
      if (t->GetLocation() == l)
      {
        Rect r = t->GetRect();
        r.DeflateRect(-1, -1, -1, -1);
        auto oldB = dc->SelectStockObject(No_Brush);
        auto oldP = dc->SelectPen(PenStyle_Solid, 2, 0, 128, 255);
        dc->Rectangle(r);
      }
  }

  bool Layout::GetLocation(const Point& p, Location& l) const noexcept
  {
    for (auto& t : tiles_)
      if (t->InRect(p)) {
        l = t->GetLocation();
        return true;
      }
    return false;
  }

  void MainLayout::Draw(DC* dc, const MainPosition* pos, _Mode mode) const
  {
    // frame around the board (needs to be drawn first)
    for (unsigned int z = 4; z > 0; z--)
    {
      if (z != 2)
        dc->Rectangle((int)(dim_.lEdge_ - z), (int)(dim_.tEdge_ - z), (int)(dim_.lEdge_ + dim_.xCount_ * (dim_.xDim_ + dim_.xSkip_) + z), (int)(dim_.tEdge_ + dim_.yCount_ * (dim_.yDim_ + dim_.ySkip_) + z));
    }
    Layout::Draw(dc, pos, mode);

    if (mode.IsSet(Mode::ShowStock) || mode.IsSet(Mode::Editing))
    {
      stock_.Draw(dc, &(pos->stock_), mode);
    }
    taken_.Draw(dc, &(pos->taken_), mode);
  }


  void Game::Draw(DC* dc) const
  {
    lay->Draw(dc, pos, _mode);

    // markup selectable tiles
    
    if (_mode.IsSet(Mode::SelectTo))
    {
      for (auto& m : moves)
      {
        lay->DrawSelected(dc, m->GetFrL());
        lay->DrawSelected(dc, m->GetToL());
      }
    }

    if (_mode.IsSet(Mode::Dragging))
    {
      const Rect r(dragPoint_.x_, dragPoint_.y_, dragPoint_.x_+32, dragPoint_.y_+32);  // doesn't work for all games! - some have 18x20!
      dragPiece_->Draw(dc, r);
    }

    if (pos->GetDepth() >= 0U) {
      char s[1000];
      int h = 10;
      const char* v = static_cast<const char*>(pos->GetValue(pos->OnTurn() == PieceColor::White));
      strcpy_s(s,"Depth"               ); dc->Text(800, h += 20, s);  sprintf_s(s,"%u", pos->GetDepth());        dc->Text(1000, h, s);
      strcpy_s(s,"Value"               ); dc->Text(800, h += 20, s);  sprintf_s(s,"%s",v);                       dc->Text(1000, h, s);
      strcpy_s(s,"PList size"          ); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", plist.size());          dc->Text(1000, h, s);
      strcpy_s(s,"free mem"            ); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", plist.freemem);         dc->Text(1000, h, s);
      strcpy_s(s,"sizeof(Position)"    ); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", sizeof(Position));      dc->Text(1000, h, s);
      strcpy_s(s,"sizeof(MainPosition)"); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", sizeof(MainPosition));  dc->Text(1000, h, s);
      strcpy_s(s,"sizeof(vector<Move>)"); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", sizeof(Moves));         dc->Text(1000, h, s);
      strcpy_s(s,"sizeof(Move)"        ); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", sizeof(Move));          dc->Text(1000, h, s);
      strcpy_s(s,"sizeof(Action)"      ); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", sizeof(Action));        dc->Text(1000, h, s);
      strcpy_s(s,"sizeof(ActionLift)"  ); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", sizeof(ActionLift));    dc->Text(1000, h, s);
      strcpy_s(s,"sizeof(ActionJump)"  ); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", sizeof(ActionJump));    dc->Text(1000, h, s);
      strcpy_s(s,"sizeof(ActionDrop)"  ); dc->Text(800, h += 20, s);  sprintf_s(s,"%zu", sizeof(ActionDrop));    dc->Text(1000, h, s);
    }
  }

  void Game::React(CmdUI* pCmdUI)                                        // react to UI events (allows to set buttons greyed, etc.)
  {
    switch (pCmdUI->GetID())
    {
      case ID_EDIT_MOVE:   if (_mode.IsSet(Mode::GameOver) || !CurrentPlayer()->IsAI()) pCmdUI->Enable(false);     break;
      case ID_EDIT_BOARD:  if (_mode.IsSet(Mode::Editing)) pCmdUI->SetCheck();                                     break;
      case ID_EDIT_INSERT: if (!_mode.IsSet(Mode::Editing)) pCmdUI->Enable(ClipBoard::IsClipboardFormatText());    break;
      case ID_LEVEL_PLUS:  pCmdUI->Enable(false);                                                                  break;
      case ID_LEVEL_MINUS: pCmdUI->Enable(false);                                                                  break;
      default:             pCmdUI->Enable(false);                                                                  break;
    }
  }


  bool Game::React(unsigned int command)                                          // react to button/menu command
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
      case ID_EDIT_INSERT:
        //pos->SetPosition(Piece::ListFromHTML(ClipBoard::GetClipboardText(),GetHTMLPieceMap()));
        return true;
//    case ID_LEVEL_PLUS: plies++; break;
//    case ID_LEVEL_MINUS: if (plies > 1) plies--; break;
      default:
        break;
    }
    return false;  // no view update needed
  }

  bool Game::React(unsigned int /*nChar*/, unsigned int /*nRepCnt*/, unsigned int /*nFlags*/)     // react to keyboard input (not menu shortcuts, but keyboard typing)
  {
    return false;  // no view update needed
  }

  bool Game::React(unsigned int event, unsigned int /*nFlags*/, const Point& p)          // react to mouse events
  {
    switch (event)
    {
      case Mouse_LButton_Down:
        if (_mode.IsSet(Mode::Editing)) DragStart(p);
        break;
      case Mouse_LButton_Up:
        if (_mode.IsSet(Mode::Dragging)) DragEnd(p);
        else Select(p);
        break;
      case Mouse_LButton_DblClk: return false;    // that will skip updating all views
      case Mouse_RButton_Down:   return false;    // that will skip updating all views
      case Mouse_RButton_Up:
        Unselect();
        break;
      case Mouse_RButton_DblClk: return false;    // that will skip updating all views
      case Mouse_Move:
        if (_mode.IsSet(Mode::Dragging)) DragTo(p);
        else return false;                    // that will skip updating all views
        break;                               
      default: return false;                  // that will skip updating all views
    }
    return true;                              // update all views
  }

  void Game::DragStart(const Point& point)
  {
    Location l{ BoardPart::Main, 0U,0U};
    if (!lay->GetLocation(point, l)) return; // clicked somewhere invalid
    dragPoint_ = point;
    dragPiece_ = &(pos->GetPiece(l));
    _mode.Set(Mode::Dragging);
  }

  void Game::DragEnd(const Point& point)
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

  void Game::Select(const Point& point)
  {
    if (_mode.IsSet(Mode::GameOver) || CurrentPlayer()->IsAI()) return;

    Location l{ BoardPart::Main, 0U,0U };
    if (!lay->GetLocation(point, l)) return;       // user clicked somewhere outside

    if (_mode.IsSet(Mode::SelectFr))  // new selection starts
    {
      assert(moves.empty());
      MainPosition* p{ pos->GetPosition(plist) };  // need to get ALL legal moves (this piece might not be allowed to move because another one has a mandatory jump)
      for (const auto& m : p->GetMoveList(pos->OnTurn() == PieceColor::White))   // filter moves of the selected piece into 'moves'
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
