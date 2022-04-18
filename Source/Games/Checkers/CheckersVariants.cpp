#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{

  void ItalianCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //      SetPieceIndex(CheckersPiece::CheckersPieceW, i, sizeY_ - 1 - j);
    //    else
    //      SetPieceIndex(CheckersPiece::CheckersPieceB, i, j);
    //  }
    //}
  }


  void PortugueseCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //      SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //    else
    //      SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //  }
    //}
  }


  void CornerCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //    {
    //      if ((i + j + 1) < sizeX_)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //      else if ((i + j) > sizeX_)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceW);
    //    }
    //  }
    //}
  }

  void OneWayCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 2; j < sizeY_; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //    {
    //      if (i < sizeX_ / 2)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //      else
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceW);
    //    }
    //  }
    //}
  }

  void GothicCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 4; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //    SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //  }
    //}
  }

  void ThaiCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 4; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //      SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //    else
    //      SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //  }
    //}
  }

  void DameoCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    //{
    //  for (Coordinate i = j; i < sizeX_ - j; i++)
    //  {
    //    SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //    SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //  }
    //}
  }

  void HawaiianCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //      SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceW);
    //    else
    //      SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //  }
    //}
  }

  void ParachuteCheckersBoard::SetStartingBoard() noexcept
  {
    //for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    //{
    //  for (Coordinate i = 0; i < sizeX_; i++)
    //  {
    //    if ((i + j) % 2)
    //    {
    //      if (j == 0)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersParaW);
    //      else
    //        SetPieceIndex(Location(BoardPartID::Stage, i, j), CheckersPiece::CheckersPieceB);
    //    }
    //    else
    //    {
    //      if (j == 0)
    //        SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersParaB);
    //      else
    //        SetPieceIndex(Location(BoardPartID::Stage, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
    //    }
    //  }
    //}
  }



}


