
namespace BoardGamesCore
{
  using BoardGamesMFC::Point;
  using BoardGamesMFC::CmdUI;

  class IUI abstract // Interface for UI
  {
  public:
    virtual void Draw(DC* dc) const = 0;
    virtual bool React(unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) = 0;   // react to keyboard input (not menu shortcuts, but typing)
    virtual bool React(unsigned int command) = 0;                                            // react to button/menu command
    virtual bool React(unsigned int event, unsigned int nFlags, const Point& p) = 0;         // react to mouse events
    virtual void React(CmdUI* pUI) = 0;                                                      // react to UI events (allows to set buttons greyed, etc.)
    virtual void DragTo(const Point& point) = 0;
    virtual void DragStart(const Point&) = 0;
    virtual void DragEnd(const Point&) = 0;
    virtual void Select(const Point& point) = 0;
    virtual void Unselect(void) = 0;
    virtual void SetUpdateCallBack(std::function<void(void)> cb) = 0;

  protected:
    const Piece* dragPiece_{};       // currently dragged piece
    Point dragPoint_{};             // point the piece is dragged to
  };

  static_assert(std::is_abstract<IUI>::value, "must be abstract");
  static_assert(!std::is_trivially_constructible<class IUI>::value, "must not be trivially constructible");

}
