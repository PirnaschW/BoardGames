#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BoardGamesCore;

namespace Microsoft
{
  namespace VisualStudio
  {
    namespace CppUnitTestFramework
    {
      template<> inline static std::wstring ToString<Offset>(const Offset& o) { std::wstringstream _s; _s << "<cannotshow internal data of q>"; return _s.str(); }
      template<> inline static std::wstring ToString<Offset>(const Offset* o) { std::wstringstream _s; _s << "<cannotshow internal data of q>"; return _s.str(); }
      template<> inline static std::wstring ToString<Offset>(      Offset* o) { std::wstringstream _s; _s << "<cannotshow internal data of q>"; return _s.str(); }

      template<> inline static std::wstring ToString<Location>(const Location& l) { std::wstringstream _s; _s << '(' << l._x << '|' << l._y << ')'; return _s.str(); }
      template<> inline static std::wstring ToString<Location>(const Location* l) { std::wstringstream _s; _s << '(' << l->_x << '|' << l->_y << ')'; return _s.str(); }
      template<> inline static std::wstring ToString<Location>(      Location* l) { std::wstringstream _s; _s << '(' << l->_x << '|' << l->_y << ')'; return _s.str(); }

      //template<> inline static std::wstring ToString<Coordinate>(const Coordinate& c) { std::wstringstream _s; _s << '(' <<  c << ')'; return _s.str(); }
      //template<> inline static std::wstring ToString<Coordinate>(const Coordinate* c) { std::wstringstream _s; _s << '(' << *c << ')'; return _s.str(); }
      //template<> inline static std::wstring ToString<Coordinate>(      Coordinate* c) { std::wstringstream _s; _s << '(' << *c << ')'; return _s.str(); }
    }
  }
}

namespace UnitTestCore
{
  TEST_CLASS(UnitTestCore)
	{
	public:
		TEST_METHOD(TestOffset)
		{
      static_assert(!std::is_trivially_constructible<class Offset>::value, "must not be trivially constructible");
      static_assert(std::is_constructible<class Offset, int, int>::value, "is not constructible");
      static_assert(std::is_nothrow_constructible<class Offset, int, int>::value, "is not nothrow constructible");
      Offset o1{ 3, 5 };
      Offset o2 = o1 * 3;
      Offset o3{ 9,15 };
      Assert::AreEqual(o2,o3);
		}
    TEST_METHOD(TestLocation)
    {
      static_assert(!std::is_trivially_constructible<class Location>::value, "must not be trivially constructible");
      static_assert(!std::is_constructible<class Location, int, int>::value, "must not be 'int' constructible");
      static_assert(std::is_constructible<class Location, Coordinate, Coordinate>::value, "is not constructible");
      static_assert(std::is_nothrow_constructible<class Offset, Coordinate, Coordinate>::value, "is not nothrow constructible");
      Offset o1{ 3, 5 };
      Offset o2 = o1 * 3;
      Location l1(2U, 3U);
      Location l2 = l1 + o1;
      Location l3(5U, 8U);
      Location l4 = l1 + o2;
      Location l5(11U, 18U);
      Location l6 = l1 + o1 * 3;
      Assert::IsTrue(l1.Valid(16, 16));
      Assert::IsTrue(l2.Valid(16, 16));
      Assert::IsTrue(l3.Valid(16, 16));
      Assert::IsFalse(l4.Valid(16, 16));
      Assert::IsFalse(l5.Valid(16, 16));
      Assert::IsFalse(l6.Valid(16, 16));
      Assert::AreEqual(l4, l5);
      Assert::AreEqual(l6, l5);
      Assert::AreNotEqual(l3, l5);
      Assert::AreEqual(l1.Index(16, 16), 3U * 16U + 2U);
      Assert::AreEqual(l2.Index(16, 16), 8U * 16U + 5U);
      Assert::AreEqual(l3.Index(16, 16), 8U * 16U + 5U);
      Assert::AreEqual(l4.Index(16, 16), 18U * 16U + 11U);
      Assert::AreEqual(l5.Index(16, 16), 18U * 16U + 11U);
      Assert::AreEqual(l6.Index(16, 16), 18U * 16U + 11U);
    }
  };
}
