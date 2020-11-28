#include <functional>

namespace BoardGamesCore
{
  using namespace BoardGamesMFC;
 
  class Game;
  class GameDispatch
  {
  public:
    GameDispatch(GameID id, VariantCode c, Coordinate xSize, Coordinate ySize) : p_{ CreateGame(id, c, xSize, ySize) } {}
    ~GameDispatch(void);  // needed to allow unique_ptr to delete the object; default destructor is fine but must be in cpp
    Game* operator->(void) const noexcept;

    void Draw(DC* dc);
    bool React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
    bool React(unsigned int command);
    bool React(unsigned int event, unsigned int nFlags, const Point& p);
    void React(CmdUI* pUI);
    void DragStart(const Point&);
    void DragTo(const Point& point);
    void DragEnd(const Point&);
    void Select(const Point& point);
    void Unselect(void);
    void SetUpdateCallBack(std::function<void(void)> cb);
    void Serialize(Archive& ar);
    void SetPosition(std::vector<const Piece*>& list);

  private:
    Game* p_;
  };

}
