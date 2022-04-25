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
