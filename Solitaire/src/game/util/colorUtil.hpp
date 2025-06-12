/**
 * @file colorUtil.hpp
 * @brief Utility functions and constants for ANSI terminal color codes (RGB and styles).
 *
 * This header provides definitions for ANSI escape codes,
 * including foreground and background RGB colors,
 * and text style modifiers like bold, italic, underline, etc.
 */
#pragma once
#include <string>

namespace ColorUtil {

    /// ANSI escape code to reset formatting
    const std::wstring RESET = L"\033[0m";

    /// ANSI escape code for bold text
    const std::wstring BOLD = L"\033[1m";

    /// ANSI escape code for dim text
    const std::wstring DIM = L"\033[2m";

    /// ANSI escape code for italic text
    const std::wstring ITALIC = L"\033[3m";

    /// ANSI escape code for underline text
    const std::wstring UNDERLINE = L"\033[4m";

    /// ANSI escape code for blinking text
    const std::wstring BLINK = L"\033[5m";

    /// ANSI escape code for reverse video
    const std::wstring REVERSE = L"\033[7m";

    /// ANSI escape code for hidden text
    const std::wstring HIDDEN = L"\033[8m";

    /// ANSI escape code for strikethrough text
    const std::wstring STRIKETHROUGH = L"\033[9m";

    /**
     * @brief Represents an RGB color.
     */
    struct RGB {
        int r; ///< Red component (0-255)
        int g; ///< Green component (0-255)
        int b; ///< Blue component (0-255)
    };

    /**
     * @brief Converts an RGB color to an ANSI escape sequence for foreground color (std::string).
     * @param color RGB color struct
     * @return ANSI escape sequence string for setting foreground color
     */
    inline std::string rgbToForeground(const RGB& color) {
        return "\x1b[38;2;" + std::to_string(color.r) + ";" +
            std::to_string(color.g) + ";" +
            std::to_string(color.b) + "m";
    }

    /**
     * @brief Converts an RGB color to an ANSI escape sequence for foreground color (std::wstring).
     * @param color RGB color struct
     * @return ANSI escape sequence wide string for setting foreground color
     */
    inline std::wstring wrgbToForeground(const RGB& color) {
        return L"\x1b[38;2;" + std::to_wstring(color.r) + L";" +
            std::to_wstring(color.g) + L";" +
            std::to_wstring(color.b) + L"m";
    }

    /**
     * @brief Converts an RGB color to an ANSI escape sequence for background color (std::string).
     * @param color RGB color struct
     * @return ANSI escape sequence string for setting background color
     */
    inline std::string rgbToBackground(const RGB& color) {
        return "\x1b[48;2;" + std::to_string(color.r) + ";" +
            std::to_string(color.g) + ";" +
            std::to_string(color.b) + "m";
    }

    /**
     * @brief Converts an RGB color to an ANSI escape sequence for background color (std::wstring).
     * @param color RGB color struct
     * @return ANSI escape sequence wide string for setting background color
     */
    inline std::wstring wrgbToBackground(const RGB& color) {
        return L"\x1b[48;2;" + std::to_wstring(color.r) + L";" +
            std::to_wstring(color.g) + L";" +
            std::to_wstring(color.b) + L"m";
    }

    /**
     * @brief Generates an ANSI escape sequence for both foreground and background colors (std::string).
     * @param fg Foreground RGB color struct
     * @param bg Background RGB color struct
     * @return ANSI escape sequence string setting both foreground and background colors
     */
    inline std::string rgbBoth(const RGB& fg, const RGB& bg) {
        return "\033[38;2;" + std::to_string(fg.r) + ";" + std::to_string(fg.g) + ";" + std::to_string(fg.b) +
            ";48;2;" + std::to_string(bg.r) + ";" + std::to_string(bg.g) + ";" + std::to_string(bg.b) + "m";
    }

    /**
     * @brief Generates an ANSI escape sequence for both foreground and background colors (std::wstring).
     * @param fg Foreground RGB color struct
     * @param bg Background RGB color struct
     * @return ANSI escape sequence wide string setting both foreground and background colors
     */
    inline std::wstring wrgbBoth(const RGB& fg, const RGB& bg) {
        return L"\033[38;2;" + std::to_wstring(fg.r) + L";" + std::to_wstring(fg.g) + L";" + std::to_wstring(fg.b) +
            L";48;2;" + std::to_wstring(bg.r) + L";" + std::to_wstring(bg.g) + L";" + std::to_wstring(bg.b) + L"m";
    }

    /**
     * @brief Converts RGB components to ANSI foreground escape code (std::wstring).
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @return ANSI escape sequence wide string for setting foreground color
     */
    inline std::wstring wrgbToForeground(int r, int g, int b) {
        return L"\x1b[38;2;" + std::to_wstring(r) + L";" +
            std::to_wstring(g) + L";" +
            std::to_wstring(b) + L"m";
    }

    /**
     * @brief Converts RGB components to ANSI background escape code (std::wstring).
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @return ANSI escape sequence wide string for setting background color
     */
    inline std::wstring wrgbToBackground(int r, int g, int b) {
        return L"\x1b[48;2;" + std::to_wstring(r) + L";" +
            std::to_wstring(g) + L";" +
            std::to_wstring(b) + L"m";
    }

    /**
     * @brief Converts RGB components to ANSI foreground escape code (std::string).
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @return ANSI escape sequence string for setting foreground color
     */
    inline std::string wrgbToForegroundString(int r, int g, int b) {
        return "\x1b[38;2;" + std::to_string(r) + ";" +
            std::to_string(g) + ";" +
            std::to_string(b) + "m";
    }

    /**
     * @brief Converts RGB components to ANSI background escape code (std::string).
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     * @return ANSI escape sequence string for setting background color
     */
    inline std::string rgbToBackground(int r, int g, int b) {
        return "\x1b[48;2;" + std::to_string(r) + ";" +
            std::to_string(g) + ";" +
            std::to_string(b) + "m";
    }

    /**
     * @brief Generates an ANSI escape sequence for both foreground and background colors from RGB components (std::string).
     * @param fg_r Foreground red component (0-255)
     * @param fg_g Foreground green component (0-255)
     * @param fg_b Foreground blue component (0-255)
     * @param bg_r Background red component (0-255)
     * @param bg_g Background green component (0-255)
     * @param bg_b Background blue component (0-255)
     * @return ANSI escape sequence string setting both foreground and background colors
     */
    inline std::string rgbBoth(int fg_r, int fg_g, int fg_b, int bg_r, int bg_g, int bg_b) {
        return "\033[38;2;" + std::to_string(fg_r) + ";" + std::to_string(fg_g) + ";" + std::to_string(fg_b) +
            ";48;2;" + std::to_string(bg_r) + ";" + std::to_string(bg_g) + ";" + std::to_string(bg_b) + "m";
    }

    /**
     * @brief Generates an ANSI escape sequence for both foreground and background colors from RGB components (std::wstring).
     * @param fg_r Foreground red component (0-255)
     * @param fg_g Foreground green component (0-255)
     * @param fg_b Foreground blue component (0-255)
     * @param bg_r Background red component (0-255)
     * @param bg_g Background green component (0-255)
     * @param bg_b Background blue component (0-255)
     * @return ANSI escape sequence wide string setting both foreground and background colors
     */
    inline std::wstring wrgbBoth(int fg_r, int fg_g, int fg_b, int bg_r, int bg_g, int bg_b) {
        return L"\033[38;2;" + std::to_wstring(fg_r) + L";" + std::to_wstring(fg_g) + L";" + std::to_wstring(fg_b) +
            L";48;2;" + std::to_wstring(bg_r) + L";" + std::to_wstring(bg_g) + L";" + std::to_wstring(bg_b) + L"m";
    }

    /**
 * @brief Converts HSV color to RGB color.
 *
 * @param h Hue component in range [0.0, 1.0]
 * @param s Saturation component in range [0.0, 1.0]
 * @param v Value (brightness) component in range [0.0, 1.0]
 * @return RGB Struct containing RGB values [0-255]
 */
    inline RGB HSVtoRGB(float h, float s, float v) {
        float r = 0, g = 0, b = 0;

        int i = static_cast<int>(h * 6.0f);
        float f = h * 6.0f - i;
        float p = v * (1.0f - s);
        float q = v * (1.0f - f * s);
        float t = v * (1.0f - (1.0f - f) * s);

        switch (i % 6) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
        }

        return RGB{
            static_cast<int>(r * 255.0f),
            static_cast<int>(g * 255.0f),
            static_cast<int>(b * 255.0f)
        };
    }

    /**
    * @brief Generates a rainbow RGB color based on a tick value.
    *
    * @param tick A time-based or position-based counter to animate or shift the rainbow.
    * @param speed The speed at which the rainbow color cycles. Higher = slower transition.
    * @param brightness The brightness (value in HSV) of the resulting color [0.0f - 1.0f].
    * @return RGB color representing the current rainbow hue at full saturation.
    */
    inline RGB getRainbowColor(int tick, int speed = 100, float saturation = 1.0f, float brightness = 1.0f) {
        float hue = static_cast<float>(tick % speed) / static_cast<float>(speed);
        return HSVtoRGB(hue, saturation, brightness); 
    }
}
