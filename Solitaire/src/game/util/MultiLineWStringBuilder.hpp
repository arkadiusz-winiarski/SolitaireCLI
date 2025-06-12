#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <regex>

/**
 * @file MultiLineWStringBuilder.hpp
 * @brief Provides a class to build multi-line wide strings with ANSI color codes per character.
 */

 /**
  * @class MultiLineWStringBuilder
  * @brief Builds and manages multi-line wide strings supporting character-level ANSI color codes.
  *
  * This class stores multiple lines of wide characters (`std::wstring`) along with
  * separate color layers for each character, allowing fine-grained control of color output.
  * It supports parsing of embedded ANSI escape sequences and renders the combined
  * colored string with minimal reset codes.
  */
class MultiLineWStringBuilder {
    std::vector<std::wstring> lines; ///< Stores lines of characters.
    std::vector<std::vector<std::wstring>> colorLayers; ///< Stores ANSI color codes per character.
    const std::wstring resetCode; ///< ANSI code to reset colors to default.

    /**
     * @brief Ensures the internal buffers have sufficient size for given position and length.
     *
     * Resizes the `lines` and `colorLayers` vectors to accommodate at least `x + length`
     * characters on line `y`.
     *
     * @param x Horizontal start position.
     * @param y Vertical line index.
     * @param length Number of characters to accommodate starting at `x`.
     */
    void ensureSize(int x, int y, int length) {
        if (y >= (int)lines.size()) {
            lines.resize(y + 1);
            colorLayers.resize(y + 1);
        }

        if ((int)lines[y].size() < x + length) {
            lines[y].resize(x + length, L' ');
            colorLayers[y].resize(x + length, L"");
        }
    }

    /**
     * @brief Checks if a string contains ANSI escape sequences.
     * @param str Input wide string to check.
     * @return True if the string contains any ANSI escape sequence, false otherwise.
     */
    bool containsAnsi(const std::wstring& str) const {
        return str.find(L"\033[") != std::wstring::npos;
    }

    /**
     * @brief Parses a wide string containing ANSI escape sequences into characters and their colors.
     *
     * Extracts characters from the input string along with the current ANSI color code
     * that applies to each character.
     *
     * @param str Input wide string possibly containing ANSI sequences.
     * @return Vector of pairs, each containing a character and the corresponding ANSI color code.
     */
    std::vector<std::pair<wchar_t, std::wstring>> parseAnsi(const std::wstring& str) {
        std::vector<std::pair<wchar_t, std::wstring>> result;
        std::wstring currentColor;
        size_t i = 0;

        while (i < str.size()) {
            if (str[i] == L'\033' && i + 1 < str.size() && str[i + 1] == L'[') {
                size_t j = i + 2;

                while (j < str.size() && (iswdigit(str[j]) || str[j] == L';')) {
                    ++j;
                }

                if (j < str.size() && ((str[j] >= L'A' && str[j] <= L'z'))) {
                    currentColor = str.substr(i, j - i + 1);
                    i = j + 1;
                    continue;
                }
            }

            result.emplace_back(str[i], currentColor);
            ++i;
        }

        return result;
    }

public:
    /**
     * @brief Default constructor initializes with the ANSI reset code "\033[0m".
     */
    MultiLineWStringBuilder() : resetCode(L"\033[0m") {}

    /**
     * @brief Constructor with a custom ANSI reset code.
     * @param resetCode Custom ANSI reset sequence to use.
     */
    MultiLineWStringBuilder(std::wstring resetCode) : resetCode(resetCode) {}

    /**
     * @brief Sets a substring starting at position (x, y) with optional embedded ANSI color codes.
     *
     * If the input string contains ANSI sequences, it is parsed and colors applied per character.
     * Otherwise, the string is placed plainly with no color.
     *
     * @param x Horizontal start position (0-based).
     * @param y Vertical line index (0-based).
     * @param str The string to place.
     */
    void set(int x, int y, const std::wstring& str) {
        if (x < 0 || y < 0) return;

        if (containsAnsi(str)) {
            auto parsed = parseAnsi(str);
            ensureSize(x, y, parsed.size());

            for (size_t i = 0; i < parsed.size(); ++i) {
                lines[y][x + i] = parsed[i].first;
                colorLayers[y][x + i] = parsed[i].second;
            }
        }
        else {
            ensureSize(x, y, str.size());

            for (size_t i = 0; i < str.size(); ++i) {
                lines[y][x + i] = str[i];
                colorLayers[y][x + i] = L"";
            }
        }
    }

    /**
     * @brief Sets a substring starting at position (x, y) with a uniform ANSI color code.
     *
     * Places the characters in the line and sets the entire substring to the same color code.
     *
     * @param x Horizontal start position (0-based).
     * @param y Vertical line index (0-based).
     * @param str The string to place.
     * @param ansiColorCode ANSI color code to apply to all characters in `str`.
     */
    void colorSet(int x, int y, const std::wstring& str, const std::wstring& ansiColorCode) {
        if (x < 0 || y < 0) return;

        ensureSize(x, y, str.size());

        for (size_t i = 0; i < str.size(); ++i) {
            lines[y][x + i] = str[i];
            colorLayers[y][x + i] = ansiColorCode;
        }
    }

    /**
     * @brief Converts the stored lines and color layers into a single wide string with embedded ANSI codes.
     *
     * Minimizes the use of reset codes by only inserting color changes when necessary.
     * Appends newline characters between lines.
     *
     * @return A wide string representing the fully colorized text.
     */
    std::wstring str() const {
        std::wstring result;

        for (size_t y = 0; y < lines.size(); ++y) {
            const auto& line = lines[y];
            const auto& colorLine = colorLayers[y];

            std::wstring currentColor = L"";

            for (size_t x = 0; x < line.size(); ++x) {
                const std::wstring& thisColor = colorLine[x];

                if (thisColor != currentColor) {
                    if (!thisColor.empty()) {
                        result += thisColor;
                    }
                    else if (!currentColor.empty()) {
                        result += resetCode;
                    }
                    currentColor = thisColor;
                }
                result += line[x];
            }

            if (!currentColor.empty())
                result += resetCode;

            result += L"\n";
        }

        return result;
    }
};
