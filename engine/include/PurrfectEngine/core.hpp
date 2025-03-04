#ifndef   _PURRFECT_ENGINE_CORE_HPP_
#define   _PURRFECT_ENGINE_CORE_HPP_

#include <assert.h>

#define PE_ASSERT(cond, msg) assert(cond && msg)

namespace PurrfectEngine {

  template <typename T>
  constexpr T max(T a, T b) { return a > b ? a : b; }

  template <typename T>
  constexpr T min(T a, T b) { return a < b ? a : b; }

}

#endif // _PURRFECT_ENGINE_CORE_HPP_