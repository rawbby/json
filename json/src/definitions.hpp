#pragma once

#include <cassert>
#include <csignal>
#include <signal.h>

#define STEP switch (is.get()) {
#define STEP_ \
  default:    \
    fail();   \
    }
#define STEPS \
  for (;;) {  \
    STEP
#define STEPS_ \
  STEP_        \
  }
#define FAIL()                 \
  {                            \
    const auto p = is.tellg(); \
    return fail();             \
  }                            \
  ((void)0)

inline int
fail()
{
#ifndef NDEBUG
  raise(SIGTRAP);
#endif
  return 1;
}
