#include "pch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BoardGamesCore;

#include "../BG/Game Checkers/CheckersGame.h"

namespace Microsoft
{
  namespace VisualStudio
  {
    namespace CppUnitTestFramework
    {
      template<> inline std::wstring ToString<Offset>(const Offset& /*o*/) { std::wstringstream _s; _s << "<cannotshow internal data of q>"; return _s.str(); }
      template<> inline std::wstring ToString<Offset>(const Offset* /*o*/) { std::wstringstream _s; _s << "<cannotshow internal data of q>"; return _s.str(); }
      template<> inline std::wstring ToString<Offset>(      Offset* /*o*/) { std::wstringstream _s; _s << "<cannotshow internal data of q>"; return _s.str(); }

      template<> inline std::wstring ToString<Location>(const Location& l) { std::wstringstream _s; _s << '(' << l. x_ << '|' << l. y_ << ')'; return _s.str(); }
      template<> inline std::wstring ToString<Location>(const Location* l) { std::wstringstream _s; _s << '(' << l->x_ << '|' << l->y_ << ')'; return _s.str(); }
      template<> inline std::wstring ToString<Location>(      Location* l) { std::wstringstream _s; _s << '(' << l->x_ << '|' << l->y_ << ')'; return _s.str(); }

      template<> inline std::wstring ToString<Field>(const Field& f) { std::wstringstream _s; _s << ToString(f. GetLocation()) << ',' << static_cast<const void*>(&f. GetPiece()); return _s.str(); }
      template<> inline std::wstring ToString<Field>(const Field* f) { std::wstringstream _s; _s << ToString(f->GetLocation()) << ',' << static_cast<const void*>(&f->GetPiece()); return _s.str(); }
      template<> inline std::wstring ToString<Field>(      Field* f) { std::wstringstream _s; _s << ToString(f->GetLocation()) << ',' << static_cast<const void*>(&f->GetPiece()); return _s.str(); }

      template<> inline std::wstring ToString<Fields>(const Fields& f) { std::wstringstream _s; _s << f. size(); return _s.str(); }
      template<> inline std::wstring ToString<Fields>(const Fields* f) { std::wstringstream _s; _s << f->size(); return _s.str(); }
      template<> inline std::wstring ToString<Fields>(      Fields* f) { std::wstringstream _s; _s << f->size(); return _s.str(); }

      template<> inline std::wstring ToString<>(const Piece& p) { std::wstringstream _s; _s << static_cast<const void*>(&p); return _s.str(); }
      template<> inline std::wstring ToString<>(const Piece* p) { std::wstringstream _s; _s << static_cast<const void*>( p); return _s.str(); }
      template<> inline std::wstring ToString<>(      Piece* p) { std::wstringstream _s; _s << static_cast<const void*>( p); return _s.str(); }

      template<> inline std::wstring ToString<>(const PositionValue& p) { std::wstringstream _s; _s << static_cast<int>( p); return _s.str(); }
      template<> inline std::wstring ToString<>(const PositionValue* p) { std::wstringstream _s; _s << static_cast<int>(*p); return _s.str(); }
      template<> inline std::wstring ToString<>(      PositionValue* p) { std::wstringstream _s; _s << static_cast<int>(*p); return _s.str(); }

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
      Assert::AreEqual(o2, o3);
      Assert::IsTrue(o1.IsParallel(o2));
      Assert::IsTrue(o2.IsParallel(o1));
      Offset o4{ 2,5 };
      Offset o5{ 5,3 };
      Offset o6{ -666,-1110 };
      Assert::IsFalse(o1.IsParallel(o4));
      Assert::IsFalse(o1.IsParallel(o5));
      Assert::IsTrue(o1.IsParallel(o6));
    }

    TEST_METHOD(TestLocation)
    {
      Offset o1{ 3, 5 };
      Offset o2 = o1 * 3;
      Location l1(BoardPart::Main, 2U, 3U);
      Location l2 = l1 + o1;
      Location l3(BoardPart::Main, 5U, 8U);
      Location l4 = l1 + o2;
      Location l5(BoardPart::Main, 11U, 18U);
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
      Location l1(BoardPart::Main, 2U, 3U);
      Location l2(BoardPart::Main, 3U, 2U);
      const Piece& p0{ Piece::NoTile };
      Field f1{ l1,p0 };
      Field f2{ l2,p0 };
      Assert::AreEqual(l1, f1.GetLocation());
      Assert::AreEqual(p0, f1.GetPiece());
      Assert::AreNotEqual(f1, f2);
      Assert::AreEqual(l2, f2.GetLocation());
    }

    TEST_METHOD(TestAction)
    {
      Location l1{ BoardPart::Main, 2U, 3U };
      Location l2{ BoardPart::Main, 3U, 2U };
      Location l3{ BoardPart::Main, 4U, 1U };

      ActionP a1{ std::make_shared<ActionLift>(l1,BoardGamesChessPieces::ChessPiece::WQ) };
      ActionP a2{ std::make_shared<ActionJump>(l2,Piece::NoPiece) };
      ActionP a3{ std::make_shared<ActionDrop>(l3,BoardGamesChessPieces::ChessPiece::WQ) };
      ActionP a4{ std::make_shared<ActionLift>(l1,BoardGamesChessPieces::ChessPiece::WQ) };
      ActionP a5{ std::make_shared<ActionJump>(l2,Piece::NoPiece) };
      ActionP a6{ std::make_shared<ActionDrop>(l3,BoardGamesChessPieces::ChessPiece::WQ) };

      Assert::IsTrue(a1->GetLocation() == l1);
      Assert::IsTrue(a2->GetLocation() == l2);
      Assert::IsTrue(a3->GetLocation() == l3);
      Assert::IsTrue(a1->GetPiece() == BoardGamesChessPieces::ChessPiece::WQ);
      Assert::IsTrue(a2->GetPiece() == Piece::NoPiece);
      Assert::IsTrue(a3->GetPiece() == BoardGamesChessPieces::ChessPiece::WQ);

      Assert::IsFalse(a1->IsJump());
      Assert::IsTrue(a2->IsJump());
      Assert::IsFalse(a3->IsJump());

      Assert::IsTrue(*a1 == *a1);
      Assert::IsTrue(*a2 == *a2);
      Assert::IsTrue(*a3 == *a3);

      Assert::IsTrue(*a1 == *a4);
      Assert::IsFalse(*a1 == *a5);
      Assert::IsFalse(*a1 == *a6);
      Assert::IsFalse(*a2 == *a4);
      Assert::IsTrue(*a2 == *a5);
      Assert::IsFalse(*a2 == *a6);
      Assert::IsFalse(*a3 == *a4);
      Assert::IsFalse(*a3 == *a5);
      Assert::IsTrue(*a3 == *a6);
      Assert::IsTrue(*a4 == *a1);
      Assert::IsFalse(*a5 == *a1);
      Assert::IsFalse(*a6 == *a1);

      Assert::IsFalse(*a1 != *a4);
      Assert::IsTrue (*a1 != *a5);
      Assert::IsTrue (*a1 != *a6);
      Assert::IsTrue (*a2 != *a4);
      Assert::IsFalse(*a2 != *a5);
      Assert::IsTrue (*a2 != *a6);
      Assert::IsTrue (*a3 != *a4);
      Assert::IsTrue (*a3 != *a5);
      Assert::IsFalse(*a3 != *a6);
      Assert::IsFalse(*a4 != *a1);
      Assert::IsTrue (*a5 != *a1);
      Assert::IsTrue (*a6 != *a1);

      {
        Actions a{};
        a.push_back(a1);
        a.push_back(a3);
        Assert::IsFalse(a.HasJump());

        a.clear();
        a.push_back(a1);
        a.push_back(a2);
        a.push_back(a3);
        Assert::IsTrue(a.HasJump());

        Assert::IsTrue(a.IsRepeat(l2));
        Assert::IsTrue(a.IsRepeat(l2, Offset{ l3,l2 }));
        Assert::IsFalse(a.IsRepeat(l1));
        Assert::IsFalse(a.IsRepeat(l3));
      }

      const Piece& p{ Piece::NoTile };
      std::function<const Piece& (void)>f = [&p] (void) -> const Piece& { return p; };

      class TestPosition : public MainPosition
      {
      public:
        TestPosition(const Location& l, const Piece& p) noexcept : l_{ l }, p_(&p), MainPosition(Variants<Checkers::CheckersGame>::GetPieces(), Checkers::CheckersGame::GetDimensions(8U, 8U)) {}
        virtual MainPosition* Clone(void) const noexcept { return nullptr; }
        virtual const Piece& SetPiece(const Location& l, const Piece& p) noexcept override { Assert::IsTrue(l == l_); p_ = &p; return p; }
        virtual const Piece& GetPiece(const Location& l) const noexcept override { Assert::IsTrue(l == l_); return *p_; }
        const Location& l_;
        const Piece* p_;
      };

      TestPosition t{ l1, Piece::NoPiece };
      Assert::IsTrue(t.SetPiece(l1, Piece::NoPiece) == Piece::NoPiece);

      // testing Action.Execute():
      TestPosition t1{ a1->GetLocation(), a1->GetPiece() };   // take starting piece
      Assert::IsTrue(t1.GetPiece(a1->GetLocation()) == a1->GetPiece());
      a1->Execute(&t1);
      Assert::IsTrue(t1.GetPiece(a1->GetLocation()) == Piece::NoPiece);

      TestPosition t2{ a2->GetLocation(), Piece::NoPiece };  // jump over empty field
      Assert::IsTrue(t2.GetPiece(a2->GetLocation()) == Piece::NoPiece);
      a2->Execute(&t2);
      Assert::IsTrue(t2.GetPiece(a2->GetLocation()) == Piece::NoPiece);

      TestPosition t3{ a3->GetLocation(), Piece::NoPiece };  // place on empty field
      Assert::IsTrue(t3.GetPiece(a3->GetLocation()) == Piece::NoPiece);
      a3->Execute(&t3);
      Assert::IsTrue(t3.GetPiece(a3->GetLocation()) == a3->GetPiece());
    }

    TEST_METHOD(TestMoves)
    {
      Location l1{ BoardPart::Main, 2U, 3U };
      Location l2{ BoardPart::Main, 3U, 2U };
      Location l3{ BoardPart::Main, 4U, 1U };

      ActionP a11{ std::make_shared<ActionLift>(l1,BoardGamesChessPieces::ChessPiece::WQ) };
      ActionP a12{ std::make_shared<ActionJump>(l2,Piece::NoPiece) };
      ActionP a13{ std::make_shared<ActionDrop>(l3,BoardGamesChessPieces::ChessPiece::WQ) };

      ActionP a21{ std::make_shared<ActionLift>(l1,BoardGamesChessPieces::ChessPiece::WQ) };
      ActionP a22{ std::make_shared<ActionDrop>(l3,BoardGamesChessPieces::ChessPiece::WQ) };

      ActionP a31{ std::make_shared<ActionLift>(l1,BoardGamesChessPieces::ChessPiece::WQ) };
      ActionP a32{ std::make_shared<ActionJump>(l2,Piece::NoPiece) };
      ActionP a33{ std::make_shared<ActionDrop>(l3,BoardGamesChessPieces::ChessPiece::WQ) };

      Actions a1{};
      a1.push_back(a11);
      a1.push_back(a12);
      a1.push_back(a13);

      Actions a2{};
      a2.push_back(a21);
      a2.push_back(a22);

      Actions a3{};
      a3.push_back(a31);
      a3.push_back(a32);
      a3.push_back(a33);

      Move m1{ a1 };
      Move m2{ a2 };
      Move m3{ a3 };

      Moves m{};
      m.push_back(std::make_shared<Move>(a1));
      m.push_back(std::make_shared<Move>(a2));
      m.push_back(std::make_shared<Move>(a3));

      class TestPosition : public MainPosition
      {
      public:
        TestPosition(void) noexcept : MainPosition(Variants<Checkers::CheckersGame>::GetPieces(), Checkers::CheckersGame::GetDimensions(8U, 8U)) {}
        virtual MainPosition* Clone(void) const noexcept { return nullptr; }
      };

      TestPosition pos{};
      Assert::IsTrue(m.size() == 3);
      pos.JumpsOnly(m);
      Assert::IsTrue(m.size() == 2);
      for (const auto& mi : m)
      {
        Assert::IsTrue(mi->GetActions().HasJump());
      }

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

    TEST_METHOD(TestColor)
    {
      Assert::IsFalse(&BoardGamesCore::Color::Void == nullptr);
      Assert::IsFalse(&BoardGamesCore::Color::NoColor == nullptr);
      Assert::IsFalse(&BoardGamesCore::Color::White == nullptr);
      Assert::IsFalse(&BoardGamesCore::Color::Black == nullptr);

      Assert::IsFalse(&BoardGamesCore::Color::Void == &BoardGamesCore::Color::NoColor);
      Assert::IsFalse(&BoardGamesCore::Color::Void == &BoardGamesCore::Color::White);
      Assert::IsFalse(&BoardGamesCore::Color::Void == &BoardGamesCore::Color::Black);
      Assert::IsFalse(&BoardGamesCore::Color::NoColor == &BoardGamesCore::Color::White);
      Assert::IsFalse(&BoardGamesCore::Color::NoColor == &BoardGamesCore::Color::Black);
      Assert::IsFalse(&BoardGamesCore::Color::White == &BoardGamesCore::Color::Black);

      Assert::IsFalse(BoardGamesCore::Color::Void == BoardGamesCore::Color::NoColor);
      Assert::IsFalse(BoardGamesCore::Color::Void == BoardGamesCore::Color::White);
      Assert::IsFalse(BoardGamesCore::Color::Void == BoardGamesCore::Color::Black);
      Assert::IsFalse(BoardGamesCore::Color::NoColor == BoardGamesCore::Color::White);
      Assert::IsFalse(BoardGamesCore::Color::NoColor == BoardGamesCore::Color::Black);
      Assert::IsFalse(BoardGamesCore::Color::White == BoardGamesCore::Color::Black);

      Assert::IsTrue(!BoardGamesCore::Color::White == BoardGamesCore::Color::Black);
      Assert::IsTrue(!BoardGamesCore::Color::Black == BoardGamesCore::Color::White);
      Assert::IsTrue(!BoardGamesCore::Color::NoColor == BoardGamesCore::Color::White);
      Assert::IsTrue(!BoardGamesCore::Color::Void == BoardGamesCore::Color::White);
    }

    TEST_METHOD(TestTileColor)
    {
      Assert::IsFalse(&BoardGamesCore::TileColor::Light == nullptr);
      Assert::IsFalse(&BoardGamesCore::TileColor::Dark == nullptr);
      Assert::IsFalse(&BoardGamesCore::TileColor::Small == nullptr);

      Assert::IsFalse(&BoardGamesCore::TileColor::Light == &BoardGamesCore::TileColor::Dark);
      Assert::IsFalse(&BoardGamesCore::TileColor::Light == &BoardGamesCore::TileColor::Small);
      Assert::IsFalse(&BoardGamesCore::TileColor::Dark == &BoardGamesCore::TileColor::Small);
    }

    TEST_METHOD(TestKind)
    {
      Assert::IsFalse(&BoardGamesCore::noKind::NoKind == nullptr);
    }

    TEST_METHOD(TestPiece)
    {
      Assert::IsFalse(&BoardGamesCore::Piece::NoTile == nullptr);
      Assert::IsFalse(&BoardGamesCore::Piece::NoPiece == nullptr);

      Assert::IsFalse(&BoardGamesCore::Piece::NoTile == &BoardGamesCore::Piece::NoPiece);

      Assert::IsTrue(BoardGamesCore::Piece::NoPiece.IsBlank());
      Assert::IsFalse(BoardGamesCore::Piece::NoTile.IsBlank());

      size_t ht = BoardGamesCore::Piece::NoTile.GetHash();
      size_t hp = BoardGamesCore::Piece::NoPiece.GetHash();
      Assert::IsFalse(BoardGamesCore::Piece::NoTile.GetHash() == BoardGamesCore::Piece::NoPiece.GetHash());

      Location l1(BoardPart::Main, 2U, 3U);
      Moves m{};
      MainPosition* p{};
      BoardGamesCore::Piece::NoTile.CollectMoves(*p, l1, m);
      Assert::IsTrue(m.size() == 0);

      Assert::IsTrue(BoardGamesCore::Piece::NoTile.IsKind(BoardGamesCore::noKind::NoKind));
      Assert::IsTrue(BoardGamesCore::Piece::NoPiece.IsKind(BoardGamesCore::noKind::NoKind));

      Assert::IsTrue(BoardGamesCore::Piece::NoTile.IsColor(BoardGamesCore::Color::Void));
      Assert::IsTrue(BoardGamesCore::Piece::NoPiece.IsColor(BoardGamesCore::Color::NoColor));

      Assert::IsTrue(BoardGamesCore::Piece::NoTile.GetColor() == BoardGamesCore::Color::Void);
      Assert::IsTrue(BoardGamesCore::Piece::NoPiece.GetColor() == BoardGamesCore::Color::NoColor);

      Assert::IsTrue(BoardGamesCore::Piece::NoTile.GetValue(*p, l1) == 0);
      Assert::IsTrue(BoardGamesCore::Piece::NoPiece.GetValue(*p, l1) == 0);

      Assert::IsFalse(BoardGamesCore::Piece::NoTile.IsPromotable());
      Assert::IsFalse(BoardGamesCore::Piece::NoPiece.IsPromotable());

      Assert::IsTrue(BoardGamesCore::Piece::NoTile.Promote(true) == BoardGamesCore::Piece::NoTile);
      Assert::IsTrue(BoardGamesCore::Piece::NoPiece.Promote(true) == BoardGamesCore::Piece::NoPiece);
      Assert::IsTrue(BoardGamesCore::Piece::NoTile.Promote(false) == BoardGamesCore::Piece::NoTile);
      Assert::IsTrue(BoardGamesCore::Piece::NoPiece.Promote(false) == BoardGamesCore::Piece::NoPiece);
    }

    TEST_METHOD(TestPlayerType)
    {
      Assert::IsFalse(&BoardGamesCore::PlayerType::Human == nullptr);
      Assert::IsFalse(&BoardGamesCore::PlayerType::Computer == nullptr);

      Assert::IsFalse(&BoardGamesCore::PlayerType::Human == &BoardGamesCore::PlayerType::Computer);
      Assert::IsFalse(BoardGamesCore::PlayerType::Human == BoardGamesCore::PlayerType::Computer);
    }

    TEST_METHOD(TestPlayer)
    {
      Player hw(&BoardGamesCore::PlayerType::Human, BoardGamesCore::Color::White);
      Player hb(&BoardGamesCore::PlayerType::Human, BoardGamesCore::Color::Black);
      Player cw(&BoardGamesCore::PlayerType::Computer, BoardGamesCore::Color::White);
      Player cb(&BoardGamesCore::PlayerType::Computer, BoardGamesCore::Color::Black);

      Assert::IsFalse(hw.IsAI());
      Assert::IsFalse(hb.IsAI());
      Assert::IsTrue(cw.IsAI());
      Assert::IsTrue(cb.IsAI());

      Assert::IsFalse(hw.GetColor() == hb.GetColor());
      Assert::IsFalse(cw.GetColor() == cb.GetColor());
      Assert::IsTrue(hw.GetColor() == cw.GetColor());
      Assert::IsTrue(hb.GetColor() == cb.GetColor());
      Assert::IsFalse(hw.GetColor() == cb.GetColor());
      Assert::IsFalse(cw.GetColor() == hb.GetColor());
    }

  };
}


namespace UnitTestAI
{
  TEST_CLASS(UnitTestAI)
  {

  public:
    TEST_METHOD(TestAI)
    {



    }

  };
}
