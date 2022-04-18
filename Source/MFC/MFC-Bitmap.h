
class CBitmap;
class Internal;

namespace BoardGamesMFC
{
  using BitmapID = unsigned int;

  class Bitmap
  {
    class Internal; // decoupled helper class to do the work
  private:
    constexpr Bitmap() = delete;
  public:
    constexpr Bitmap(BitmapID id) noexcept : id_(id) {}
    constexpr ~Bitmap() noexcept = default;
    void Draw(DC* dc, const Rect& r) const;

    static void Cleanup();

  private:
    const BitmapID id_{};

  private:  // collect all internal objects ina static map to allow decoupling and constexpr Bitmap objects
    static inline std::unordered_map<BitmapID, Internal*> map_{};
  };

}
