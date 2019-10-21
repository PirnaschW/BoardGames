namespace BoardGamesCore
{

  class IUI abstract // Interface for UI
  {
  public:
    virtual void Draw(CDC* pDC) const = 0;
    virtual bool React(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/) = 0;      // react to keyboard input (not menu shortcuts, but typing)
    virtual bool React(UINT command) = 0;                                           // react to button/menu command
    virtual bool React(UINT event, UINT nFlags, const CPoint& p) = 0;               // react to mouse events
    virtual void React(CCmdUI* pCmdUI) = 0;                                         // react to UI events (allows to set buttons greyed, etc.)
    virtual void DragTo(const CPoint& point) = 0;
    virtual void DragStart(const CPoint&) = 0;
    virtual void DragEnd(const CPoint&) = 0;
    virtual void Select(const CPoint& point) = 0;
    virtual inline void Unselect(void) = 0;
    virtual void SetUpdateCallBack(std::function<void(void)> cb) = 0;
  protected:
    const Piece* dragPiece_{};       // currently dragged piece
    CPoint dragPoint_{};             // point the piece is dragged to
  };

  static_assert(std::is_abstract<IUI>::value, "must be abstract");
  static_assert(!std::is_trivially_constructible<class IUI>::value, "must not be trivially constructible");

}