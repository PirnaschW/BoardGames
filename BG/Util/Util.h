
namespace ClipBoard
{

  std::string GetClipboardText(void);

};

namespace URL
{

  const std::string GetHTMLFromURL(const std::wstring& url);
  inline const std::string GetHTMLFromURL(const std::string& url) { return GetHTMLFromURL(std::wstring(url.begin(), url.end())); }

}

namespace Example
{
  class XX {
  public:
    // ...
    virtual ~XX() = default;             // virtual destructor as class is a base class
    XX(const XX&) = default;             // copy constructor
    XX& operator=(const XX&) = default;  // copy assignment
    XX(XX&&) = default;                  // move constructor
    XX& operator=(XX&&) = default;       // move assignment
  };
}

namespace Math
{
  constexpr unsigned int ipow(unsigned int base, unsigned int exp, unsigned int result = 1)
  {
    return exp < 1 ? result : ipow(base * base, exp / 2, (exp % 2) ? result * base : result);
  }
}