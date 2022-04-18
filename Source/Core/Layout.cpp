#include "Core.h"

using namespace BoardGamesMFC;

namespace BoardGamesCore
{

  //void Tile::Draw(DC* pDC, const Piece& p) const
  //{
  //  if (p == Piece::NoTile) return;
  //  tilecolor_.Draw(pDC, rect_);
  //  if (p == Piece::NoPiece) return;
  //  p.Draw(pDC, rect_);
  //}

  Layout::Layout(const BoardPartDimension& d, const BoardPartID, const LayoutType lt) noexcept : dim_(d), ltype_(lt), tiles_{ 1ULL * d.xCount_*d.yCount_,nullptr }
  {
    if (ltype_ != LayoutType::None)  // None means the Game is handling its own tile creation
    {
      unsigned int z = 0;
      for (Coordinate i = 0; i < dim_.xCount_; i++)
        for (Coordinate j = 0; j < dim_.yCount_; j++, z++)
        {
          const TileColor* f{nullptr};
          switch (ltype_)
          {
            case LayoutType::Light:       f = &TileColor::Light;                                     break;
            case LayoutType::Dark:        f = &TileColor::Dark;                                      break;
            case LayoutType::Small:       f = &TileColor::Small;                                     break;
            case LayoutType::Alternating: f = ((i + j) % 2) ? &TileColor::Dark : &TileColor::Light;  break;
            default:                      f = &TileColor::Light;                                     break;
          }
          assert(f != nullptr);

          const Rect r{
            (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * i),
            (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * j),
            (int)(dim_.lEdge_ + dim_.xSkip_ * i + dim_.xDim_ * (i + 1U)),
            (int)(dim_.tEdge_ + dim_.ySkip_ * j + dim_.yDim_ * (j + 1U)) };
          (tiles_)[z] = new Tile(Location(b, i, j), r, *f);
        }
    }
  }

  void Layout::Draw(DC* dc, const BoardPart* board_, const PieceMapP& pMap, Mode /*mode_*/) const
  {
    for (const auto& t : tiles_)
    {
      const Location l{ t->GetLocation() };
      const PieceIndex pI = board_->GetPieceIndex(l.x_, l.y_);
      t->Draw(dc, pMap->GetPieceIndex(pI));
    }
  }

  void Layout::DrawSelected(DC* dc, const Location& l) const
  {
    for (auto& t : tiles_)
      if (t->GetLocation() == l)
      {
        Rect r = t->GetRect();
        r.DeflateRect(1, 1, 1, 1);
        dc->Rectangle(r, Pen::PenSelected);
      }
  }

  bool Layout::GetLocationFromPoint(const Point& p, Location& l) const noexcept
  {
    for (auto& t : tiles_)
      if (t->IsInRect(p)) {
        l = t->GetLocation();
        return true;
      }
    return false;
  }

  void MainLayout::Draw(DC* dc, const Board* board_, Mode mode_) const
  {
    // frame around the board (needs to be drawn first)
    for (unsigned int z = 4; z > 0; z--)
    {
      if (z != 2)
        dc->Rectangle(Rect{ (int) (dim_.lEdge_ - z), (int) (dim_.tEdge_ - z), (int) (dim_.lEdge_ + dim_.xCount_ * (dim_.xDim_ + dim_.xSkip_) + z), (int) (dim_.tEdge_ + dim_.yCount_ * (dim_.yDim_ + dim_.ySkip_) + z) });
    }
    Layout::Draw(dc, &(board_->board_), board_->pMap_, mode_);

    if (mode_.IsSet(Mode::ShowStock) || mode_.IsSet(Mode::Editing))
    {
      stock_.Draw(dc, &(board_->stock_), board_->pMap_, mode_);
    }
    taken_.Draw(dc, &(board_->taken_), board_->pMap_, mode_);
  }



}
