#include "Core.h"

//#include <chrono>
#include <algorithm>

using namespace BoardGamesMFC;

namespace BoardGamesCore
{

  Game::Game(const VariantChosen& v, Board* b) noexcept : v_{ v }, board_{ ai_.Remember(b) }
  {
    AddPlayer(new Player(PlayerType::Human, PieceColor::White));
    AddPlayer(new Player(PlayerType::Computer, PieceColor::Black));
  }

  Game::~Game()
  {
    for (auto& p : players_) delete p;
   // delete board_;  // NO! already deleted when plist_ is cleaned
    Bitmap::Cleanup();
  }
  
  void Game::Serialize(Archive& ar)
  {
    if (ar.IsLoading()) ar.Serialize(v_);   // when loading, game type was already read to create this object
    board_->Serialize(ar);
  }

  //void Game::AddToStock(const Location& l, const Piece& p) noexcept
  //{
  //  board_->SetPiece(p, l.x_, l.y_, l.b_);
  //}

  void Game::AIAction()  // execute computer moves while it is its turn
  {
    while (!(mode_.IsSet(Mode_::GameOver)) && CurrentPlayer().IsAI())
      if (!AIMove()) mode_ = Mode_::GameOver;
  }

  void Game::AddPlayer(Player* p) noexcept { players_.push_back(p); }
  void Game::SetCurrentPlayer(unsigned int p) noexcept { current_ = p; }
  const Player& Game::CurrentPlayer() const noexcept { return *(players_[current_]); }
  const Player& Game::NextPlayer() noexcept { current_ = ++current_ % players_.size(); return *players_[current_]; }
  
  bool Game::AIMove()
  {
    // cleanup position buffer
    ai_.Purge(board_->sequence_);  // Positions with less than the current amount of moves can be discarded, they will not be needed any more

    // evaluate position
    PositionValue v = ai_.Evaluate(board_, AIMethod::BruteForce, 9, 4.0);

    // inform player
    if (v == PositionValue::PValueType::Lost)
    {
      BoardGamesMFC::AfxMessageBox(L"Computer resigns - Player wins!");
      mode_.Set(Mode_::GameOver);
      return false;
    }
    if (v == PositionValue::PValueType::Won) BoardGamesMFC::AfxMessageBox(L"You might as well resign - Computer will win!");
    if (v == PositionValue::PValueType::Tie) BoardGamesMFC::AfxMessageBox(L"Computer will hold a Draw.");

    // now execute best move:
    const Moves& m = board_->GetMoveList(board_->WhiteOnTurn());
    assert(m.size() > 0);
    Execute(*m[0]);
    return true;
  }
  
  void Game::Execute(const Move& m)
  {
    board_->Execute(m);                                              // execute move (includes setting board_ to next player)
    NextPlayer();                                                    // the game has also a pointer to the current player
  }

  void Game::SetBoard(const std::vector<PieceIndex>& list) noexcept       // set board from deserialization
  {
    if (list.empty()) board_->SetStartingBoard();
    else board_->SetBoard(list);

    board_->EvaluateStatically();
    board_ = ai_.Remember(board_);
  }


  void Game::Draw(DC* dc) const
  {
    board_->Draw(dc, mode_);

    // markup selectable tiles

    if (mode_.IsSet(Mode_::SelectTo))
    {
      for (auto& m : moves_)
      {
        board_->DrawSelected(dc, m->GetFrL());
        board_->DrawSelected(dc, m->GetToL());
      }
    }

    if (mode_.IsSet(Mode_::Dragging))
    {
      if (dragPiece_ != PMap[Piece::NoPiece])
      {
        const Rect r(dragPoint_.x_, dragPoint_.y_, dragPoint_.x_ + 32, dragPoint_.y_ + 32);  // doesn't work for all games! - some have 18x20!
        PMap[dragPiece_].Draw(dc, r);
      }
    }

    if (board_->GetDepth() >= 0U) {
      wchar_t s[1000];
      int h = 10;
      const wchar_t* v = static_cast<const wchar_t*>(board_->GetValue(board_->WhiteOnTurn()));
      wcscpy_s(s, L"Depth");                dc->Text(800, h += 20, s);  swprintf_s(s, L"%u", board_->GetDepth());        dc->Text(1000, h, s);
      wcscpy_s(s, L"Value");                dc->Text(800, h += 20, s);  swprintf_s(s, L"%s", v);                       dc->Text(1000, h, s);
      wcscpy_s(s, L"PList size");           dc->Text(800, h += 20, s);  swprintf_s(s, L"%zu", ai_.GetSize());          dc->Text(1000, h, s);
      wcscpy_s(s, L"sizeof(BoardPart)");    dc->Text(800, h += 20, s);  swprintf_s(s, L"%zu", sizeof(BoardPart));      dc->Text(1000, h, s);
      wcscpy_s(s, L"sizeof(Board)");        dc->Text(800, h += 20, s);  swprintf_s(s, L"%zu", sizeof(Board));  dc->Text(1000, h, s);
      wcscpy_s(s, L"sizeof(vector<Move>)"); dc->Text(800, h += 20, s);  swprintf_s(s, L"%zu", sizeof(Moves));         dc->Text(1000, h, s);
      wcscpy_s(s, L"sizeof(Move)");         dc->Text(800, h += 20, s);  swprintf_s(s, L"%zu", sizeof(Move));          dc->Text(1000, h, s);
      wcscpy_s(s, L"sizeof(Action)");       dc->Text(800, h += 20, s);  swprintf_s(s, L"%zu", sizeof(Action));        dc->Text(1000, h, s);
      wcscpy_s(s, L"sizeof(ActionLift)");   dc->Text(800, h += 20, s);  swprintf_s(s, L"%zu", sizeof(ActionLift));    dc->Text(1000, h, s);
      wcscpy_s(s, L"sizeof(ActionJump)");   dc->Text(800, h += 20, s);  swprintf_s(s, L"%zu", sizeof(ActionJump));    dc->Text(1000, h, s);
      wcscpy_s(s, L"sizeof(ActionDrop)");   dc->Text(800, h += 20, s);  swprintf_s(s, L"%zu", sizeof(ActionDrop));    dc->Text(1000, h, s);
    }
  }

  void Game::React(CmdUI* pCmdUI)                                        // react to UI events (allows to set buttons greyed, etc.)
  {
    switch (pCmdUI->GetID())
    {
    case ID_EDIT_MOVE:   if (mode_.IsSet(Mode_::GameOver) || !CurrentPlayer().IsAI()) pCmdUI->Enable(false);     break;
    case ID_EDIT_BOARD:  if (mode_.IsSet(Mode_::Editing)) pCmdUI->SetCheck();                                    break;
    case ID_EDIT_INSERT: if (!mode_.IsSet(Mode_::Editing)) pCmdUI->Enable(ClipBoard::IsClipboardFormatText());   break;
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
      if (!mode_.IsSet(Mode_::GameOver))
      {
        AIAction();  // execute computer move if it is its turn
        return true; // update all views
      }
      break;
    case ID_EDIT_BOARD:
      // toggle Editing
      if (mode_.IsSet(Mode_::Editing))
      {
        mode_.Del(Mode_::Editing);
        mode_.Set(Mode_::SelectFr);
      }
      else
      {
        mode_.Del(Mode_::SelectFr);
        mode_.Del(Mode_::SelectTo);
        mode_.Set(Mode_::Editing);
      }
      return true;
    case ID_EDIT_INSERT:
      //board_->SetBoard(Piece::ListFromHTML(ClipBoard::GetClipboardText(),GetHTMLPieceMap()));
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
      if (mode_.IsSet(Mode_::Editing)) DragStart(p);
      break;
    case Mouse_LButton_Up:
      if (mode_.IsSet(Mode_::Dragging)) DragEnd(p);
      else Select(p);
      break;
    case Mouse_LButton_DblClk: return false;    // that will skip updating all views
    case Mouse_RButton_Down:   return false;    // that will skip updating all views
    case Mouse_RButton_Up:
      Unselect();
      break;
    case Mouse_RButton_DblClk: return false;    // that will skip updating all views
    case Mouse_Move:
      if (mode_.IsSet(Mode_::Dragging)) DragTo(p);
      else return false;                    // that will skip updating all views
      break;
    default: return false;                  // that will skip updating all views
    }
    return true;                              // update all views
  }

  void Game::DragStart(const Point& point)
  {
    Location l{ BoardPartID::Stage, 0U,0U };
    if (!board_->GetLocationFromPoint(point, l)) return; // clicked somewhere invalid
    PieceIndex pI = board_->GetPieceIndex(l.x_, l.y_, l.b_);
    if (pI == PMap[Piece::NoTile]) return;
    dragPoint_ = point;
    dragPiece_ = pI;
    mode_.Set(Mode_::Dragging);
  }

  void Game::DragEnd(const Point& point)
  {
    Location l{ BoardPartID::Stage, 0U,0U };
    if (board_->GetLocationFromPoint(point, l))
    {
      if (l.b_ != BoardPartID::Stock)
        board_->SetPieceIndex(dragPiece_, l.x_, l.y_, l.b_); // dropped on a valid target
    }
    dragPoint_ = {};
    dragPiece_ = PMap[Piece::NoPiece];
    mode_.Del(Mode_::Dragging);
  }

  void Game::Select(const Point& point)
  {
    if (mode_.IsSet(Mode_::GameOver) || CurrentPlayer().IsAI()) return;

    Location l{ BoardPartID::Stage, 0U,0U };
    if (!board_->GetLocationFromPoint(point, l)) return;       // user clicked somewhere outside

    if (mode_.IsSet(Mode_::SelectFr))  // new selection starts
    {
      //assert(moves_.empty());
      //// need to get ALL legal moves (this piece might not be allowed to move because another one has a mandatory jump)
      //Evaluator eva(plist_, board_, Evaluator::AIMethod::Statically);
      //eva.Evaluate(0, 0.0);
      //plist_.Insert(board_);

      for (const auto& m : board_->GetMoveList(board_->WhiteOnTurn()))   // filter moves of the selected piece into 'moves'
      {
        const Location& lf = m->GetFrL();
        if (lf == l || (lf.b_ == BoardPartID::Stock && m->GetToL() == l)) moves_.push_back(m);
      }
      if (!moves_.empty())
      {
        mode_.Del(Mode_::SelectFr);
        mode_.Set(Mode_::SelectTo);
      }
    }
    else  // starting point was already defined
    {
      for (const auto& m : moves_)                  // check through allowed moves
      {
        if (m->GetToL() == l)
        {
          Execute(*m);
          moves_.clear();
          mode_.Del(Mode_::SelectTo);
          mode_.Set(Mode_::SelectFr);
          return;
        }
      }
    }
  }

}
