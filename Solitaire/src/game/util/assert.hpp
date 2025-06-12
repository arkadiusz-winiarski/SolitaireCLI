#pragma once

/**
 * @file Assert.hpp
 * @brief Custom assertion macro for debug builds.
 *
 * Provides an `ASSERT` macro that prints detailed error information
 * and aborts the program if the condition is false. In release builds,
 * the macro does nothing.
 */

#include <iostream>

#ifdef _DEBUG

 /**
  * @def ASSERT(condition)
  * @brief Runtime assertion that triggers on failed conditions in debug mode.
  *
  * If `condition` evaluates to `false`, prints the failed condition, file name,
  * and line number to `std::cerr`, and then aborts the program.
  *
  * Example:
  * @code
  * ASSERT(x > 0); // Will abort if x <= 0
  * @endcode
  *
  * @param condition The condition to evaluate.
  */
#define ASSERT(condition)                                                  \
            if (!(condition)) {                                            \
                std::cerr << "Assertion failed:\n"                         \
                          << "  Condition: " << #condition << "\n"         \
                          << "  File: " << __FILE__ << "\n"                \
                          << "  Line: " << std::dec << __LINE__ << std::endl;          \
                std::abort();                                              \
            }

#else

 /**
  * @def ASSERT(condition)
  * @brief No-op version of ASSERT in release builds.
  *
  * Evaluates to a no-op, allowing code to compile without assertion checks.
  */
#define ASSERT(condition) ((void)0)

#endif
