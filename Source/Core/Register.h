
#include <map>
#include <memory>



template <class Base, class... Args>
class Factory {
public:
  template <class T>
  class Registrar : Base
  {
  public:
    friend T;
    static bool registerT() {
      const auto name = T::name;
      Factory::data()[name] = [](Args... args) -> Base* { return new T(std::forward<Args>(args)...); };
      return true;
    }
    static bool registered;

  private:
    Registrar() : Base(Key{}) { (void)registered; }
  };

public:
  class Key {
    Key() {};
    template <class T> friend class Registrar;
  };

public:
  Factory() = default;
  using FuncType = Base*(*)(Args...);
  static auto& data() {
    static std::map<std::string, FuncType> s;
    return s;
  }
};

// The really fun part
template <class Base, class... Args>
template <class T>
bool Factory<Base, Args...>::Registrar<T>::registered = Factory<Base, Args...>::Registrar<T>::registerT();






//
//
//
//
//
//
//
//
//class myGame : Factory<myGame, int> {
//  Animal(Key) {}
//  virtual void makeNoise() = 0;
//  virtual ~Animal() = default;
//};
//
//
//using namespace Core;
//
//static std::map<int, std::unique_ptr<Game>(*)(int)> GameList{};
//template<class T>
//class Register
//{
//private:
//  static T t;
//
//};

