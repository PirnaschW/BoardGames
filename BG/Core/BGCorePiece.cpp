#include "pch.h"

namespace BoardGamesCore
{

  const Piece Piece::NoTile { noKind::NoKind, PieceColor::Void,    0 };   // nothing exists there, not even a tile
  const Piece Piece::NoPiece{ noKind::NoKind, PieceColor::NoColor, 0 };   // empty tile

  void Piece::CollectMoves(const MainPosition& p, const Location& l, Moves& m) const
  {
    kind_.CollectMoves(p, l, m);
  }

  size_t Piece::GetHash(void) const noexcept
  { 
    return kind_.GetHash() + color_.GetHash();
  }
  
  bool Piece::IsKind(const Kind& k) const noexcept
  { 
    return k == kind_;
  }
  bool Piece::IsColor(const PieceColor& c) const noexcept
  { 
    return c == color_;
  }

  bool Piece::IsBlank(void) const noexcept
  {
    return color_ == PieceColor::NoColor && kind_ == noKind::NoKind;
  }

  inline void Piece::Serialize(CArchive* ar) const
  { 
    color_.Serialize(ar);
    kind_.Serialize(ar);
  }
  
  inline unsigned int Piece::GetValue(const MainPosition& p, const Location& l) const noexcept
  { 
    return kind_.GetValue(p, l);
  }

  bool Piece::CanTake(const Piece& p) const noexcept
  {
    return kind_.CanTake(p.kind_);
  }

  void Piece::Draw(CDC* pDC, const CRect& r) const
  {
    assert(ID_ != 0);

    if (bmP_.m_hObject == 0) // lazy load
    {
      bmP_.LoadBitmap(ID_);
      assert(bmP_.m_hObject != 0);

      BITMAP bm;
      bmP_.GetObject(sizeof(BITMAP), &bm);  // really does 'GetObjectInfo'
      if ((bm.bmWidth != r.Width()) || (bm.bmHeight != r.Height()))
      {
        //throw ("rectangle has wrong size!");
      }

      // Create monochrome (1 bit) mask bitmap.  
      auto b = bmM_.CreateBitmap(bm.bmWidth, bm.bmHeight, 1U, 1U, NULL);

      // Get compatible CDC for mask calculation
      CDC CDCImage;
      CDCImage.CreateCompatibleDC(pDC);
      CBitmap* Imageold = CDCImage.SelectObject(&bmP_);
      // Set the background color of the image to the color to be transparent.
      CDCImage.SetBkColor(COLORREF(0x00FF00FF));

      CDC CDCMask;
      CDCMask.CreateCompatibleDC(pDC);
      CBitmap* Maskold = CDCMask.SelectObject(&bmM_);
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

    CDC dcMemory;
    dcMemory.CreateCompatibleDC(pDC);
    CBitmap* MaskOld = dcMemory.SelectObject(&bmM_);
    pDC->BitBlt(r.TopLeft().x, r.TopLeft().y, r.Width(), r.Height(), &dcMemory, 0, 0, SRCAND);

    dcMemory.SelectObject(&bmP_);
    pDC->BitBlt(r.TopLeft().x, r.TopLeft().y, r.Width(), r.Height(), &dcMemory, 0, 0, SRCPAINT);
    dcMemory.SelectObject(MaskOld);
    dcMemory.DeleteDC();
  }

  const std::unordered_map<std::string, const Piece&>& Piece::GetHTMLPieceMap(void) noexcept
  {
    static const std::unordered_map<std::string, const Piece&> map_{
      { R"(blank35)",     Piece::NoPiece },
      //{ R"(chess/wk35)",BoardGamesChessPieces::ChessPiece::WK },
      //{ R"(chess/wq35)",BoardGamesChessPieces::ChessPiece::WQ },
      //{ R"(chess/wr35)",BoardGamesChessPieces::ChessPiece::WR },
      //{ R"(chess/wb35)",BoardGamesChessPieces::ChessPiece::WB },
      //{ R"(chess/wn35)",BoardGamesChessPieces::ChessPiece::WN },
      //{ R"(chess/wp35)",BoardGamesChessPieces::ChessPiece::WP },
      //{ R"(chess/bk35)",BoardGamesChessPieces::ChessPiece::BK },
      //{ R"(chess/bq35)",BoardGamesChessPieces::ChessPiece::BQ },
      //{ R"(chess/br35)",BoardGamesChessPieces::ChessPiece::BR },
      //{ R"(chess/bb35)",BoardGamesChessPieces::ChessPiece::BB },
      //{ R"(chess/bn35)",BoardGamesChessPieces::ChessPiece::BN },
      //{ R"(chess/bp35)",BoardGamesChessPieces::ChessPiece::BP },
      //{ R"(hshogi/w35)",Hasami::HasamiPiece::HasamiPieceW },
      //{ R"(hshogi/b35)",Hasami::HasamiPiece::HasamiPieceB },
    };
    return map_;
  }

  std::vector<const Piece*> Piece::ListFromHTML(std::string s, const std::unordered_map<std::string, const Piece&>& map_)
  {
    static const std::string s0{ R"(<div id="game-board-section">)" };
    static const std::string s1{ R"(<img src=https://i.brainking.com/)" };
    static const std::string s2{ R"(.gif)" };
    static const std::string s9{ R"(<div id="game-info-section">)" };

    std::vector<const Piece*> list{};

    std::string::size_type p1{ s.find(s0) };
    std::string::size_type p9{ s.find(s9,p1) };
    while ((p1 = s.find(s1, p1)) < p9)
    {
      p1 += s1.size();
      auto p2 = s.find(s2, p1);
      const auto& it{ map_.find(s.substr(p1, p2 - p1)) };
      if (it != map_.end()) list.push_back(&(it->second));
      else;// throw s.substr(p1, p2 - p1);
    }
    return list;
  }

}