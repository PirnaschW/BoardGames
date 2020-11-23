#include <memory>

template<typename T>
class Pimpl
{
private:
  std::unique_ptr<T> p_;
public:
  Pimpl();
  template<typename ...Args> Pimpl(Args&& ...);
  ~Pimpl();
  T* operator->();
  T& operator*();
};
