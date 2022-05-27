namespace BoardGamesCore
{

  enum class LayoutType : unsigned short
  {
    Light       = 1,   // all fields are light colored
    Dark        = 2,   // all fields are dark colored
    Alternating = 3,   // fields are alternating light and dark ('Checkerboard')
    Small       = 4,   // smaller fields (mostly for Stock / Taken)
  //None        = 5,   // Game handles fields individually - currently NOT supportted
  };

  struct BoardPartDimension final // PoD structure to collect a game's layout dimensions
  {
  public:
    constexpr BoardPartDimension(
      Coordinate xc, Coordinate yc,              // tile count in x and y directions
      LayoutType ltype,                          // field background coloring rule
      Pixel le, Pixel te,                        // starting edge in x and y directions
      Pixel xd, Pixel yd,                        // tile size in x and y directions
      Pixel xs = 0, Pixel ys = 0) noexcept :     // extra distance between tiles in x and y directions
      xCount_(xc), yCount_(yc),
      ltype_(ltype),
      xDim_(xd), yDim_(yd),
      xSkip_(xs), ySkip_(ys),
      lEdge_(le), tEdge_(te) {}
    void Serialize(std::stringstream& s) const
    { 
      s.write(static_cast<const char* const>(static_cast<const void*>(this)), sizeof(*this));
    }

  public:
    const Coordinate xCount_;   // tile count in x and y directions
    const Coordinate yCount_;   // tile count in x and y directions
    const LayoutType ltype_;    // field background coloring rule
    const Pixel lEdge_;         // starting edge in x and y directions
    const Pixel tEdge_;         // starting edge in x and y directions
    const Pixel xDim_;          // tile size in x and y directions
    const Pixel yDim_;          // tile size in x and y directions
    const Pixel xSkip_;         // extra distance between tiles in x and y directions
    const Pixel ySkip_;         // extra distance between tiles in x and y directions
  };
  using BoardPartDimensions = std::vector<BoardPartDimension>;  // Stage, Stock, Taken

  class BoardPart final
  {
  private:
    BoardPart() noexcept = delete;

  public:
    constexpr BoardPart(const BoardPartDimension& d, BoardPartID id, PieceIndex init = PMap[Piece::NoPiece]) noexcept
      : dim_(d), id_(id)
    {
      assert(dim_.xCount_ > 0);
      assert(dim_.yCount_ > 0);
      const auto count = dim_.xCount_ * dim_.yCount_;
      fields_.reserve(count);
      for (Pixel i = 0; i < count; ++i)
      {
        Pixel x = i % dim_.xCount_;
        Pixel y = i / dim_.xCount_;

        const TileColor* f{ nullptr };
        switch (dim_.ltype_)
        {
          case LayoutType::Light:       f = &TileColor::Light;                                      break;
          case LayoutType::Dark:        f = &TileColor::Dark;                                       break;
          case LayoutType::Small:       f = &TileColor::Small;                                      break;
          case LayoutType::Alternating: f = ((x + y) % 2) ? &TileColor::Dark : &TileColor::Light;   break;
          default:                      f = &TileColor::Light;                                      break;
        }
        assert(f != nullptr);

        Rect r = {
          dim_.lEdge_ + dim_.xSkip_ * x + dim_.xDim_ * x,
          dim_.tEdge_ + dim_.ySkip_ * y + dim_.yDim_ * y,
          dim_.lEdge_ + dim_.xSkip_ * x + dim_.xDim_ * (x + 1),
          dim_.tEdge_ + dim_.ySkip_ * y + dim_.yDim_ * (y + 1) };

        fields_.emplace_back(Field(r, *f, init));
      }
    }
    constexpr BoardPart(const BoardPart& p) noexcept : dim_(p.dim_), id_(p.id_), fields_{ p.fields_ } {}
    constexpr BoardPart(BoardPart&& p) noexcept : dim_(std::move(p.dim_)), id_(p.id_), fields_{ std::move(p.fields_) } {}
    constexpr BoardPart& operator= (const BoardPart& p) noexcept = delete;
    constexpr BoardPart& operator= (BoardPart&& p) noexcept = delete;
    constexpr ~BoardPart() noexcept = default;

    void Serialize(std::stringstream& s) const { dim_.Serialize(s); for (const auto& f : fields_) f.Serialize(s); }
    constexpr bool operator ==(const BoardPart& p) const noexcept
    { 
      auto predicate = [](const Field& f1, const Field& f2) { return f1 == f2; };
      return p.dim_.xCount_ == dim_.xCount_ && p.dim_.yCount_ == dim_.yCount_ &&
        std::equal(fields_.cbegin(),fields_.cend(), p.fields_.cbegin(), predicate);
    }
    constexpr bool operator !=(const BoardPart& p) const noexcept { return !(*this == p); }
    constexpr BoardPartID GetID() const noexcept { return id_; }

    std::size_t GetHash() const noexcept;
    using ActivityRO = std::function<void(PieceIndex)>;
    using ActivityRW = std::function<PieceIndex(PieceIndex)>;
    void ForEach(const ActivityRO& f) const noexcept; // read-only  loop over all fields
    void ForEach(const ActivityRW& f) noexcept;       // read-write loop over all fields
    void DrawFrame(DC* dc) const;
    void Draw(DC* dc) const { for (const auto& f : fields_) f.Draw(dc); }
    void DrawSelected(DC* dc, Coordinate x, Coordinate y) const { fields_[Index(x, y)].DrawSelected(dc); }

    bool GetLocationFromPoint(const Point& p, Coordinate& x, Coordinate& y) const noexcept;

    constexpr Coordinate GetSizeX() const noexcept { return dim_.xCount_; }  // needed for loops
    constexpr Coordinate GetSizeY() const noexcept { return dim_.yCount_; }  // needed for loops
    constexpr bool IsValid(Coordinate x, Coordinate y) const noexcept   // check if this is a field of the BoardPart
    { return -x <= dim_.xCount_ && x < dim_.xCount_ && -y <= dim_.yCount_ && y < dim_.yCount_; }

    constexpr bool HasPiece(PieceIndex pI) const noexcept { for (const auto& f : fields_) if (f.GetPieceIndex() == pI) return true; return false; }
    constexpr PieceIndex GetPieceIndex(Coordinate x, Coordinate y) const noexcept { assert(IsValid(x, y)); return fields_[Index(x, y)].GetPieceIndex(); }
    constexpr void SetPieceIndex(PieceIndex pI, Coordinate x, Coordinate y) noexcept
    {
      assert(IsValid(x, y));
      hash_ = 0;
      fields_[Index(x, y)].SetPieceIndex(pI);
    }

  private:
    std::size_t Index(Coordinate x, Coordinate y) const noexcept;
    Coordinate GetX(PieceIndex pI) const noexcept { return pI % dim_.xCount_; }
    Coordinate GetY(PieceIndex pI) const noexcept { return pI / dim_.xCount_; }

  private:
    const BoardPartDimension dim_;                 // dimensions of this board part
    const BoardPartID id_;                         // type of this part
    std::vector<Field> fields_{};                  // piece and screen coords of each field 
    mutable std::size_t hash_{};                   // mutable, so it can be lazy-filled
  };

//  static_assert(sizeof(BoardPart) == 72, "size of BoardPart has changed");
  static_assert(BoardPart(BoardPartDimension(3, 1, LayoutType::Light, 0, 0, 0, 0), BoardPartID::Stage, 0).GetSizeX() == 3, "BoardPart is not constexpr");

}
