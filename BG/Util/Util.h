
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