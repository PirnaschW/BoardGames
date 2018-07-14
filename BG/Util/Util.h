
namespace Util
{
  class URL
  {
  public:
    static const std::string GetHTMLFromURL(const std::string& url) { return GetHTMLFromURL(std::wstring(url.begin(), url.end())); }
    static const std::string GetHTMLFromURL(const std::wstring& url);
  };
}