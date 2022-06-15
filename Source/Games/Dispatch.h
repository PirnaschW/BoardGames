#include <functional>

namespace BoardGamesCore
{
  using namespace BoardGamesMFC;
 
  class Game;
  class GameDispatch
  {
  public:
    GameDispatch(const VariantChosen& v, const std::vector<PieceIndex>& list = std::vector<PieceIndex>{}) : p_{ CreateGame(v, list) } {}
    ~GameDispatch();  // needed to allow unique_ptr to delete the object; default destructor is fine but must be in cpp
    Game* operator->() const noexcept;

    void Draw(DC* dc);
    bool React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
    bool React(unsigned int command);
    bool React(unsigned int event, unsigned int nFlags, const Point& p);
    void React(CmdUI* pUI);
    void SetUpdateCallBack(std::function<void()> cb);
    void Serialize(Archive& ar);

  private:
    Game* p_;
  };

}
