#include "pch.h"

namespace BoardGamesCore
{

  void PieceColor::Serialize(CArchive* ar) const { *ar << color_; }

  constexpr const PieceColor PieceColor::Void   { 'X' };
  constexpr const PieceColor PieceColor::NoColor{ ' ' };
  constexpr const PieceColor PieceColor::White  { 'W' };
  constexpr const PieceColor PieceColor::Black  { 'B' };

  
  const TileColor TileColor::Light { 'L', IDB_XXL };  // normal size light tile
  const TileColor TileColor::Dark  { 'D', IDB_XXD };  // normal size dark  tile
  const TileColor TileColor::Small { 'l', IDB_XXS };  // small  size light tile

  void TileColor::Serialize(CArchive* ar) const { *ar << tilecolor_; }

  void TileColor::Draw(CDC* pDC, const CRect& r) const
  {
    assert(ID_ != 0);

    if (ID_ == 0) return;  // ignore draw request for 'No'Tiles
    if (cb_.m_hObject == 0) cb_.LoadBitmap(ID_);
    if (cb_.m_hObject != 0)
    {
      BITMAP bm;
      cb_.GetObject(sizeof(BITMAP), &bm);
      if ((bm.bmWidth != r.Width()) || (bm.bmHeight != r.Height()))
      {
        //throw ("rectangle has wrong size!");
      }
    }

    CDC dcMemory;
    dcMemory.CreateCompatibleDC(pDC);
    dcMemory.SelectObject(&cb_);
    pDC->BitBlt(r.TopLeft().x, r.TopLeft().y, r.Width(), r.Height(), &dcMemory, 0, 0, SRCCOPY);
  }

}
