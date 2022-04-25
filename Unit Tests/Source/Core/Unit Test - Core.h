// Unit Tests - Core.h: This is a precompiled header file.

#include "..\UnitTest\include\CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "Core.h"
using namespace BoardGamesCore;

#include "MemoryLeaks.h"

// create a lambda that can be called with void(std::stringstream& s) lambdas and executes them, to test serialization:
std::function<std::string(std::function<void(std::stringstream&)>)> TestSerialize = [&](auto f) -> std::string
{
  std::stringstream s;
  f(s);
  return s.str();
};
