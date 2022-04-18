
namespace BoardGamesCore
{

  class PieceMap final
  {
  public:
    constexpr PieceMap() noexcept = default;
    constexpr ~PieceMap() noexcept = default;

    constexpr PieceIndex Register(const Piece& p) noexcept            // register a piece
    { 
      // skip double registration
      auto pI = GetIndex_(p);
      if (pI != Invalid) return pI;
      map_[used_] = &p;
      return used_++;
    }
    constexpr const Piece& operator[](PieceIndex pI) const noexcept    // retrieve a piece
    {
      assert(pI != Invalid);
      return GetPiece_(pI);
    }
    constexpr PieceIndex operator[](const Piece& p) const noexcept    // find a piece' index in the map
    {
      auto pI = GetIndex_(p);
      assert(pI != Invalid); // used invalid piece!
      return pI;
    }
    PieceIndex Find(std::stringstream& s) const noexcept             // find a piece from its serialization
    {
      char kind;
      char color;
      s >> std::noskipws >> kind >> color;
      for (PieceIndex pI = 1; pI < used_; pI++)
        if (map_[pI]->IsPiece(kind, color)) return pI;
      assert(false); // used invalid piece!
      return Invalid;
    }
    constexpr PieceIndex size() const noexcept { return used_; }
    [[TODO::Unittest]] constexpr bool IsColor(PieceIndex pI, const PieceColor& c) const noexcept { return this->operator[](pI).IsColor(c); }

  private:
    constexpr const Piece& GetPiece_(PieceIndex pI) const noexcept    // retrieve a piece
    {
      assert(pI != Invalid);
      return *map_[pI];
    }
    constexpr PieceIndex GetIndex_(const Piece& p) const noexcept     // find a piece in the map
    {
      for (PieceIndex pI = 1; pI < used_; pI++)
        if (map_[pI] == &p) return pI;
       return Invalid;  // not a valid Piece
    }

  public:
    constexpr static const PieceIndex Invalid{ 0 };     // used for invalid index (not found, etc)

  private:
    constexpr static const size_t MaxPieces{ 64 };      // all games together have at max that many different Pieces
    PieceIndex used_{ 1 };                              // current usage of the map. 0 stays unused; 0 is NOT a valid PieceIndex
    std::array<const Piece*, MaxPieces> map_{nullptr};  // maps index -> Piece*

  private:
    static PieceMap PMap;
  };

  inline PieceMap PMap{};

  static_assert(PieceMap().size() == 1, "PieceMaps can't be constexpr");
  static_assert(PieceMap().Register(Piece::NoPiece) == 1, "PieceMaps can't be constexpr");

}
