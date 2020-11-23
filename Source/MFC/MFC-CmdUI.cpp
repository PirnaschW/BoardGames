#include "MFC-Win.h"

namespace BoardGamesMFC
{

  CmdUI::CmdUI(CCmdUI* pUI) noexcept : pUI_{ pUI }, ID_(pUI->m_nID) {}
  void CmdUI::Enable(bool b) const noexcept { pUI_->Enable(b); }
  void CmdUI::SetCheck(void) const noexcept { pUI_->SetCheck(); }

}
