#include "Games.h"

#include "CheckersGame.h"

namespace Checkers
{

  void ItalianCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
          SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
        else
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
      }
    }
  }


  void PortugueseCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
          SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
        else
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
      }
    }
  }


  void CornerCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
        {
          if ((i + j + 1) < sizeX_)
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
          else if ((i + j) > sizeX_)
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceW);
        }
      }
    }
  }

  void OneWayCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 2; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
        {
          if (i < sizeX_ / 2)
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
          else
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceW);
        }
      }
    }
  }

  void GothicCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 4; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
        SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
      }
    }
  }

  void ThaiCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 4; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
        else
          SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
      }
    }
  }

  void DameoCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    {
      for (Coordinate i = j; i < sizeX_ - j; i++)
      {
        SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
        SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
      }
    }
  }

  void HawaiianCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceW);
        else
          SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
      }
    }
  }

  void ParachuteCheckersPosition::SetStartingPosition() noexcept
  {
    for (Coordinate j = 0; j < sizeY_ / 2 - 1; j++)
    {
      for (Coordinate i = 0; i < sizeX_; i++)
      {
        if ((i + j) % 2)
        {
          if (j == 0)
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersParaW);
          else
            SetPiece(Location(BoardPart::Main, i, j), CheckersPiece::CheckersPieceB);
        }
        else
        {
          if (j == 0)
            SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersParaB);
          else
            SetPiece(Location(BoardPart::Main, i, sizeY_ - 1 - j), CheckersPiece::CheckersPieceW);
        }
      }
    }
  }



}


