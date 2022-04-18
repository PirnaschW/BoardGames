
class CDC;
class CBrush;
class CPen;

namespace BoardGamesMFC
{
 
  class Pen
  {
  public:
    constexpr Pen(int nPenStyle, int nWidth, unsigned char r, unsigned char g, unsigned char b) noexcept;
    constexpr ~Pen() noexcept;

  public:
    const CPen* const pen_;

  public:
    static const Pen PenBlack;
    static const Pen PenSelected;
  };
  constexpr inline const Pen Pen::PenBlack   { PenStyle_Solid, 1,   0,   0,   0 };
  constexpr inline const Pen Pen::PenSelected{ PenStyle_Solid, 3,  64,  64, 255 };

  class Brush
  {
  public:
    constexpr Brush(unsigned char r, unsigned char g, unsigned char b) noexcept;
    Brush(unsigned int style) noexcept;
    Brush(unsigned int style, unsigned char r, unsigned char g, unsigned char b, unsigned long hatch) noexcept;
    constexpr ~Brush() noexcept;

  public:
    const CBrush* const brush_;

  public:
    static const Brush BrushNull;
    static const Brush Brushxxx;
  };


  class DC
  {
  public:
    constexpr DC(CDC* cdc) noexcept : cdc_{ cdc } {};
    constexpr operator CDC* () const noexcept { return cdc_; }
    void Rectangle(const Rect& r, const Pen& p = Pen::PenBlack, const Brush& brush = Brush::BrushNull);
    bool Text(int x, int y, std::wstring s);

  private:
    CDC* const cdc_{ nullptr };
  };

}
