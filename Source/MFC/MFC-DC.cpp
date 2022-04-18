#include "MFC-Win.h"

namespace BoardGamesMFC
{

  // Pen handling
  constexpr Pen::Pen(int nPenStyle, int nWidth, unsigned char r, unsigned char g, unsigned char b) noexcept
    : pen_{ new CPen(nPenStyle,nWidth,RGB(r,g,b)) }
  {}
  constexpr Pen::~Pen() noexcept { delete pen_; }


  // Brush handling
  namespace
  {
    static inline CBrush* Indirect(const LOGBRUSH& l) noexcept // helper to create indirect
    {
      CBrush* b = new CBrush;
      b->CreateBrushIndirect(&l);
      return b;
    }
  }
  constexpr Brush::Brush(unsigned char r, unsigned char g, unsigned char b) noexcept : brush_{ new CBrush(RGB(r,g,b)) } {}
  Brush::Brush(unsigned int style) noexcept : brush_{ Indirect(LOGBRUSH{style,RGB(0,0,0),HS_CROSS}) }  {}
  Brush::Brush(unsigned int style, unsigned char r, unsigned char g, unsigned char b, unsigned long hatch) noexcept
    : brush_{ Indirect(LOGBRUSH{style,RGB(r,g,b),hatch}) } {}
  constexpr Brush::~Brush() noexcept { delete brush_; }

  const Brush Brush::Brushxxx{ 128, 128, 255 }; // example brush
  const Brush Brush::BrushNull{BS_NULL};


  void DC::Rectangle(const Rect& r, const Pen& pen, const Brush& brush)
  {
    cdc_->SelectObject(const_cast<CPen*  >(pen  .pen_  ));
    cdc_->SelectObject(const_cast<CBrush*>(brush.brush_));
    cdc_->Rectangle(r.left_, r.top_, r.right_, r.bottom_);
  }
  bool DC::Text(int x, int y, std::wstring s) { return cdc_->TextOutW(x, y, (LPCTSTR)s.c_str(), (int)s.size()); }

}
