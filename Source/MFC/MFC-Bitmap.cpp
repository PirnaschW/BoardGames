#include "MFC-Win.h"

namespace BoardGamesMFC
{

  class Bitmap::Internal
  {
  public:
    Internal() = delete;
    constexpr Internal(unsigned int id) noexcept : id_(id) {}
    ~Internal() noexcept { delete bmP_; delete bmM_; };
    void Draw(DC* dc, const Rect& r) const;

  private:
    void Load(DC* dc, const Rect& r) const;

    const unsigned int id_{};
    mutable CBitmap* bmP_{nullptr};  // Object bitmap - mutable to allow 'lazy' fill - also, Windows doesn't allow filling before main()
    mutable CBitmap* bmM_{nullptr};  // Mask   bitmap 
  };

  void Bitmap::Internal::Load(DC* dc, const Rect& r) const
  {
    bmP_ = new CBitmap();
    bmM_ = new CBitmap();

    bmP_->LoadBitmap(id_);
    assert(bmP_->m_hObject != 0);

    BITMAP bm;
    bmP_->GetObject(sizeof(BITMAP), &bm);  // really does 'GetObjectInfo'
    if ((bm.bmWidth != r.Width()) || (bm.bmHeight != r.Height()))
    {
      //throw ("rectangle has wrong size!");
    }

    // Create monochrome (1 bit) mask bitmap.  
    bmM_->CreateBitmap(bm.bmWidth, bm.bmHeight, 1U, 1U, NULL);

    CDC* const pCDC = *dc;

    // Get compatible DC for mask calculation
    CDC CDCImage;
    CDCImage.CreateCompatibleDC(pCDC);
    CBitmap* Imageold = CDCImage.SelectObject(bmP_);
    // Set the background color of the image to the color to be transparent.
    CDCImage.SetBkColor(COLORREF(0x00FF00FF));

    CDC CDCMask;
    CDCMask.CreateCompatibleDC(pCDC);
    CBitmap* Maskold = CDCMask.SelectObject(bmM_);
    // Copy image to the B+W mask - background color ends up white, everything else ends up black
    CDCMask.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &CDCImage, 0, 0, SRCCOPY);

    // use mask to turn the transparent colour in original image to black so the transparency effect will work right
    CDCImage.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &CDCMask, 0, 0, SRCINVERT);

    // clean up
    CDCMask.SelectObject(Maskold);
    CDCImage.SelectObject(Imageold);
    CDCMask.DeleteDC();
    CDCImage.DeleteDC();
  }


  void Bitmap::Internal::Draw(DC* dc, const Rect& r) const
  {
    CDC* const pCDC = *dc;
    if (bmP_ == nullptr) Load(dc, r); // lazy load

    CDC dcMemory;
    dcMemory.CreateCompatibleDC(pCDC);
    CBitmap* MaskOld = dcMemory.SelectObject(bmM_);
    pCDC->BitBlt(r.left_, r.top_, r.Width(), r.Height(), &dcMemory, 0, 0, SRCAND);

    dcMemory.SelectObject(bmP_);
    pCDC->BitBlt(r.left_, r.top_, r.Width(), r.Height(), &dcMemory, 0, 0, SRCPAINT);
    dcMemory.SelectObject(MaskOld);
    dcMemory.DeleteDC();
  }


  void Bitmap::Draw(DC* dc, const Rect& r) const
  {
    Internal*& b = map_[id_];
    if (b == nullptr)
    {
      b = new Internal(id_);
    }
    b->Draw(dc, r);

  }

  void Bitmap::Cleanup()
  {
    for (const auto& [id,b] : map_) if (b) delete b;
    map_.clear();
  }

}
