
class CArchive;

namespace BoardGamesMFC
{

  class Archive : public std::stringstream
  {
  public:
    Archive(CArchive* ar) noexcept : ar_(ar) {}

    bool IsLoading() const; // This code must be outside the header, as during header compilation, CArchive doesn't yet know it's methods.
    bool IsStoring() const { return !IsLoading(); }

    // methods: save (<<) and load (>>) any POD
    template<typename T>
    Archive& operator << (const T& t)
    {
      static_assert(std::is_fundamental_v<T> || std::is_standard_layout_v<T>, "Only primitives and PODs can be serialized");
      const std::byte* p = static_cast<const std::byte*>(static_cast<const void*>(std::addressof(t)));
      for (size_t z = 0U; z < sizeof(T); z++) SaveByte(p[z]);
      return *this;
    };

    template<typename T>
    Archive& operator >> (T& t)
    {
      static_assert(std::is_fundamental_v<T> || std::is_standard_layout_v<T>, "Only primitives and PODs can be serialized");
      std::byte* p = static_cast<std::byte*>(static_cast<void*>(std::addressof(t)));
      for (size_t z = 0U; z < sizeof(T); z++) p[z] = LoadByte();
      return *this;
    };

    template<typename T>
    void Serialize(T& t)
    {
      static_assert(std::is_fundamental_v<T> || std::is_standard_layout_v<T>, "Only primitives and PODs can be serialized");
      IsStoring() ? *this << t : *this >> t;
    }

  private:
    // Load and Save through CArchive.
    // This code must be outside the header, as during header compilation, CArchive doesn't yet know it's methods.
    void SaveByte(const std::byte b) const;
    std::byte LoadByte() const;

  private:
    CArchive* ar_;  // just store it so we can use it, we do NOT own it
  };

}
