#include "MFC-Win.h"

namespace BoardGamesMFC
{
  
  GdiObjectRAII::GdiObjectRAII(CDC* cdc, CGdiObject* gdi) : cdc_{ cdc }, gdi_{ gdi } { gdi_->DeleteObject(); };
  
  GdiObjectRAII::~GdiObjectRAII(void) { /* cdc_->SelectObject(gdi_); */ }
  
  const GdiObjectRAII DC::SelectStockObject(int nIndex)
  {
    return GdiObjectRAII(cdc_, cdc_->SelectStockObject(nIndex));
  }

  const GdiObjectRAII DC::SelectPen(int nPenStyle, int nWidth, unsigned char r, unsigned char g, unsigned char b)
  {
    return SelectPen(nPenStyle, nWidth, RGB(r, g, b));
  }

  const GdiObjectRAII DC::SelectPen(int nPenStyle, int nWidth, unsigned long crColor)
  {
    CPen pen{ nPenStyle, nWidth, crColor };
    return GdiObjectRAII(cdc_, (CGdiObject*) cdc_->SelectObject(&pen));
    pen.Detach();
  }

  void DC::Rectangle(const Rect& r)
  {
    Rectangle(r.left_, r.top_, r.right_, r.bottom_);
  }

  void DC::Rectangle(int l, int t, int r, int b)
  {
    cdc_->Rectangle(l,t,r,b);
  }

  bool DC::Text(int x, int y, std::string s)
  {
    std::wstring w(s.cbegin(), s.cend());
    return cdc_->TextOutW(x, y, (LPCTSTR)w.c_str(), (int)s.size());
  }

}
