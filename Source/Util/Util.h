
#include <string>
#include <assert.h>

namespace ClipBoard
{

  bool IsClipboardFormatText(void);
  std::string GetClipboardText(void);

};


namespace URL
{

  const std::string GetHTMLFromURL(const std::wstring& url);
  const std::string GetHTMLFromURL(const std::string& url);

}


namespace Math
{

  consteval unsigned int ipow(unsigned int base, unsigned int exp, unsigned int result = 1)
  {
    return exp < 1 ? result : ipow(base * base, exp / 2, (exp % 2) ? result * base : result);
  }


}