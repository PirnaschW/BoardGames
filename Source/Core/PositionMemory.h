
namespace BoardGamesCore
{

  template <class T>
  class PositionMemory
  {
  public:
    PositionMemory(auto& preinsert) : preinsert_{ preinsert } {}
    ~PositionMemory() { for (auto& b : objList_) delete b.first; }

  public:
    size_t GetSize() const noexcept { return objList_.size(); }
    T* FindPos(size_t h)
    {
      const auto it = posList_.find(h);
      return it == posList_.end() ? nullptr : it->second;   // return the version found in the map
    }

    void Insert(T* t)   // unconditionally add to maps
    {
      preinsert_(t);
      auto pHash{ std::hash<T>()(*t) };
      auto entry1 = objList_.insert({ t,pHash });
      auto entry2 = posList_.insert({ pHash,t });
      assert(entry1.second);
      assert(entry2.second);
    }

    T* InsertOrUpdate(T* t)  // transfers ownership of t to us!
    {
      // check if position exists
      T* pos = FindPos(std::hash<T>()(*t));
      if (pos)       // it's an existing position, reuse
      {
        delete t;    // we own t, so clean up
        return pos;  // return the buffered position found in the map
      }

      // it's a new position
      Insert(t);
      return t;
    }

    void Purge(const auto& purge) // forget all pos with purge() = true
    {
      for (auto it = objList_.begin(); it != objList_.end();)
      {
        T* p = it->first;
        if (purge(p))
        {
          const size_t z = posList_.erase(it->second);
          assert(z == 1);
          it = objList_.erase(it);
          delete p;
        }
        else it++;
      }
    }

  private:
    std::unordered_map<T*, size_t> objList_{};
    std::unordered_map<size_t, T*> posList_{};
    std::function<void(T*)> preinsert_;
  };

}
