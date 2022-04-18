#include "Games.h"

#include "ChessGame.h"

namespace Chess
{

  const Pawn     Pawn::ThePawn{};
  const Knight   Knight::TheKnight{};
  const Bishop   Bishop::TheBishop{};
  const Rook     Rook::TheRook{};
  const Cardinal Cardinal::TheCardinal{};
  const Marshall Marshall::TheMarshall{};
  const Queen    Queen::TheQueen{};
  const Amazon   Amazon::TheAmazon{};
  const King     King::TheKing{};
  const Behemoth Behemoth::TheBehemoth{};
  const Ice      Ice::TheIce{};

  const ChessPiece ChessPiece::WP{ Pawn::ThePawn,    PieceColor::White,   IDB_WPL };  // White Pawn
  const ChessPiece ChessPiece::WN{ Knight::TheKnight,  PieceColor::White,   IDB_WNL };  // White Knight
  const ChessPiece ChessPiece::WB{ Bishop::TheBishop,  PieceColor::White,   IDB_WBL };  // White Bishop
  const ChessPiece ChessPiece::WR{ Rook::TheRook,    PieceColor::White,   IDB_WRL };  // White Rook 
  const ChessPiece ChessPiece::WC{ Cardinal::TheCardinal,PieceColor::White,   IDB_WCL };  // White Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
  const ChessPiece ChessPiece::WM{ Marshall::TheMarshall,PieceColor::White,   IDB_WML };  // White Marshall = Rook + Knight
  const ChessPiece ChessPiece::WQ{ Queen::TheQueen,   PieceColor::White,   IDB_WQL };  // White Queen
  const ChessPiece ChessPiece::WA{ Amazon::TheAmazon,  PieceColor::White,   IDB_WAL };  // White Amazon   = Queen + Knight
  const ChessPiece ChessPiece::WK{ King::TheKing,    PieceColor::White,   IDB_WKL };  // White King 

  const ChessPiece ChessPiece::BP{ Pawn::ThePawn,    PieceColor::Black,   IDB_BPL };  // Black Pawn
  const ChessPiece ChessPiece::BN{ Knight::TheKnight,  PieceColor::Black,   IDB_BNL };  // Black Knight
  const ChessPiece ChessPiece::BB{ Bishop::TheBishop,  PieceColor::Black,   IDB_BBL };  // Black Bishop
  const ChessPiece ChessPiece::BR{ Rook::TheRook,    PieceColor::Black,   IDB_BRL };  // Black Rook
  const ChessPiece ChessPiece::BC{ Cardinal::TheCardinal,PieceColor::Black,   IDB_BCL };  // Black Cardinal = Bishop + Knight (also: Archbishop, Princess, Janus)
  const ChessPiece ChessPiece::BM{ Marshall::TheMarshall,PieceColor::Black,   IDB_BML };  // Black Marshall = Rook + Knight
  const ChessPiece ChessPiece::BQ{ Queen::TheQueen,   PieceColor::Black,   IDB_BQL };  // Black Queen
  const ChessPiece ChessPiece::BA{ Amazon::TheAmazon,  PieceColor::Black,   IDB_BAL };  // Black Amazon   = Queen + Knight
  const ChessPiece ChessPiece::BK{ King::TheKing,    PieceColor::Black,   IDB_BKL };  // Black King

  const ChessPiece ChessPiece::RB{ Behemoth::TheBehemoth,PieceColor::NoColor, IDB_RBL };  // (Red) Behemoth
  const ChessPiece ChessPiece::GI{ Ice::TheIce,     PieceColor::NoColor, IDB_GIL };  // (Gray) Ice

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



  void Pawn::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    const int dy = c == PieceColor::White ? -1 : 1;

    m.push_back(Motion{ { 0, dy  }, false });  // normal move
    m.push_back(Motion{ { 0, dy * 2}, false });  // double step
    m.push_back(Motion{ {+1, dy  }, false });  // take right
    m.push_back(Motion{ {-1, dy  }, false });  // take left
  }

  void Knight::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    m.push_back(Motion{ {+2, +1}, false });
    m.push_back(Motion{ {+1, +2}, false });
    m.push_back(Motion{ {-1, +2}, false });
    m.push_back(Motion{ {-2, +1}, false });
    m.push_back(Motion{ {-2, -1}, false });
    m.push_back(Motion{ {-1, -2}, false });
    m.push_back(Motion{ {+1, -2}, false });
    m.push_back(Motion{ {+2, -1}, false });
  }

  void Bishop::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    for (const Offset& o : Offset::BDirection)
      m.push_back(Motion{ o, true });
  }

  void Rook::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    for (const Offset& o : Offset::RDirection)
      m.push_back(Motion{ o, true });
  }

  void Cardinal::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    Bishop::TheBishop.GetMotions(m, c);
    Knight::TheKnight.GetMotions(m, c);
  }

  void Marshall::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    Rook::TheRook.GetMotions(m, c);
    Knight::TheKnight.GetMotions(m, c);
  }

  void Queen::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    Rook::TheRook.GetMotions(m, c);
    Bishop::TheBishop.GetMotions(m, c);
  }

  void Amazon::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    Knight::TheKnight.GetMotions(m, c);
    Queen::TheQueen.GetMotions(m, c);
  }

  void King::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    for (const Offset& o : Offset::QDirection)
      m.push_back(Motion{ o, false });
  }

  void Behemoth::GetMotions(Motions& m, const PieceColor& c) const noexcept
  {
    Queen::TheQueen.GetMotions(m, c);
  }


  void Pawn::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    const PieceColor& c{ p.GetPieceIndex(l).GetColor()};
    const int dy = c == PieceColor::White ? -1 : 1;

    if (p.HasRule(AllowTakes))
    {
      // lamdba to handle the taking tests
      auto TryTake = [&](const Offset& o)
      {
        const Location& lt{ l + o };                                                      // target field of take
        const PieceColor ct{ p.GetPieceIndex(lt).GetColor() };                                 // color of piece on target
        if (ct == ~c || (p.HasRule(TakeOwn) && ct == c)) p.AddIfLegal(moves, l, lt);      // if there is a piece there, allow the pawn to take
      };

      if (p.HasRule(BerolinaPawns)) TryTake({ 0, dy });    // Berolina: takes straight
      else if (p.HasRule(LeganPawns))                      // Legan: takes left and up
      {
        TryTake({ 0, dy });
        TryTake({ dy, 0 });
      }
      else                                                 // Standard: takes diagonally left and right
      {
        TryTake({ +1, dy });
        TryTake({ -1, dy });
      }
    }

    if (p.HasRule(AllowMoves))
    {
      const Coordinate z = c == PieceColor::White ? -2 : 1;  // find double-step row
  
      // lamdba to handle the moving tests
      auto TryMove = [&](const Offset& o)
      {
        const Location& lt{ l + o };                   // target field of move
        if (!p.GetPieceIndex(lt).IsBlank()) return;         // must be blank

        bool step = p.AddIfLegal(moves, l, lt);        // pawn can move there
        if (step && p.HasRule(PawnsDoubleStep))        // double step allowed
        {
          if (l.y_ != z) return;                       // is this the double-step row?
          const Location& lt2{ l + o + o};             // target field of double-step
          if (!p.GetPieceIndex(lt2).IsBlank()) return;      // must be blank
          p.AddIfLegal(moves, l, lt2);                 // pawn can move there too
        }
      };

      if (p.HasRule(LeganPawns)) TryMove({ dy, dy });  // Legan: moves diagonally left
      else if (p.HasRule(BerolinaPawns))
      {
        TryMove({ +1, dy });                           // Berolina: moves diagonally left and right
        TryMove({ -1, dy });
      }
      else TryMove({ 0, dy });                        // Standard: moves straight
    }
  }

  void Knight::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
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
  void Bishop::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::BDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Rook::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::RDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Cardinal::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
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
  void Marshall::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
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
  void Queen::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::QDirection) for (int z = 1; p.AddIfLegal(moves, l, l + o * z); z++);
  }
  void Amazon::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
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
  void King::CollectMoves(const Board& p, const Location& l, Moves& moves) const noexcept
  {
    for (const Offset& o : Offset::QDirection) p.AddIfLegal(moves, l, l + o);

    // TODO: implement castling rules
    if (p.HasRule(CastlingShort))
    {
      p.AddIfLegal(moves, l, l + Offset{ 2, 0 });
    }
    if (p.HasRule(CastlingLong))
    {
      p.AddIfLegal(moves, l, l + Offset{ -3, 0 });
    }
  }

}
