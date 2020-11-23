#include "Core.h"

namespace BoardGamesCore
{

  static_assert(!std::is_abstract<Location>::value, "must not be abstract");
  static_assert(!std::is_trivially_constructible<class Location>::value, "must not be trivially constructible");
  //  static_assert(!std::is_constructible<class Location, int, int>::value, "must not be 'int' constructible");
  static_assert(std::is_constructible<class Location, Location::BoardPart, Coordinate, Coordinate>::value, "is not constructible");
  //  static_assert(std::is_nothrow_constructible<class Offset, Location::BoardPart, Coordinate, Coordinate>::value, "is not nothrow constructible");

}
