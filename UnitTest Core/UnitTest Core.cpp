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

      template<> inline static std::wstring ToString<Location>(const Location& l) { std::wstringstream _s; _s << '(' << l. _x << '|' << l. _y << ')'; return _s.str(); }
      template<> inline static std::wstring ToString<Location>(const Location* l) { std::wstringstream _s; _s << '(' << l->_x << '|' << l->_y << ')'; return _s.str(); }
      template<> inline static std::wstring ToString<Location>(      Location* l) { std::wstringstream _s; _s << '(' << l->_x << '|' << l->_y << ')'; return _s.str(); }

      template<> inline static std::wstring ToString<Field>(const Field& f) { std::wstringstream _s; _s << ToString(f. GetLocation()) << ',' << static_cast<const void*>(f. GetPiece()); return _s.str(); }
      template<> inline static std::wstring ToString<Field>(const Field* f) { std::wstringstream _s; _s << ToString(f->GetLocation()) << ',' << static_cast<const void*>(f->GetPiece()); return _s.str(); }
      template<> inline static std::wstring ToString<Field>(      Field* f) { std::wstringstream _s; _s << ToString(f->GetLocation()) << ',' << static_cast<const void*>(f->GetPiece()); return _s.str(); }

      template<> inline static std::wstring ToString<Fields>(const Fields& f) { std::wstringstream _s; _s << f. size(); return _s.str(); }
      template<> inline static std::wstring ToString<Fields>(const Fields* f) { std::wstringstream _s; _s << f->size(); return _s.str(); }
      template<> inline static std::wstring ToString<Fields>(      Fields* f) { std::wstringstream _s; _s << f->size(); return _s.str(); }

      template<> inline static std::wstring ToString<>(const Piece& p) { std::wstringstream _s; _s << static_cast<const void*>(&p); return _s.str(); }
      template<> inline static std::wstring ToString<>(const Piece* p) { std::wstringstream _s; _s << static_cast<const void*>( p); return _s.str(); }
      template<> inline static std::wstring ToString<>(      Piece* p) { std::wstringstream _s; _s << static_cast<const void*>( p); return _s.str(); }

      template<> inline static std::wstring ToString<>(const Step::StepType& s) { std::wstringstream _s; _s << s; return _s.str(); }
      template<> inline static std::wstring ToString<>(const Step::StepType* s) { std::wstringstream _s; _s << s; return _s.str(); }
      template<> inline static std::wstring ToString<>(      Step::StepType* s) { std::wstringstream _s; _s << s; return _s.str(); }

      template<> inline static std::wstring ToString<>(const SimpleStep& s) { std::wstringstream _s; _s << ToString(s. GetFr()) << ToString(s. GetTo()) << s. GetType(); return _s.str(); }
      template<> inline static std::wstring ToString<>(const SimpleStep* s) { std::wstringstream _s; _s << ToString(s->GetFr()) << ToString(s->GetTo()) << s->GetType(); return _s.str(); }
      template<> inline static std::wstring ToString<>(      SimpleStep* s) { std::wstringstream _s; _s << ToString(s->GetFr()) << ToString(s->GetTo()) << s->GetType(); return _s.str(); }

      template<> inline static std::wstring ToString<>(const ComplexStep& s) { std::wstringstream _s; _s << ToString(s. GetFr()) << ToString(s. GetTo()) << s. GetType(); return _s.str(); }
      template<> inline static std::wstring ToString<>(const ComplexStep* s) { std::wstringstream _s; _s << ToString(s->GetFr()) << ToString(s->GetTo()) << s->GetType(); return _s.str(); }
      template<> inline static std::wstring ToString<>(      ComplexStep* s) { std::wstringstream _s; _s << ToString(s->GetFr()) << ToString(s->GetTo()) << s->GetType(); return _s.str(); }

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
      Offset o1{ 3, 5 };
      Offset o2 = o1 * 3;
      Offset o3{ 9,15 };
      Assert::AreEqual(o2,o3);
		}

    TEST_METHOD(TestLocation)
    {
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

    TEST_METHOD(TestField)
    {
      Location l1(2U, 3U);
      Location l2(3U, 2U);
      const Piece* p0{ nullptr };
      Field f1{ l1,p0 };
      Field f2{ l2,p0 };
      Assert::AreEqual(l1, f1.GetLocation());
      Assert::AreEqual(p0, f1.GetPiece());
      Assert::AreNotEqual(f1,f2);
      Assert::AreEqual(l2, f2.GetLocation());
    }

    TEST_METHOD(TestSimpleStep)
    {
      Location l1(2U, 3U);
      Location l2(3U, 2U);
      const Piece* p0{ nullptr };
      Field f1{ l1,p0 };
      Field f2{ l2,p0 };
      Assert::AreNotEqual(f1, f2);
      SimpleStep s1{ f1,f2 };
      Assert::AreEqual(f1, s1.GetFr());
      Assert::AreEqual(f2, s1.GetTo());
      Assert::AreNotEqual(f2, s1.GetFr());
      Assert::AreNotEqual(f1, s1.GetTo());
      Assert::AreEqual(Step::StepType::Normal, s1.GetType());
      Assert::IsFalse(s1.IsTake());

      Step::StepType st1 = static_cast<Step::StepType>(Step::StepType::Take | Step::StepType::Jump);
      SimpleStep s2{ f2,f1, st1 };
      Assert::AreNotEqual(s1, s2);
      Assert::AreEqual(st1, s2.GetType());
      Assert::AreNotEqual(Step::StepType::Normal, s2.GetType());
      Assert::IsTrue(s2.IsTake());
      Assert::IsTrue(s1 != s2);
      Assert::AreEqual(Fields{ f1 }, s2.GetTakes());
      Assert::AreEqual(s2.GetTakes().size(), 1U);
      Assert::AreEqual(s2.GetTakes()[0], f1);
    }

    TEST_METHOD(TestComplexStep)
    {
      Location l1(2U, 3U);
      Location l2(3U, 2U);
      const Piece* p0{ nullptr };
      Field f1{ l1,p0 };
      Field f2{ l2,p0 };
      Assert::AreNotEqual(f1, f2);
      ComplexStep s1{ f1,f2 };
      Assert::AreEqual(f1, s1.GetFr());
      Assert::AreEqual(f2, s1.GetTo());
      Assert::AreNotEqual(f2, s1.GetFr());
      Assert::AreNotEqual(f1, s1.GetTo());
      Assert::AreEqual(Step::StepType::Normal, s1.GetType());
      Assert::IsFalse(s1.IsTake());

      Step::StepType st1 = static_cast<Step::StepType>(BoardGamesCore::Step::StepType::Take | BoardGamesCore::Step::StepType::Jump);
      ComplexStep s2{ f2,f1, st1 };
      Assert::AreNotEqual(s1, s2);
      Assert::AreEqual(st1, s2.GetType());
      Assert::AreNotEqual(Step::StepType::Normal, s2.GetType());
      Assert::IsTrue(s2.IsTake());
      Assert::IsTrue(s1 != s2);
      Assert::AreEqual(Fields{ f1 }, s2.GetTakes());
      Assert::AreEqual(s2.GetTakes().size(), 1U);
      Assert::AreEqual(s2.GetTakes()[0], f1);

    }

  };
}
