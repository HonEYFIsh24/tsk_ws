#ifndef SRC_TEST_TOOL_TYPES_H_
#define SRC_TEST_TOOL_TYPES_H_
#include <chrono>
#include <map>
#include "deptrum/deptrum.hpp"

typedef std::chrono::high_resolution_clock::time_point TimePoint;
#define TIMESTART std::chrono::high_resolution_clock::now()
#define TIMEELAPSED(x) std::chrono::duration_cast<std::chrono::milliseconds>(TIMESTART - x).count()
#define CHECK_SDK_RETURN_VALUE(ret)                    \
  do {                                                 \
    int val = ret;                                     \
    if (val != 0) {                                    \
      std::cout << "error code: " << val << std::endl; \
      return val;                                      \
    }                                                  \
  } while (0)

#endif