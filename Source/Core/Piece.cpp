#include "Core.h"

namespace BoardGamesCore
{

  Piece::Piece(const Kind& k, const PieceColor& c, unsigned int ID) noexcept : kind_{ k }, color_{ c }, bmp_{ ID } { }

  bool Piece::operator ==(const Piece& p) const noexcept { return p.IsKind(kind_) && p.IsColor(color_); }
  bool Piece::operator !=(const Piece& p) const noexcept { return !(*this == p); }

  const Piece Piece::NoTile { noKind::NoKind, PieceColor::Void,    0 };   // nothing exists there, not even a tile
  const Piece Piece::NoPiece{ noKind::NoKind, PieceColor::NoColor, 0 };   // empty tile

  void Piece::CollectMoves(const MainPosition& p, const Location& l, Moves& m) const
  {
    kind_.CollectMoves(p, l, m);
  }

  size_t Piece::GetHash(void) const noexcept
  { 
    return kind_.GetHash() + color_.GetHash();
  }
  
  bool Piece::IsKind(const Kind& k) const noexcept
  { 
    return k == kind_;
  }
  bool Piece::IsColor(const PieceColor& c) const noexcept
  { 
    return c == color_;
  }

  bool Piece::IsBlank(void) const noexcept
  {
    return color_ == PieceColor::NoColor && kind_ == noKind::NoKind;
  }

  void Piece::Serialize(Archive& ar) const
  { 
    color_.Serialize(ar);
    kind_.Serialize(ar);
  }
  
  unsigned int Piece::GetValue(const MainPosition& p, const Location& l) const noexcept
  { 
    return kind_.GetValue(p, l);
  }

  bool Piece::CanTake(const Piece& p) const noexcept
  {
    return kind_.CanTake(p.kind_);
  }

  void Piece::Draw(DC* dc, const Rect& r) const
  {
    bmp_.Draw(dc, r);
  }

  const std::unordered_map<std::string, const Piece&>& Piece::GetHTMLPieceMap(void) noexcept
  {
    static const std::unordered_map<std::string, const Piece&> map_{
      { R"(blank35)",     Piece::NoPiece },
      //{ R"(chess/wk35)",BoardGamesChessPieces::ChessPiece::WK },
      //{ R"(chess/wq35)",BoardGamesChessPieces::ChessPiece::WQ },
      //{ R"(chess/wr35)",BoardGamesChessPieces::ChessPiece::WR },
      //{ R"(chess/wb35)",BoardGamesChessPieces::ChessPiece::WB },
      //{ R"(chess/wn35)",BoardGamesChessPieces::ChessPiece::WN },
      //{ R"(chess/wp35)",BoardGamesChessPieces::ChessPiece::WP },
      //{ R"(chess/bk35)",BoardGamesChessPieces::ChessPiece::BK },
      //{ R"(chess/bq35)",BoardGamesChessPieces::ChessPiece::BQ },
      //{ R"(chess/br35)",BoardGamesChessPieces::ChessPiece::BR },
      //{ R"(chess/bb35)",BoardGamesChessPieces::ChessPiece::BB },
      //{ R"(chess/bn35)",BoardGamesChessPieces::ChessPiece::BN },
      //{ R"(chess/bp35)",BoardGamesChessPieces::ChessPiece::BP },
      //{ R"(hshogi/w35)",Hasami::HasamiPiece::HasamiPieceW },
      //{ R"(hshogi/b35)",Hasami::HasamiPiece::HasamiPieceB },
    };
    return map_;
  }

  std::vector<const Piece*> Piece::ListFromHTML(std::string s, const std::unordered_map<std::string, const Piece&>& map_)
  {
    static const std::string s0{ R"(<div id="game-board-section">)" };
    static const std::string s1{ R"(<img src=https://i.brainking.com/)" };
    static const std::string s2{ R"(.gif)" };
    static const std::string s9{ R"(<div id="game-info-section">)" };

    std::vector<const Piece*> list{};

    std::string::size_type p1{ s.find(s0) };
    std::string::size_type p9{ s.find(s9,p1) };
    while ((p1 = s.find(s1, p1)) < p9)
    {
      p1 += s1.size();
      auto p2 = s.find(s2, p1);
      const auto& it{ map_.find(s.substr(p1, p2 - p1)) };
      if (it != map_.end()) list.push_back(&(it->second));
      else;// throw s.substr(p1, p2 - p1);
    }
    return list;
  }

}