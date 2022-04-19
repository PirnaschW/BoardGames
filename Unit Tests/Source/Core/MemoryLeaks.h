#include <crtdbg.h>

class CheckForMemoryLeaks
{
public:
  inline CheckForMemoryLeaks() noexcept
  {
    _CrtMemCheckpoint(&state1);
  }
  ~CheckForMemoryLeaks() noexcept
  {
    _CrtMemCheckpoint(&state2);
    if (_CrtMemDifference(&state3, &state1, &state2))
    {
      _CrtMemDumpStatistics(&state3);
      Assert::Fail();
    }
  }
private:
  _CrtMemState state1;
  _CrtMemState state2;
  _CrtMemState state3;
};
