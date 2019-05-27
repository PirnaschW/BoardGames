#include "stdafx.h"

namespace BoardGamesCore
{

  const std::unordered_map<std::string, const Piece*>& Piece::GetHTMLPieceMap(void) noexcept
  {
    static const std::unordered_map<std::string, const Piece*> map{
      { R"(blank35)",     &Piece::NoPiece },
      { R"(chess/wk35)",&BoardGamesChessPieces::ChessPiece::WK },
      { R"(chess/wq35)",&BoardGamesChessPieces::ChessPiece::WQ },
      { R"(chess/wr35)",&BoardGamesChessPieces::ChessPiece::WR },
      { R"(chess/wb35)",&BoardGamesChessPieces::ChessPiece::WB },
      { R"(chess/wn35)",&BoardGamesChessPieces::ChessPiece::WN },
      { R"(chess/wp35)",&BoardGamesChessPieces::ChessPiece::WP },
      { R"(chess/bk35)",&BoardGamesChessPieces::ChessPiece::BK },
      { R"(chess/bq35)",&BoardGamesChessPieces::ChessPiece::BQ },
      { R"(chess/br35)",&BoardGamesChessPieces::ChessPiece::BR },
      { R"(chess/bb35)",&BoardGamesChessPieces::ChessPiece::BB },
      { R"(chess/bn35)",&BoardGamesChessPieces::ChessPiece::BN },
      { R"(chess/bp35)",&BoardGamesChessPieces::ChessPiece::BP },
      //{ R"(hshogi/w35)",&Hasami::HasamiPiece::HasamiPieceW },
      //{ R"(hshogi/b35)",&Hasami::HasamiPiece::HasamiPieceB },
    };
    return map;
  }


  std::vector<const Piece*> Piece::ListFromHTML(std::string s, const std::unordered_map<std::string, const Piece*>& map)
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
      const auto& it{ map.find(s.substr(p1, p2 - p1)) };
      if (it != map.end()) list.push_back(it->second);
      else;// throw s.substr(p1, p2 - p1);
    }
    return list;
  }

  const Piece Piece::NoTile{ &noKind::NoKind, &Color::NoColor, 0, 0, 0 };               // nothing exists there, don't draw the tile at all
  const Piece Piece::NoPiece{ &noKind::NoKind, &Color::NoColor, IDB_XXL, IDB_XXD, 0 };   // no piece on the tile, but still draw it

}