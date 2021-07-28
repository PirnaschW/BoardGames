#include "MFC-Win.h"

namespace BoardGamesMFC
{
  
  Bitmap::Bitmap(unsigned int ID) noexcept : ID_(ID), bmP_(new CBitmap()), bmM_(new CBitmap()) {}

  Bitmap::~Bitmap(void) noexcept { delete bmP_; delete bmM_; }

  void Bitmap::Load(DC* pDC, const Rect& r) const noexcept
  {
    assert(bmP_ != nullptr);

    bmP_->LoadBitmap(ID_);
    assert(bmP_->m_hObject != 0);

    BITMAP bm;
    bmP_->GetObject(sizeof(BITMAP), &bm);  // really does 'GetObjectInfo'
    if ((bm.bmWidth != r.Width()) || (bm.bmHeight != r.Height()))
    {
      //throw ("rectangle has wrong size!");
    }

    // Create monochrome (1 bit) mask bitmap.  
    /*auto b = */  bmM_->CreateBitmap(bm.bmWidth, bm.bmHeight, 1U, 1U, NULL);

    CDC* const pCDC = *pDC;

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


  void Bitmap::Draw(DC* pDC, const Rect& r) const noexcept
  {
    CDC* const pCDC = *pDC;
    if (bmP_->m_hObject == 0) Load(pDC, r); // lazy load

    CDC dcMemory;
    dcMemory.CreateCompatibleDC(pCDC);
    CBitmap* MaskOld = dcMemory.SelectObject(bmM_);
    pCDC->BitBlt(r.left_, r.top_, r.Width(), r.Height(), &dcMemory, 0, 0, SRCAND);

    dcMemory.SelectObject(bmP_);
    pCDC->BitBlt(r.left_, r.top_, r.Width(), r.Height(), &dcMemory, 0, 0, SRCPAINT);
    dcMemory.SelectObject(MaskOld);
    dcMemory.DeleteDC();
  }

}
