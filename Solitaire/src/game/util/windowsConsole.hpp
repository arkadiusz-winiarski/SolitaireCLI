#pragma once
#include <Windows.h>
#include <string>

/**
 * @file windowsConsole.hpp
 * @brief Provides Windows console utility functions for color support and output.
 */

namespace WindowsConsole {

    /**
     * @brief Handle to the standard output console.
     */
    static const HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    /**
    * @brief Handle to the standard input console.
    */
    static const HANDLE stdIn = GetStdHandle(STD_INPUT_HANDLE);

    static DWORD originalOutputMode = 0;
    static DWORD originalInputMode = 0;
    static bool consoleInitialized = false;

    /**
   * @brief Initializes the original console state by saving current input and output modes.
   *
   * This function stores the current console input and output modes in static variables so that
   * they can be restored later. It is only executed once during the application's runtime.
   */
    inline void initConsoleState() {
        if (!consoleInitialized) {
            GetConsoleMode(stdOut, &originalOutputMode);
            GetConsoleMode(stdIn, &originalInputMode);
            consoleInitialized = true;
        }
    }

    /**
     * @brief Enables 24-bit (true color) ANSI escape sequences in the Windows console.
     *
     * This function enables the virtual terminal processing mode on the console output handle,
     * which allows the console to interpret ANSI escape sequences for colors and formatting.
     *
     * If the console mode cannot be retrieved or set, the function returns silently.
     */
    inline void enable24BitColors() {
        initConsoleState();
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
        {
            return;
        }

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode))
        {
            return;
        }

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hOut, dwMode))
        {
            return;
        }
    }

    /**
    * @brief Restores the original console modes for input and output.
    *
    * This function reverts the Windows console to its original state,
    * including disabling any modifications such as virtual terminal processing
    * or mouse input that were previously enabled.
    */
    inline void restoreConsole() {
        if (consoleInitialized) {
            SetConsoleMode(stdOut, originalOutputMode);
            SetConsoleMode(stdIn, originalInputMode);
        }
    }

    /**
    * @brief Checks if the console window size has changed since the last check.
    *
    * This function compares the current width and height of the console window to the last recorded size.
    * If the dimensions have changed, it updates the stored size and returns true.
    *
    * @return true if the window size has changed, false otherwise.
    */
    inline bool hasResized() {
        static SHORT lastWidth = 0;
        static SHORT lastHeight = 0;

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(stdOut, &csbi)) {
            return false;
        }

        SHORT currentWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        SHORT currentHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        if (currentWidth != lastWidth || currentHeight != lastHeight) {
            lastWidth = currentWidth;
            lastHeight = currentHeight;
            return true;
        }

        return false;
    }


    /**
     * @brief Clears the entire Windows console screen using cls
     */
    inline void clear() {
        system("cls");
    }

    /**
     * @brief Writes a wide-character string (`std::wstring`) directly to the Windows console.
     *
     * Uses the Windows API `WriteConsoleW` to output the string without any encoding conversions.
     *
     * @param str The wide string to write to the console.
     */
    inline void WriteWStringToConsole(const std::wstring& str) {
        DWORD written;
        WriteConsoleW(stdOut, str.c_str(), static_cast<DWORD>(str.length()), &written, nullptr);
    }

    /**
    * @brief Converts a wide character to a UTF-8 encoded `std::string`.
    *
    * @param wch The wide character to convert.
    * @return A UTF-8 encoded string representation of the input wide character.
    */
    inline std::string wideCharToUtf8(wchar_t wch) {
        char buf[4]{};
        int len = WideCharToMultiByte(CP_UTF8, 0, &wch, 1, buf, sizeof(buf), nullptr, nullptr);
        return std::string(buf, len);
    }

    /**
    * @brief Reads a line of UTF-8 encoded input from the console using raw input mode.
    *
    * This function processes key input events directly using `ReadConsoleInputW`, allowing for real-time
    * character processing (including backspace handling and UTF-8 conversion).
    *
    * @param echo Whether to display typed characters in the console.
    * @param externalBuffer Optional pointer to a string where the current input buffer will be copied for external use (e.g., live UI updates).
    * @return The line entered by the user as a UTF-8 encoded `std::string`.
    */
    inline std::string getLine(bool echo = true, std::string* externalBuffer = nullptr) {
        HANDLE hIn = stdIn;
        std::string input;
        INPUT_RECORD record;
        DWORD read;

        DWORD originalMode;
        GetConsoleMode(hIn, &originalMode);
        SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT);

        while (true) {
            ReadConsoleInputW(hIn, &record, 1, &read);

            if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
                auto key = record.Event.KeyEvent;
                wchar_t wch = key.uChar.UnicodeChar;

                if (wch == L'\r') { // Enter
                    if (echo) WriteWStringToConsole(L"\n");
                    break;
                }
                else if (wch == L'\b') { // Backspace
                    if (!input.empty()) {
                        // Remove last UTF-8 character
                        do {
                            input.pop_back();
                        } while (!input.empty() && (input.back() & 0xC0) == 0x80); // Handle UTF-8 multibyte

                        if (echo) {
                            WriteWStringToConsole(L"\b \b");
                        }
                    }
                }
                else if (wch >= 0x20) { // Printable
                    std::string utf8Char = wideCharToUtf8(wch);
                    input += utf8Char;
                    if (echo) {
                        WriteWStringToConsole(std::wstring(1, wch));
                    }
                }

                if (externalBuffer) {
                    *externalBuffer = input; // Allow external redraw
                }
            }
        }

        SetConsoleMode(hIn, originalMode);
        return input;
    }

    /**
    * @brief Processes available console input events asynchronously, updating the input buffer.
    *
    * This function reads keyboard events from the Windows console input buffer without blocking.
    * It updates the given input buffer with UTF-8 characters typed by the user, handles backspace,
    * and optionally echoes input characters to the console. It returns true if the Enter key was
    * pressed, indicating that the input line is complete.
    *
    * The console input mode is set once on the first call to enable window and processed input.
    *
    *  @param[out] buffer A reference to a std::string buffer that will be updated with user input.
    *                    The buffer accumulates UTF-8 encoded characters typed so far.
    * @param[in] echo If true (default), the typed characters are echoed to the console output.
    *
    * @return true if the Enter key was pressed signaling end of input line; false otherwise.
     *
     * @note This function is non-blocking and should be called repeatedly in a loop to process
    *       incoming keyboard events incrementally.
     * @note The input buffer must be maintained externally and can be cleared or reused as needed.
    * @note The console input mode enabling is done internally on the first call and not restored
    *       automatically. Consider restoring console mode on program exit if needed.
    */
    inline bool processConsoleInput(std::string& buffer, bool echo = true,bool backspaceEcho = true) {
        DWORD eventsRead = 0;
        INPUT_RECORD record;

        static bool modeSet = false;
        static DWORD originalMode = 0;
        if (!modeSet) {
            GetConsoleMode(stdIn, &originalMode);
            SetConsoleMode(stdIn, ENABLE_WINDOW_INPUT | ENABLE_PROCESSED_INPUT);
            modeSet = true;
        }

        if (!PeekConsoleInputW(stdIn, &record, 1, &eventsRead) || eventsRead == 0) {
            return false; 
        }

        while (eventsRead > 0) {
            ReadConsoleInputW(stdIn, &record, 1, &eventsRead);

            if (record.EventType == KEY_EVENT && record.Event.KeyEvent.bKeyDown) {
                wchar_t wch = record.Event.KeyEvent.uChar.UnicodeChar;

                if (wch == L'\r') { // Enter pressed
                    if (echo) WriteWStringToConsole(L"\n");
                    return true; // Input line finished
                }
                else if (wch == L'\b') { // Backspace
                    if (!buffer.empty()) {
                        // Remove last UTF-8 char
                        do {
                            buffer.pop_back();
                        } while (!buffer.empty() && (buffer.back() & 0xC0) == 0x80);

                        if (backspaceEcho) {
                            WriteWStringToConsole(L"\b \b");
                        }
                    }
                }
                else if (wch >= 0x20) { // Printable char
                    std::string utf8Char = wideCharToUtf8(wch);
                    buffer += utf8Char;
                    if (echo) {
                        WriteWStringToConsole(std::wstring(1, wch));
                    }
                }
            }

            PeekConsoleInputW(stdIn, &record, 1, &eventsRead);
        }

        return false;
    }

    /**
    * @brief Sets the console cursor position to the specified coordinates.
    *
    * @param x The horizontal position (column), starting from 0.
    * @param y The vertical position (row), starting from 0.
    */
    inline void setCursorPosition(short x, short y) {
        COORD pos = { x, y };
        SetConsoleCursorPosition(stdOut, pos);
    }

} // namespace WindowsConsole
