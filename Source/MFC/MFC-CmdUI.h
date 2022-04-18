
class CCmdUI;

namespace BoardGamesMFC
{

  class CmdUI
  {
  public:
    CmdUI(CCmdUI* pUI) noexcept;
    constexpr unsigned int GetID() const noexcept { return ID_; }
    void Enable(bool b) const noexcept;
    void SetCheck() const noexcept;

  private:
    CCmdUI* const pUI_;  // just store it so we can use it, we do NOT own it
    unsigned int ID_;
  };

}
