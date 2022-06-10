#include "Core.h"

#include <algorithm>

using namespace BoardGamesMFC;

namespace BoardGamesCore
{

  Game::Game(const VariantChosen& v, Board* b) noexcept : v_{ v }, board_{ ai_.Remember(b) }
  {
    AddPlayer(PlayerType::Human, PieceColor::White);
    AddPlayer(PlayerType::Computer, PieceColor::Black);
  }

  Game::~Game() noexcept
  {
    for (auto& p : players_) delete p;
    // do not delete board_ - it is not owned
    Bitmap::Cleanup();
  }
  
  void Game::Serialize(std::stringstream& s) const noexcept
  {
    //if (ar.IsLoading()) ar.Serialize(v_);   // when loading, game type was already read to create this object
    v_.Serialize(s);
    board_->Serialize(s);
  }

  void Game::SetupBoard(const std::vector<PieceIndex>& list) noexcept      
  {
    if (list.empty()) board_->SetStartingBoard();  // no list means use the standard starting board for this game
    else board_->SetupBoard(list);                 // set board up from deserialization
    board_ = ai_.Remember(board_);
  }
  
  void Game::Draw(DC* dc) const
  {
    board_->Draw(dc, true);

    // markup selectable tiles

    if (state_ == State::SelectTarget)
    {
      for (auto& m : moves_)
      {
        board_->DrawSelected(dc, m->GetFrL());
        board_->DrawSelected(dc, m->GetToL());
      }
    }

    if (state_ == State::Dragging)
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


  // UI handling
  bool Game::React(State state, unsigned int event) noexcept  // FSM (Finite State Machine) for UI handling
  {
    struct StateChange {
      State state;
      unsigned int event;
      struct Hash { std::size_t operator()(const StateChange& c) const noexcept { return std::hash<long>()((c.event << 8L) + static_cast<long>(c.state)); } };
      struct Equality { bool operator()(const StateChange& c1, const StateChange& c2) const noexcept { return c1.state == c2.state && c1.event == c2.event; } };
    };

    // list of all allowed transitions
    static const std::unordered_map<StateChange, std::function<bool(Game*)>, StateChange::Hash, StateChange::Equality> reactMap_{
      { {State::SelectFrom,    BoardGamesMFC::Mouse_LButton_Up   }, [](Game* g) -> bool { return g->React_SelectFrom(); } },
      { {State::SelectTarget,  BoardGamesMFC::Mouse_RButton_Up   }, [](Game* g) -> bool { return g->React_Reset(); } },
      { {State::SelectTarget,  BoardGamesMFC::Mouse_LButton_Up   }, [](Game* g) -> bool { return g->React_SelectTarget(); } },
      { {State::ReadyToSubmit, BoardGamesMFC::Mouse_RButton_Up   }, [](Game* g) -> bool { return g->React_Reset(); } },
      { {State::ReadyToSubmit, ID_EDIT_MOVE                      }, [](Game* g) -> bool { return g->React_AIMove(); } },
      { {State::SelectFrom,    ID_EDIT_BOARD                     }, [](Game* g) -> bool { return g->React_StartEdit(); } },
      { {State::BoardEditing,  ID_EDIT_BOARD                     }, [](Game* g) -> bool { return g->React_Reset(); } },
      { {State::BoardEditing,  BoardGamesMFC::Mouse_LButton_Down }, [](Game* g) -> bool { return g->React_StartDrag(); } },
      { {State::Dragging,      BoardGamesMFC::Mouse_Move         }, [](Game* g) -> bool { return g->React_Drag(); } },
      { {State::Dragging,      BoardGamesMFC::Mouse_LButton_Up   }, [](Game* g) -> bool { return g->React_Drop(); } },
      { {State::UIAvailable,   ID_EDIT_MOVE                      }, [](Game* g) -> bool { return g->React_CanMove(); } },
      { {State::UIAvailable,   ID_EDIT_BOARD                     }, [](Game* g) -> bool { return g->React_CanEdit(); } },
      { {State::UIChecked,     ID_EDIT_BOARD                     }, [](Game* g) -> bool { return g->React_Editing(); } },
    };
    const auto validStateChange = reactMap_.find({ state, event });
    return validStateChange != reactMap_.end() && validStateChange->second(this);
  }

  bool Game::React_CanMove() const noexcept
  {
    return state_ == State::ReadyToSubmit;
  }

  bool Game::React_CanEdit() const noexcept
  {
    return state_ == State::SelectFrom || state_ == State::BoardEditing;
  }

  bool Game::React_Editing() const noexcept
  {
    return state_ == State::BoardEditing;
  }

  bool Game::React_SelectFrom() noexcept
  {
    Location l{ BoardPartID::Stage, 0U,0U };
    if (!board_->GetLocationFromPoint(point_, l)) return false;        // user clicked somewhere outside
    for (const auto& m : board_->GetMoveList(board_->WhiteOnTurn()))   // filter moves of the selected piece into 'moves'
    {
      const Location& lf = m->GetFrL();
      if (lf == l || (lf.b_ == BoardPartID::Stock && m->GetToL() == l)) moves_.push_back(m);
    }
    if (moves_.empty()) return false;
    state_ = State::SelectTarget;
    return true;
  }

  bool Game::React_Reset() noexcept
  {
    moves_.clear();
    state_ = State::SelectFrom;
    return true;
  }

  bool Game::React_SelectTarget() noexcept
  {
    Location l{ BoardPartID::Stage, 0U,0U };
    if (!board_->GetLocationFromPoint(point_, l)) return false;        // user clicked somewhere outside
    for (const auto& m : moves_)   // check through allowed moves
    {
      if (m->GetToL() == l)
      {
        MoveP mm = m;
        moves_.clear();
        moves_.push_back(mm);
        state_ = State::ReadyToSubmit;
        return true;
      }
    }
    return false;
  }

  bool Game::React_AIMove() noexcept
  {
    PositionValue v = ai_.MakeMove(board_);  // execute computer move

        // inform player
    if (v == PositionValue::PValueType::Lost)
    {
      BoardGamesMFC::AfxMessageBox(L"Computer resigns - Player wins!");
      state_ = State::GameOver;
      return false;
    }
    if (v == PositionValue::PValueType::Won)  BoardGamesMFC::AfxMessageBox(L"You might as well resign - Computer will win!");
    if (v == PositionValue::PValueType::Tie)  BoardGamesMFC::AfxMessageBox(L"Computer will hold a Draw.");
    state_ = State::SelectFrom;
    return true;
  }
  bool Game::React_StartEdit() noexcept
  {
    state_ = State::BoardEditing;
    return true;
  }
  bool Game::React_StartDrag() noexcept
  {
    Location l{ BoardPartID::Stage, 0U,0U };
    if (!board_->GetLocationFromPoint(point_, l)) return false; // clicked somewhere invalid
    PieceIndex pI = board_->GetPieceIndex(l.x_, l.y_, l.b_);
    if (pI == PMap[Piece::NoTile]) return false;;
    dragPoint_ = point_;
    dragPiece_ = pI;
    state_ = State::Dragging;
    return true;
  }
  bool Game::React_Drag() noexcept
  {
    dragPoint_ = point_;
    return true;
  }
  bool Game::React_Drop() noexcept
  {
    Location l{ BoardPartID::Stage, 0U,0U };
    if (board_->GetLocationFromPoint(point_, l))
    {
      if (l.b_ != BoardPartID::Stock)
        board_->SetPieceIndex(dragPiece_, l.x_, l.y_, l.b_); // dropped on a valid target
    }
    dragPoint_ = {};
    dragPiece_ = PMap[Piece::NoPiece];
    state_ = State::BoardEditing;
    return true;
  }


  //void Game::AddToStock(const Location& l, const Piece& p) noexcept
  //{
  //  board_->SetPiece(p, l.x_, l.y_, l.b_);
  //}
  //
  //void Game::AIAction()  // execute computer moves while it is its turn
  //{
  //  while (!(mode_.IsSet(Mode_::GameOver)) && AIOnTurn())
  //    mode_.Set(ai_.MakeMove(board_));
  //}
  //
  //void Game::AddPlayer(Player* p) noexcept { players_.push_back(p); }
  //void Game::SetCurrentPlayer(unsigned int p) noexcept { current_ = p; }
  //const Player& Game::CurrentPlayer() const noexcept { return *(players_[current_]); }
  //const Player& Game::NextPlayer() noexcept { current_ = ++current_ % players_.size(); return *players_[current_]; }
  //
  //bool Game::AIMove()
  //{
  //  // cleanup position buffer
  //  ai_.Purge(board_->sequence_);  // Positions with less than the current amount of moves can be discarded, they will not be needed any more
  //
  //  // evaluate position
  //  PositionValue v = ai_.Evaluate(board_, AIMethod::BruteForce, 9, 4.0);
  //
  //  // inform player
  //  if (v == PositionValue::PValueType::Lost)
  //  {
  //    BoardGamesMFC::AfxMessageBox(L"Computer resigns - Player wins!");
  //    mode_.Set(Mode_::GameOver);
  //    return false;
  //  }
  //  if (v == PositionValue::PValueType::Won) BoardGamesMFC::AfxMessageBox(L"You might as well resign - Computer will win!");
  //  if (v == PositionValue::PValueType::Tie) BoardGamesMFC::AfxMessageBox(L"Computer will hold a Draw.");
  //
  //  // now execute best move:
  //  const Moves& m = board_->GetMoveList(board_->WhiteOnTurn());
  //  assert(m.size() > 0);
  //  Execute(*m[0]);
  //  return true;
  //}
  //
  //void Game::Execute(const Move& m)
  //{
  //  board_->Execute(m);                                              // execute move (includes setting board_ to next player)
  //  //NextPlayer();                                                    // the game has also a pointer to the current player
  //}

}
