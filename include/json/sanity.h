#pragma once

#include <csignal>
#include <cstdlib>
#include <iostream>

// clang-format off
#define MACRO_STRINGIFY_(x) #x
#define MACRO_STRINGIFY(x) MACRO_STRINGIFY_(x)
// clang-format on

// ensure DEBUG and NDEBUG (not DEBUG) is set properly ...
#if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
#undef NDEBUG
#define DEBUG
#else
#define NDEBUG
#undef DEBUG
#endif

#define GENERATE_LINE() MACRO_STRINGIFY(__LINE__)
#define GENERATE_FILE() __FILE__

template<int CODE = 0>
[[noreturn]] inline void
Exit(const char* msg)
{
#ifdef DEBUG
  std::ios_base::sync_with_stdio(false);
  std::cout.tie(nullptr);
  puts(msg);
  std::raise(SIGTRAP);
#endif
#ifdef JSON_EXIT_NOT_THROW
  std::exit(CODE);
#else
  throw std::runtime_error(msg);
#endif
}

#define ERROR(MSG) Exit<1>("\nError at " GENERATE_FILE() ":" GENERATE_LINE() "!\n    " MSG)

#define ASSERT(COND)                                                                                                                                                               \
  if (!(COND))                                                                                                                                                                     \
    ERROR("Assertion \"" #COND "\" failed");                                                                                                                                       \
  ((void)0)

#ifdef DEBUG
#define DEBUG_ASSERT(COND) ASSERT(COND)
#else
#define DEBUG_ASSERT(COND) ((void)0)
#endif
