#include "Games.h"
#include "Enum.h"
#include "Dispatch.h"

namespace BoardGamesCore
{

  Game* GameDispatch::operator->(void) const noexcept { return p_; }
  GameDispatch::~GameDispatch(void) { delete p_; }

  bool GameDispatch::React(unsigned int command) { return p_->React(command); }
  bool GameDispatch::React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) { return p_->React(nChar, nRepCnt, nFlags); }
  bool GameDispatch::React(unsigned int event, unsigned int nFlags, const Point& p) { return p_->React(event, nFlags, p); }
  void GameDispatch::React(CmdUI* pUI) { p_->React(pUI); }
  void GameDispatch::DragStart(const Point& point) { p_->DragStart(point); }
  void GameDispatch::DragTo(const Point& point) { p_->DragTo(point); }
  void GameDispatch::DragEnd(const Point& point) { p_->DragEnd(point); }
  void GameDispatch::Select(const Point& point) { p_->Select(point); }
  void GameDispatch::Unselect(void) { p_->Unselect(); }
  void GameDispatch::SetUpdateCallBack(std::function<void(void)> cb) { p_->SetUpdateCallBack(cb); }
  void GameDispatch::Serialize(Archive& ar) { p_->Serialize(ar); }
  void GameDispatch::Draw(DC* dc) { p_->Draw(dc); }

}
