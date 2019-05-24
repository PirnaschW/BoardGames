#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BoardGamesCore;

namespace Microsoft
{
  namespace VisualStudio
  {
    namespace CppUnitTestFramework
    {
      template<> inline std::wstring ToString<Offset>(const Offset& /*o*/) { std::wstringstream _s; _s << "<cannotshow internal data of q>"; return _s.str(); }
      template<> inline std::wstring ToString<Offset>(const Offset* /*o*/) { std::wstringstream _s; _s << "<cannotshow internal data of q>"; return _s.str(); }
      template<> inline std::wstring ToString<Offset>(      Offset* /*o*/) { std::wstringstream _s; _s << "<cannotshow internal data of q>"; return _s.str(); }

      template<> inline std::wstring ToString<Location>(const Location& l) { std::wstringstream _s; _s << '(' << l. _x << '|' << l. _y << ')'; return _s.str(); }
      template<> inline std::wstring ToString<Location>(const Location* l) { std::wstringstream _s; _s << '(' << l->_x << '|' << l->_y << ')'; return _s.str(); }
      template<> inline std::wstring ToString<Location>(      Location* l) { std::wstringstream _s; _s << '(' << l->_x << '|' << l->_y << ')'; return _s.str(); }

      template<> inline std::wstring ToString<Field>(const Field& f) { std::wstringstream _s; _s << ToString(f. GetLocation()) << ',' << static_cast<const void*>(f. GetPiece()); return _s.str(); }
      template<> inline std::wstring ToString<Field>(const Field* f) { std::wstringstream _s; _s << ToString(f->GetLocation()) << ',' << static_cast<const void*>(f->GetPiece()); return _s.str(); }
      template<> inline std::wstring ToString<Field>(      Field* f) { std::wstringstream _s; _s << ToString(f->GetLocation()) << ',' << static_cast<const void*>(f->GetPiece()); return _s.str(); }

      template<> inline std::wstring ToString<Fields>(const Fields& f) { std::wstringstream _s; _s << f. size(); return _s.str(); }
      template<> inline std::wstring ToString<Fields>(const Fields* f) { std::wstringstream _s; _s << f->size(); return _s.str(); }
      template<> inline std::wstring ToString<Fields>(      Fields* f) { std::wstringstream _s; _s << f->size(); return _s.str(); }

      template<> inline std::wstring ToString<>(const Piece& p) { std::wstringstream _s; _s << static_cast<const void*>(&p); return _s.str(); }
      template<> inline std::wstring ToString<>(const Piece* p) { std::wstringstream _s; _s << static_cast<const void*>( p); return _s.str(); }
      template<> inline std::wstring ToString<>(      Piece* p) { std::wstringstream _s; _s << static_cast<const void*>( p); return _s.str(); }

      template<> inline std::wstring ToString<>(const Step::StepType& s) { std::wstringstream _s; _s << s; return _s.str(); }
      template<> inline std::wstring ToString<>(const Step::StepType* s) { std::wstringstream _s; _s << s; return _s.str(); }
      template<> inline std::wstring ToString<>(      Step::StepType* s) { std::wstringstream _s; _s << s; return _s.str(); }

      template<> inline std::wstring ToString<>(const SimpleStep& s) { std::wstringstream _s; _s << ToString(s. GetFr()) << ToString(s. GetTo()) << s. GetType(); return _s.str(); }
      template<> inline std::wstring ToString<>(const SimpleStep* s) { std::wstringstream _s; _s << ToString(s->GetFr()) << ToString(s->GetTo()) << s->GetType(); return _s.str(); }
      template<> inline std::wstring ToString<>(      SimpleStep* s) { std::wstringstream _s; _s << ToString(s->GetFr()) << ToString(s->GetTo()) << s->GetType(); return _s.str(); }

      template<> inline std::wstring ToString<>(const ComplexStep& s) { std::wstringstream _s; _s << ToString(s. GetFr()) << ToString(s. GetTo()) << s. GetType(); return _s.str(); }
      template<> inline std::wstring ToString<>(const ComplexStep* s) { std::wstringstream _s; _s << ToString(s->GetFr()) << ToString(s->GetTo()) << s->GetType(); return _s.str(); }
      template<> inline std::wstring ToString<>(      ComplexStep* s) { std::wstringstream _s; _s << ToString(s->GetFr()) << ToString(s->GetTo()) << s->GetType(); return _s.str(); }

      template<> inline std::wstring ToString<>(const PositionValue& p) { std::wstringstream _s; _s << static_cast<int>( p); return _s.str(); }
      template<> inline std::wstring ToString<>(const PositionValue* p) { std::wstringstream _s; _s << static_cast<int>(*p); return _s.str(); }
      template<> inline std::wstring ToString<>(      PositionValue* p) { std::wstringstream _s; _s << static_cast<int>(*p); return _s.str(); }

      template<> inline std::wstring ToString<>(const SimpleMove& s) { std::wstringstream _s; _s << ToString(s. GetStep()->GetFr()) << ToString(s. GetStep()->GetTo()) << s. GetStep()->GetType(); return _s.str(); }
      template<> inline std::wstring ToString<>(const SimpleMove* s) { std::wstringstream _s; _s << ToString(s->GetStep()->GetFr()) << ToString(s->GetStep()->GetTo()) << s->GetStep()->GetType(); return _s.str(); }
      template<> inline std::wstring ToString<>(      SimpleMove* s) { std::wstringstream _s; _s << ToString(s->GetStep()->GetFr()) << ToString(s->GetStep()->GetTo()) << s->GetStep()->GetType(); return _s.str(); }

      template<> inline std::wstring ToString<>(const ComplexMove& c) { std::wstringstream _c; _c << ToString(c. GetSteps()[0]->GetFr()) << ToString(c. GetSteps()[0]->GetTo()) << c. GetSteps()[0]->GetType(); return _c.str(); }
      template<> inline std::wstring ToString<>(const ComplexMove* c) { std::wstringstream _c; _c << ToString(c->GetSteps()[0]->GetFr()) << ToString(c->GetSteps()[0]->GetTo()) << c->GetSteps()[0]->GetType(); return _c.str(); }
      template<> inline std::wstring ToString<>(      ComplexMove* c) { std::wstringstream _c; _c << ToString(c->GetSteps()[0]->GetFr()) << ToString(c->GetSteps()[0]->GetTo()) << c->GetSteps()[0]->GetType(); return _c.str(); }


    }
  }
}

#include <functional>

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
      ComplexStep s2{ f2,f1, st1 };  // Takes defaults to empty, not like in SimpleStep!
      Assert::AreNotEqual(s1, s2);
      Assert::AreEqual(st1, s2.GetType());
      Assert::AreNotEqual(Step::StepType::Normal, s2.GetType());
      Assert::IsTrue(s2.IsTake());
      Assert::AreNotEqual(Fields{ f1 }, s2.GetTakes());  // Takes defaults to empty, not like in SimpleStep!
      Assert::AreEqual(s2.GetTakes().size(), 0U);

      //std::function<const Field&(void)>_l1 = [&s2] { return s2.GetTake(); };
      //Assert::ExpectException<std::exception>(_l1);

      Fields f{ f1,f2 };
      ComplexStep s3{ f2,f1, st1, f};
      Assert::AreNotEqual(s3, s1);
      Assert::AreNotEqual(s3, s2);
      Assert::AreEqual(st1, s3.GetType());
      Assert::AreNotEqual(Step::StepType::Normal, s3.GetType());
      Assert::IsTrue(s3.IsTake());
      Assert::AreNotEqual(Fields{ f1 }, s3.GetTakes());  // Takes defaults to empty, not like in SimpleStep!
      Assert::AreEqual(s3.GetTakes().size(), 2U);
      Assert::AreEqual(s3.GetTake(0),f1);
      Assert::AreEqual(s3.GetTake(1),f2);
    }

    TEST_METHOD(TestPValue)
    {
      PositionValue p0(PositionValue::Undefined);
      PositionValue p1(45);
      PositionValue p2(56);
      PositionValue p3(-67);
      PositionValue p4(-67);

      Assert::IsFalse(p0 == p1);
      Assert::IsTrue(p0 != p1);
      Assert::IsTrue(p0 != p2);
      Assert::IsTrue(p0 != p3);
      Assert::IsTrue(p1 != p2);
      Assert::IsTrue(p1 != p3);
      Assert::IsTrue(p2 != p3);
      Assert::IsTrue(p4 == p3);
      Assert::IsFalse(p4 != p3);

      Assert::IsFalse(p0 == PositionValue(0));
      Assert::IsTrue(p0 == PositionValue(PositionValue::Undefined));
      Assert::AreEqual(p1, PositionValue(45));
      Assert::AreEqual(p2, PositionValue(56));
      Assert::AreEqual(p3, PositionValue(-67));

      std::function<int(void)>_l1 = [&p0] { return static_cast<int>(p0); };
      Assert::ExpectException<std::exception>(_l1);

      Assert::AreEqual(static_cast<int>(p1), 45);
      Assert::AreEqual(static_cast<int>(p2), 56);
      Assert::AreEqual(static_cast<int>(p3), -67);

      Assert::IsTrue(p1 > p0);
      Assert::IsTrue(p2 > p0);
      Assert::IsTrue(p3 > p0);
      Assert::IsTrue(p2 > p1);
      Assert::IsFalse(p3 > p1);
      Assert::IsFalse(p3 > p2);

      Assert::IsFalse(p1 < p0);
      Assert::IsFalse(p2 < p0);
      Assert::IsFalse(p3 < p0);
      Assert::IsFalse(p2 < p1);
      Assert::IsTrue(p3 < p1);
      Assert::IsTrue(p3 < p2);

      Assert::IsTrue(p1 >= p0);
      Assert::IsTrue(p2 >= p0);
      Assert::IsTrue(p3 >= p0);
      Assert::IsTrue(p2 >= p1);
      Assert::IsFalse(p3 >= p1);
      Assert::IsFalse(p3 >= p2);

      Assert::IsFalse(p1 <= p0);
      Assert::IsFalse(p2 <= p0);
      Assert::IsFalse(p3 <= p0);
      Assert::IsFalse(p2 <= p1);
      Assert::IsTrue(p3 <= p1);
      Assert::IsTrue(p3 <= p2);

      PositionValue pu(BoardGamesCore::PositionValue::PValueType::Undefined);
      PositionValue pw(BoardGamesCore::PositionValue::PValueType::Won);
      PositionValue pl(BoardGamesCore::PositionValue::PValueType::Lost);
      PositionValue pt(BoardGamesCore::PositionValue::PValueType::Tie);

      Assert::IsTrue(pu == p0);

      Assert::IsFalse(pl > pl);
      Assert::IsTrue(pu > pl);
      Assert::IsTrue(p1 > pl);
      Assert::IsTrue(p2 > pl);
      Assert::IsTrue(p3 > pl);
      Assert::IsTrue(pt > pl);
      Assert::IsTrue(pw > pl);

      Assert::IsFalse(pl > pu);
      Assert::IsFalse(pu > pu);
      Assert::IsTrue(p1 > pu);
      Assert::IsTrue(p2 > pu);
      Assert::IsTrue(p3 > pu);
      Assert::IsTrue(pt > pu);
      Assert::IsTrue(pw > pu);

      Assert::IsFalse(pl > pt);
      Assert::IsFalse(pu > pt);
      Assert::IsTrue(p1 > pt);
      Assert::IsTrue(p2 > pt);
      Assert::IsFalse(p3 > pt);
      Assert::IsFalse(pt > pt);
      Assert::IsTrue(pw > pt);

      Assert::IsFalse(pl > pw);
      Assert::IsFalse(pu > pw);
      Assert::IsFalse(p1 > pw);
      Assert::IsFalse(p2 > pw);
      Assert::IsFalse(p3 > pw);
      Assert::IsFalse(pt > pw);
      Assert::IsFalse(pw > pw);
    }

    TEST_METHOD(TestSimpleMove)
    {
      Location l1(2U, 3U);
      Location l2(3U, 2U);
      const Piece* p0{ nullptr };
      Field f1{ l1,p0 };
      Field f2{ l2,p0 };
      std::shared_ptr<Step> s0 = std::make_shared<SimpleStep>(f1, f2);
      SimpleMove m1(s0);
      Assert::IsTrue(m1.GetValue() == PositionValue(PositionValue::Undefined));

      PositionValue pw(BoardGamesCore::PositionValue::PValueType::Won);
      PositionValue pl(BoardGamesCore::PositionValue::PValueType::Lost);
      PositionValue pt(BoardGamesCore::PositionValue::PValueType::Tie);

      m1.SetValue(pw);
      Assert::IsTrue(m1.GetValue() == pw);
      m1.SetValue(pl);
      Assert::IsTrue(m1.GetValue() == pl);
      m1.SetValue(pt);
      Assert::IsTrue(m1.GetValue() == pt);

      std::shared_ptr<Step> s1 = std::make_shared<SimpleStep>(f1, f2);
      Assert::IsTrue(dynamic_cast<const SimpleStep&>(*s1) == dynamic_cast<const SimpleStep&>(*s0));

      std::shared_ptr<Step> s2 = std::make_shared<SimpleStep>(f2, f1);
      Assert::IsFalse(dynamic_cast<const SimpleStep&>(*s2) == dynamic_cast<const SimpleStep&>(*s0));

      Step::StepType st0 = static_cast<Step::StepType>(Step::StepType::Take | Step::StepType::Jump);
      std::shared_ptr<Step> s3 = std::make_shared<SimpleStep>(f2,f1, st0);
      Assert::IsFalse(dynamic_cast<const SimpleStep&>(*s3) == dynamic_cast<const SimpleStep&>(*s0));
      Assert::IsFalse(dynamic_cast<const SimpleStep&>(*s3) == dynamic_cast<const SimpleStep&>(*s2));

      SimpleMove m3(s3);
      Assert::IsFalse(m1 == m3);

      m1.SetValue(pt);
      m3.SetValue(pw);
      Assert::IsTrue(m1 < m3);
      m3.SetValue(pl);
      Assert::IsFalse(m1 < m3);
      m1.SetValue(pt);
      Assert::IsFalse(m1 < m3);

      std::shared_ptr<Step> s1r = m1.GetStep();
      Assert::IsTrue(*s1 == *s1r);

      std::shared_ptr<Step> s3r = m3.GetStep();
      Assert::IsTrue(*s3 == *s3r);
      Assert::IsFalse(*s1r == *s3r);

      Steps ss1 = m1.GetSteps();
      Steps ss3 = m3.GetSteps();
      Assert::IsTrue(ss1.size() == 1);
      Assert::IsTrue(ss3.size() == 1);
      Assert::IsTrue(ss1[0] == s0);
      Assert::IsTrue(ss3[0] == s3);
      Assert::IsFalse(ss1[0] == s1);
      Assert::IsFalse(ss1[0] == s2);
      Assert::IsFalse(ss1[0] == s3);

      Assert::IsFalse(m1.IsTake());
      Assert::IsTrue(m3.IsTake());

      Assert::IsTrue(m1.GetFr().GetPiece() == p0);
      Assert::IsTrue(m3.GetFr().GetPiece() == p0);
      Assert::IsTrue(m1.GetTo().GetPiece() == p0);
      Assert::IsTrue(m3.GetTo().GetPiece() == p0);
      Assert::IsTrue(m1.GetFr().GetLocation() == l1);
      Assert::IsTrue(m1.GetTo().GetLocation() == l2);
      Assert::IsTrue(m3.GetFr().GetLocation() == l2);
      Assert::IsTrue(m3.GetTo().GetLocation() == l1);
    }

    TEST_METHOD(TestComplexMove)
    {
      Location l1(2U, 3U);
      Location l2(3U, 2U);
      const Piece* p0{ nullptr };
      Field f1{ l1,p0 };
      Field f2{ l2,p0 };
      std::shared_ptr<Step> s0 = std::make_shared<ComplexStep>(f1, f2);
      Steps ss1{ s0 };
      ComplexMove m1(ss1);
      Assert::IsTrue(m1.GetValue() == PositionValue(PositionValue::Undefined));

      PositionValue pw(BoardGamesCore::PositionValue::PValueType::Won);
      PositionValue pl(BoardGamesCore::PositionValue::PValueType::Lost);
      PositionValue pt(BoardGamesCore::PositionValue::PValueType::Tie);

      m1.SetValue(pw);
      Assert::IsTrue(m1.GetValue() == pw);
      m1.SetValue(pl);
      Assert::IsTrue(m1.GetValue() == pl);
      m1.SetValue(pt);
      Assert::IsTrue(m1.GetValue() == pt);

      std::shared_ptr<Step> s1 = std::make_shared<ComplexStep>(f1, f2);
      Assert::IsTrue(dynamic_cast<const ComplexStep&>(*s1) == dynamic_cast<const ComplexStep&>(*s0));

      std::shared_ptr<Step> s2 = std::make_shared<ComplexStep>(f2, f1);
      Assert::IsFalse(dynamic_cast<const ComplexStep&>(*s2) == dynamic_cast<const ComplexStep&>(*s0));

      Step::StepType st0 = static_cast<Step::StepType>(Step::StepType::Take | Step::StepType::Jump);
      std::shared_ptr<Step> s3 = std::make_shared<ComplexStep>(f2, f1, st0);
      Assert::IsFalse(dynamic_cast<const ComplexStep&>(*s3) == dynamic_cast<const ComplexStep&>(*s0));
      Assert::IsFalse(dynamic_cast<const ComplexStep&>(*s3) == dynamic_cast<const ComplexStep&>(*s2));

      Steps ss3{ s3 };
      ComplexMove m3(ss3);
      Assert::IsFalse(m1 == m3);

      m1.SetValue(pt);
      m3.SetValue(pw);
      Assert::IsTrue(m1 < m3);
      m3.SetValue(pl);
      Assert::IsFalse(m1 < m3);
      m1.SetValue(pt);
      Assert::IsFalse(m1 < m3);

      std::shared_ptr<Step> s1r = m1.GetStep();
      Assert::IsTrue(*s1 == *s1r);

      std::shared_ptr<Step> s3r = m3.GetStep();
      Assert::IsTrue(*s3 == *s3r);
      Assert::IsFalse(*s1r == *s3r);

      Steps ss1r = m1.GetSteps();
      Steps ss3r = m3.GetSteps();
      Assert::IsTrue(ss1r.size() == 1);
      Assert::IsTrue(ss3r.size() == 1);
      Assert::IsTrue(ss1r[0] == s0);
      Assert::IsTrue(ss3r[0] == s3);
      Assert::IsFalse(ss1r[0] == s1);
      Assert::IsFalse(ss1r[0] == s2);
      Assert::IsFalse(ss1r[0] == s3);

      Assert::IsFalse(m1.IsTake());
      Assert::IsTrue(m3.IsTake());

      Assert::IsTrue(m1.GetFr().GetPiece() == p0);
      Assert::IsTrue(m3.GetFr().GetPiece() == p0);
      Assert::IsTrue(m1.GetTo().GetPiece() == p0);
      Assert::IsTrue(m3.GetTo().GetPiece() == p0);
      Assert::IsTrue(m1.GetFr().GetLocation() == l1);
      Assert::IsTrue(m1.GetTo().GetLocation() == l2);
      Assert::IsTrue(m3.GetFr().GetLocation() == l2);
      Assert::IsTrue(m3.GetTo().GetLocation() == l1);
      //std::function<const Field&(void)>_l1 = [&s2] { return s2.GetTake(); };
      //Assert::ExpectException<std::exception>(_l1);
      //std::function<Steps(void)>_l1 = [&m1] { return m1.GetSteps(); };
      //Assert::ExpectException<std::exception>(_l1);
    }

  };
}
