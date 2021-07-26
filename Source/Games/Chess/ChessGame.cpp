#include "Games.h"

#include "ChessGame.h"

namespace Chess
{

  const Pawn     Pawn    ::ThePawn    {};
  const Knight   Knight  ::TheKnight  {};
  const Bishop   Bishop  ::TheBishop  {};
  const Rook     Rook    ::TheRook    {};
  const Cardinal Cardinal::TheCardinal{};
  const Marshall Marshall::TheMarshall{};
  const Queen    Queen   ::TheQueen   {};
  const Amazon   Amazon  ::TheAmazon  {};
  const King     King    ::TheKing    {};
  const Behemoth Behemoth::TheBehemoth{};
  const Ice      Ice     ::TheIce     {};

  const ChessPiece ChessPiece::WP{ Pawn    ::ThePawn,    PieceColor::White,   IDB_WPL };  // White Pawn
  const ChessPiece ChessPiece::WN{ Knight  ::TheKnight,  PieceColor::White,   IDB_WNL };  // White Knight
  const ChessPiece ChessPiece::WB{ Bishop  ::TheBishop,  PieceColor::White,   IDB_WBL };  // White Bishop
  const ChessPiece ChessPiece::WR{ Rook    ::TheRook,    PieceColor::White,   IDB_WRL };  // White Rook 
  const ChessPiece ChessPiece::WC{ Cardinal::TheCardinal,PieceColor::White,   IDB_WCL };  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
  const ChessPiece ChessPiece::WM{ Marshall::TheMarshall,PieceColor::White,   IDB_WML };  // White Marshall = Rook + Knight
  const ChessPiece ChessPiece::WQ{ Queen   ::TheQueen,   PieceColor::White,   IDB_WQL };  // White Queen
  const ChessPiece ChessPiece::WA{ Amazon  ::TheAmazon,  PieceColor::White,   IDB_WAL };  // White Amazon   = Queen + Knight
  const ChessPiece ChessPiece::WK{ King    ::TheKing,    PieceColor::White,   IDB_WKL };  // White King 

  const ChessPiece ChessPiece::BP{ Pawn    ::ThePawn,    PieceColor::Black,   IDB_BPL };  // Black Pawn
  const ChessPiece ChessPiece::BN{ Knight  ::TheKnight,  PieceColor::Black,   IDB_BNL };  // Black Knight
  const ChessPiece ChessPiece::BB{ Bishop  ::TheBishop,  PieceColor::Black,   IDB_BBL };  // Black Bishop
  const ChessPiece ChessPiece::BR{ Rook    ::TheRook,    PieceColor::Black,   IDB_BRL };  // Black Rook
  const ChessPiece ChessPiece::BC{ Cardinal::TheCardinal,PieceColor::Black,   IDB_BCL };  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
  const ChessPiece ChessPiece::BM{ Marshall::TheMarshall,PieceColor::Black,   IDB_BML };  // Black Marshall = Rook + Knight
  const ChessPiece ChessPiece::BQ{ Queen   ::TheQueen,   PieceColor::Black,   IDB_BQL };  // Black Queen
  const ChessPiece ChessPiece::BA{ Amazon  ::TheAmazon,  PieceColor::Black,   IDB_BAL };  // Black Amazon   = Queen + Knight
  const ChessPiece ChessPiece::BK{ King    ::TheKing,    PieceColor::Black,   IDB_BKL };  // Black King

  const ChessPiece ChessPiece::RB{ Behemoth::TheBehemoth,PieceColor::NoColor, IDB_RBL };  // (Red) Behemoth
  const ChessPiece ChessPiece::GI{ Ice     ::TheIce,     PieceColor::NoColor, IDB_GIL };  // (Gray) Ice

  void Pawn::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    const MainPosition* pp = &p;
    static bool t1 = p.HasRule(PawnsDoubleStep);
    static bool t2 = pp->HasRule(PawnsDoubleStep);
    const ChessPosition& cp = dynamic_cast<const ChessPosition&>(p);
    const ChessPosition* cpp = &cp;
    static bool tc1 = cp.HasRule(PawnsDoubleStep);
    static bool tc2 = cpp->HasRule(PawnsDoubleStep);


    const PieceColor& c{ p.GetPiece(l).GetColor()};
    const int dy = c == PieceColor::White ? -1 : 1;

    // if there is an opponent's piece there, allow the pawn to take diagonally
    if (p.GetPiece(l + Offset(+1, dy)).IsColor(~c)) p.AddIfLegal(moves, l, l + Offset(+1, dy));
    if (p.GetPiece(l + Offset(-1, dy)).IsColor(~c)) p.AddIfLegal(moves, l, l + Offset(-1, dy));

    // if the field ahead is free, pawn can move there
    if (p.GetPiece(l + Offset(0, dy)).IsBlank())
    {
      bool step = p.AddIfLegal(moves, l, l + Offset(0, dy));
      if (step && pp->HasRule(PawnsDoubleStep))
      {
        Coordinate z = c == PieceColor::White ? p.GetSizeY() - 2U : 1U;
        // if the next field ahead is free too, pawn can move there too
        if (l.y_ == z) if (p.GetPiece(l + Offset(0, dy * 2)).IsBlank()) p.AddIfLegal(moves, l, l + Offset(0, dy * 2));
      }
    }
  }
  void Knight::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(+2, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, +2));
    p.AddIfLegal(moves, l, l + Offset(-1, +2));
    p.AddIfLegal(moves, l, l + Offset(-2, +1));
    p.AddIfLegal(moves, l, l + Offset(-2, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, -2));
    p.AddIfLegal(moves, l, l + Offset(+1, -2));
    p.AddIfLegal(moves, l, l + Offset(+2, -1));
  }
  void Bishop::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::BDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Rook::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::RDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Cardinal::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(+2, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, +2));
    p.AddIfLegal(moves, l, l + Offset(-1, +2));
    p.AddIfLegal(moves, l, l + Offset(-2, +1));
    p.AddIfLegal(moves, l, l + Offset(-2, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, -2));
    p.AddIfLegal(moves, l, l + Offset(+1, -2));
    p.AddIfLegal(moves, l, l + Offset(+2, -1));
    for (const Offset& o : Offset::BDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Marshall::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(+2, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, +2));
    p.AddIfLegal(moves, l, l + Offset(-1, +2));
    p.AddIfLegal(moves, l, l + Offset(-2, +1));
    p.AddIfLegal(moves, l, l + Offset(-2, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, -2));
    p.AddIfLegal(moves, l, l + Offset(+1, -2));
    p.AddIfLegal(moves, l, l + Offset(+2, -1));
    for (const Offset& o : Offset::RDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Queen::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::QDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Amazon::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    p.AddIfLegal(moves, l, l + Offset(+2, +1));
    p.AddIfLegal(moves, l, l + Offset(+1, +2));
    p.AddIfLegal(moves, l, l + Offset(-1, +2));
    p.AddIfLegal(moves, l, l + Offset(-2, +1));
    p.AddIfLegal(moves, l, l + Offset(-2, -1));
    p.AddIfLegal(moves, l, l + Offset(-1, -2));
    p.AddIfLegal(moves, l, l + Offset(+1, -2));
    p.AddIfLegal(moves, l, l + Offset(+2, -1));
    for (const Offset& o : Offset::QDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void King::CollectMoves(const MainPosition& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::QDirection) p.AddIfLegal(moves, l, l + o);
  }

  void ChessPosition::SetPawns(Coordinate row, const PieceColor& c) noexcept
  {
    const Piece& p{ c == PieceColor::White ? ChessPiece::WP : ChessPiece::BP };
    for (Coordinate i = 0; i < sizeX_; i++)
    {
      const Location& l{ BoardPart::Main, i, row };
      SetPiece(l, p);
    }
  }

  void ChessPosition::SetPiecesPSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept
  {
    SetPiece({ BoardPart::Main,               x,               y }, b);
    SetPiece({ BoardPart::Main, sizeX_ - 1U - x, sizeY_ - 1U - y }, w);
  }
  void ChessPosition::SetPiecesHSymmetrical(Coordinate x, Coordinate y, const ChessPiece& b, const ChessPiece& w) noexcept
  {
    SetPiece({ BoardPart::Main,               x,               y }, b);
    SetPiece({ BoardPart::Main,               x, sizeY_ - 1U - y }, w);
  }

  void ChessPosition::SetStartingPosition() noexcept
  {
    SetPawns(1U, PieceColor::Black);
    SetPawns(sizeY_ - 2U, PieceColor::White);

    SetPiece({ BoardPart::Main,               0U,          0U }, ChessPiece::BR);
    SetPiece({ BoardPart::Main,               1U,          0U }, ChessPiece::BN);
    SetPiece({ BoardPart::Main,               2U,          0U }, ChessPiece::BB);
    SetPiece({ BoardPart::Main, sizeX_      - 3U,          0U }, ChessPiece::BB);
    SetPiece({ BoardPart::Main, sizeX_      - 2U,          0U }, ChessPiece::BN);
    SetPiece({ BoardPart::Main, sizeX_      - 1U,          0U }, ChessPiece::BR);
    SetPiece({ BoardPart::Main, sizeX_ / 2U - 1U,          0U }, ChessPiece::BQ);  // set Queen and King last!
    SetPiece({ BoardPart::Main, sizeX_ / 2U     ,          0U }, ChessPiece::BK);  // this is useful for smaller boards

    SetPiece({ BoardPart::Main,               0U, sizeY_ - 1U }, ChessPiece::WR);
    SetPiece({ BoardPart::Main,               1U, sizeY_ - 1U }, ChessPiece::WN);
    SetPiece({ BoardPart::Main,               2U, sizeY_ - 1U }, ChessPiece::WB);
    SetPiece({ BoardPart::Main, sizeX_ -      3U, sizeY_ - 1U }, ChessPiece::WB);
    SetPiece({ BoardPart::Main, sizeX_ -      2U, sizeY_ - 1U }, ChessPiece::WN);
    SetPiece({ BoardPart::Main, sizeX_ -      1U, sizeY_ - 1U }, ChessPiece::WR);
    SetPiece({ BoardPart::Main, sizeX_ / 2U - 1U, sizeY_ - 1U }, ChessPiece::WQ);  // set Queen and King last!
    SetPiece({ BoardPart::Main, sizeX_ / 2U     , sizeY_ - 1U }, ChessPiece::WK);  // this is useful for smaller boards
  }

  bool ChessPosition::AddIfLegal(Moves& m, const Location& fr, const Location& to) const noexcept
  {
    const Piece& pf = GetPiece(fr);
    assert(pf != Piece::NoTile);                                         // start field must exist
    assert(!pf.IsBlank());                                               // start field must be a piece

    const Piece& pt = GetPiece(to);
    if (pt == Piece::NoTile) return false;                               // out of board
    if (pt.IsBlank())                                                    // moving onto free field
    {
      Actions a{};
      a.push_back(std::make_shared<ActionLift>(fr, pf));                 // pick piece up
      a.push_back(std::make_shared<ActionDrop>(to, pf));                 // and place it on target
      m.push_back(std::make_shared<Move>(a));                            // add move to move list
      return true;                                                       // keep trying this direction
    }

    if (pt.GetColor() == pf.GetColor()) return false;                    // own piece; don't keep trying this direction

    // valid take move, save into collection
    Actions a{};
    a.push_back(std::make_shared<ActionLift>(fr, pf));                   // pick piece up
    a.push_back(std::make_shared<ActionLift>(to, pt));                   // pick opponent piece up
    a.push_back(std::make_shared<ActionDrop>(GetNextTakenL(pf.GetColor()), pt));    // place it in Taken
    a.push_back(std::make_shared<ActionDrop>(to, pf));                   // and place it on target
    m.push_back(std::make_shared<Move>(a));                              // add move to move list

    return false;                                                        // don't keep trying this direction
  };


  const VariantList& ChessGame::GetVariants(void) noexcept
  {
    static VariantList v{
      { Variant{ "Standard Chess",          VC(ChessVariant::Standard),           8,  8 } },
      { Variant{ "Corner Chess",            VC(ChessVariant::Corner),             8,  8 } },
      { Variant{ "Fortress Chess",          VC(ChessVariant::Fortress),           8,  8 } },
      { Variant{ "Horde Chess",             VC(ChessVariant::Horde),              8,  8 } },
      { Variant{ "Loop Chess",              VC(ChessVariant::Loop),               8,  8 } },
      { Variant{ "Anti Chess",              VC(ChessVariant::Anti),               8,  8 } },
      { Variant{ "Extinction Chess",        VC(ChessVariant::Extinction),         8,  8 } },
      { Variant{ "Maharajah Chess",         VC(ChessVariant::Maharajah),          8,  8 } },
      { Variant{ "Three Checks Chess",      VC(ChessVariant::ThreeChecks),        8,  8 } },
      { Variant{ "Dark Chess",              VC(ChessVariant::Dark),               8,  8 } },
      { Variant{ "Atomic Chess",            VC(ChessVariant::Atomic),             8,  8 } },
      { Variant{ "Janus Chess",             VC(ChessVariant::Janus),             10,  8 } },
      { Variant{ "Embassy Chess",           VC(ChessVariant::Embassy),           10,  8 } },
      { Variant{ "Screen Chess",            VC(ChessVariant::Screen),             8,  8 } },
      { Variant{ "Crazy Screen Chess",      VC(ChessVariant::CrazyScreen),        8,  8 } },
      { Variant{ "Cylinder Chess",          VC(ChessVariant::Cylinder),           8,  8 } },
      { Variant{ "Amazon Chess",            VC(ChessVariant::Amazons),            8,  8 } },
      { Variant{ "Berolina Chess",          VC(ChessVariant::Berolina),           8,  8 } },
      { Variant{ "Fischer Random Chess",    VC(ChessVariant::FischerRandom),      8,  8 } },
      { Variant{ "Legan Chess",             VC(ChessVariant::Legan),              8,  8 } },
      { Variant{ "Knight Relay Chess",      VC(ChessVariant::KnightRelay),        8,  8 } },
      { Variant{ "Grand Chess",             VC(ChessVariant::Grand),             10, 10 } },
      { Variant{ "Capablanca Random Chess", VC(ChessVariant::CapablancaRandom),  10,  8 } },
      { Variant{ "Los Alamos Chess",        VC(ChessVariant::LosAlamos),          6,  6 } },
      { Variant{ "Ambiguous Chess",         VC(ChessVariant::Ambiguous),          8,  8 } },
      { Variant{ "Cheversi",                VC(ChessVariant::Cheversi),           8,  8 } },
      { Variant{ "Dice Chess",              VC(ChessVariant::Dice),               8,  8 } },
      { Variant{ "Recycle Chess",           VC(ChessVariant::Recycle),            8,  8 } },
      { Variant{ "Ice Age Chess",           VC(ChessVariant::IceAge),             8,  8 } },
      { Variant{ "Behemoth Chess",          VC(ChessVariant::Behemoth),           8,  8 } },
      { Variant{ "Cheshire Cat Chess",      VC(ChessVariant::CheshireCat),        8,  8 } },
      { Variant{ "Knightmate Chess",        VC(ChessVariant::Knightmate),         8,  8 } },
      { Variant{ "Racing Kings",            VC(ChessVariant::RacingKings),        8,  8 } },
      { Variant{ "Dice Chess 10x10",        VC(ChessVariant::Dice10x10),         10, 10 } },
      { Variant{ "Massacre Chess",          VC(ChessVariant::Massacre),           8,  8 } },
    };
    return v;
  }

  const PieceMapP& ChessGame::GetPieces(const VariantChosen& v) noexcept
  {
    static const PieceMapP& p = std::make_shared<PieceMap>();
    p->Empty();
    p->Add(ChessPiece::WP);    p->Add(ChessPiece::BP);    // Pawn
    p->Add(ChessPiece::WN);    p->Add(ChessPiece::BN);    // Knight
    p->Add(ChessPiece::WB);    p->Add(ChessPiece::BB);    // Bishop
    p->Add(ChessPiece::WR);    p->Add(ChessPiece::BR);    // Rook
    p->Add(ChessPiece::WC);    p->Add(ChessPiece::BC);    // Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
    p->Add(ChessPiece::WM);    p->Add(ChessPiece::BM);    // Marshall = Rook + Knight 
    p->Add(ChessPiece::WQ);    p->Add(ChessPiece::BQ);    // Queen 
    p->Add(ChessPiece::WA);    p->Add(ChessPiece::BA);    // Amazon   = Queen + Knight 
    p->Add(ChessPiece::WK);    p->Add(ChessPiece::BK);    // King  

    p->Add(ChessPiece::RB);                                // (Red) Behemoth
    p->Add(ChessPiece::GI);                                // (Gray) Ice
    return p;
  }


  const Dimensions ChessGame::GetDimensions(const VariantChosen& v) noexcept
  {
    Dimensions d{
       Dimension(v.x, v.y, BoardStartX, BoardStartY, FieldSizeX, FieldSizeY, 1, 1),
       Dimension(11, 2, BoardStartX + FieldSizeX, BoardStartY + FieldSizeY * (v.y + 1), FieldSizeX, FieldSizeY),
       Dimension(v.x * v.y / 2, 2, BoardStartX + FieldSizeX * (v.x + 1), BoardStartY + FieldSizeSY, FieldSizeSX, FieldSizeSY, 0, FieldSizeY * v.y - FieldSizeSY * 4),
    };
    return d;
  }

}
