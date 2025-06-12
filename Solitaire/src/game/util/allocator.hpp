#pragma once

/**
 * @file Allocator.hpp
 * @brief Provides debug memory allocation tracking on Windows in debug mode.
 *
 * This header sets up memory leak detection using the CRT debug heap facilities
 * available in Visual Studio. When compiled in debug mode (`_DEBUG`), it enables
 * tracking of memory allocations and automatic leak reporting at program exit.
 */

#ifdef _WIN32

#ifdef _DEBUG

 // Enable mapping of malloc/calloc/realloc/free to their debug versions
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>

/**
 * @brief Overrides the new operator to include file and line number info for leak tracking.
 *
 * When debugging, all `new` allocations are replaced with `DEBUG_NEW`, which includes
 * source file and line number information for each allocation. This helps in identifying
 * memory leaks.
 */
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW

#endif  // _DEBUG

#endif  // _WIN32

 /**
  * @namespace Allocator
  * @brief Provides initialization for memory leak tracking in debug mode.
  */
namespace Allocator
{
    /**
     * @brief Initializes CRT debug heap settings for memory leak tracking.
     *
     * Sets flags to enable:
     * - Memory allocation tracking
     * - Automatic leak check at program termination
     * - Output of CRT debug messages to `stdout`
     *
     * Only has effect in Windows debug builds. Does nothing in release builds or on non-Windows platforms.
     */
    static void initialize()
    {
#ifdef _CRTDBG_MAP_ALLOC
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
#endif
    }
}
