
namespace ClipBoard
{

  std::string GetClipboardText(void);

};

namespace URL
{

  const std::string GetHTMLFromURL(const std::wstring& url);
  inline const std::string GetHTMLFromURL(const std::string& url) { return GetHTMLFromURL(std::wstring(url.begin(), url.end())); }

}
