// dependent on Color, Piece

namespace BoardGamesCore
{

  using BoardGamesMFC::Point;
  using BoardGamesMFC::Pen;

  class Field final
  {
  public:
    constexpr Field(const Rect& r, const TileColor& f, PieceIndex pI) noexcept : tilecolor_{ f }, rect_{ r }, pI_{ pI } {}
    constexpr Field(const Field&) = default;            // copy constructor
    constexpr Field& operator=(const Field&) = delete;  // copy assignment
    constexpr Field(Field&&) = default;                 // move constructor
    constexpr Field& operator=(Field&&) = delete;       // move assignment
    constexpr ~Field() = default;

    // Note: a field is considered 'equal' if the same Piece is on it!
    constexpr bool operator ==(const Field& f) const noexcept { return f.pI_ == pI_; }
    constexpr bool operator !=(const Field& f) const noexcept = default;

    void Serialize(std::stringstream& s) const { PMap[pI_].Serialize(s); }
    constexpr PieceIndex GetPieceIndex() const noexcept { return pI_; }
    constexpr void SetPieceIndex(PieceIndex pI) noexcept { pI_ = pI; }
    constexpr bool IsInRect(const Point& p) const noexcept { return rect_.PtInRect(p); }
    void Draw(DC* dc) const
    {
      const auto& p = PMap[pI_];
      if (p == Piece::NoTile) return;
      tilecolor_.Draw(dc, rect_);
      if (p == Piece::NoPiece) return;
      p.Draw(dc, rect_);
    }
    void DrawSelected(DC* dc) const
    {
      Rect r = rect_;
      r.DeflateRect(1, 1, 1, 1);
      dc->Rectangle(r, Pen::PenSelected);
    }

  private:
    PieceIndex pI_;                 // piece that is currently on this field
    const TileColor& tilecolor_;    // tile background color
    const Rect rect_;               // tile location on screen
  };

//  static_assert(sizeof(Field) == 32, "size of Field has changed");
  static_assert(!std::is_abstract<Field>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Field>::value, "must not be trivially constructible");
  static_assert(std::is_constructible<class Field, Rect&, TileColor&, PieceIndex>::value, "is not constructible");
  static_assert(std::is_nothrow_constructible<class Field, Rect&, TileColor&, PieceIndex>::value, "is not nothrow constructible");
  static_assert(std::is_copy_constructible<class Field>::value, "is copy constructible");
  static_assert(std::is_move_constructible<class Field>::value, "is move constructible");
  static_assert(!std::is_copy_assignable<class Field>::value, "is copy assignable");
  static_assert(!std::is_move_assignable<class Field>::value, "is move assignable");
  static_assert(Field(Rect(0, 0, 0, 0), TileColor::Light, 3).GetPieceIndex() == 3, "Field is not constexpr");

}