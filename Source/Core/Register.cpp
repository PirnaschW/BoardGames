#include "Core.h"

#include "Register.h"

struct Base : Factory<Base, int> {
  Base(Key) {}
  virtual ~Base() = default;
};

