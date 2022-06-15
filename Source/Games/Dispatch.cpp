#include "Games.h"
#include "Enum.h"
#include "Dispatch.h"

namespace BoardGamesCore
{

  Game* GameDispatch::operator->() const noexcept { return p_; }
  GameDispatch::~GameDispatch() { delete p_; }

  bool GameDispatch::React(unsigned int command) { return p_->React(command); }
  bool GameDispatch::React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) { return p_->React(nChar, nRepCnt, nFlags); }
  bool GameDispatch::React(unsigned int event, unsigned int nFlags, const Point& p) { return p_->React(event, nFlags, p); }
  void GameDispatch::React(CmdUI* pUI) { p_->React(pUI); }
  void GameDispatch::SetUpdateCallBack(std::function<void()> cb) { p_->SetUpdateCallBack(cb); }
  void GameDispatch::Serialize(Archive& ar) { p_->Serialize(ar); }
  void GameDispatch::Draw(DC* dc) { p_->Draw(dc); }

}
