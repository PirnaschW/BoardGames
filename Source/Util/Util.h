
#include <string>
#include <algorithm>
#include <random>

#include <assert.h>

namespace ClipBoard
{

  bool IsClipboardFormatText();
  std::string GetClipboardText();

};


namespace URL
{

  struct Cookie {
    std::string url{};
    std::string name{};
    std::string value{};
  };
  using Cookies = std::vector<Cookie>;

  const std::string GetHTMLFromURL(const std::string& url, const Cookies& c);
  const std::string GetHTMLFromURL(const std::string& url);

}


namespace Math
{

  consteval unsigned int ipow(unsigned int base, unsigned int exp, unsigned int result = 1)
  {
    return exp < 1 ? result : ipow(base * base, exp / 2, (exp % 2) ? result * base : result);
  }

  namespace
  {
    static std::mt19937 rnd;  //    static auto rnd = std::mt19937();
    static std::uniform_int_distribution<> d6(1, 6);
  }
  
  template<class T> void Shuffle(std::vector<T>& v) { std::ranges::shuffle(v, rnd); }

  inline int D6() { return d6(rnd); }


}