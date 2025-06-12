#pragma once
#include "common.hpp"
#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <filesystem>

/**
 * @file fs.hpp
 * @brief Provides BufferedFileIO
 */

namespace BufferedIO {

    /**
   * @brief Checks if a file exists.
   *
   * Uses standard C++17 filesystem API to check file existence.
   *
   * @param filename Path to the file.
   * @return true if the file exists, false otherwise.
   */
    inline bool fileExists(const std::string& filename) {
        return std::filesystem::exists(filename);
    }

    /**
    * @brief Checks if a given filename string is valid for saving a file.
    *
    * This function validates the filename according to common restrictions:
    * - Must not be empty.
    * - Must not contain illegal characters (such as \ / : * ? " < > |).
    * - Must not consist only of spaces or dots.
    * - Must not exceed typical filename length limits (255 characters).
    *
    * @param filename The filename string to validate.
    * @return true If the filename is valid and good to use for saving a file.
    * @return false If the filename is invalid.
    *
    * @note This function performs basic validation primarily suitable for Windows filenames.
    *       Additional OS-specific checks (like reserved names) are not included.
    */
    inline bool isValidFilename(const std::string& filename) {
        if (filename.empty()) return false;

        const std::string illegalChars = "\\/:*?\"<>|";

        if (filename.find_first_of(illegalChars) != std::string::npos) {
            return false;
        }

        bool allSpacesOrDots = true;
        for (char ch : filename) {
            if (ch != ' ' && ch != '.') {
                allSpacesOrDots = false;
                break;
            }
        }
        if (allSpacesOrDots) return false;
        if (filename.size() > 255) return false;
        return true;
    }

    /**
     * @class BufferedFileReader
     * @brief Buffered reader for binary files with integer and line reading.
     *
     * This class opens a file for reading in binary mode and buffers reads
     * for efficiency. It provides methods to read lines and 32-bit integers.
     * On read errors, methods return -1 or false, no exceptions thrown.
     */
    class BufferedFileReader {
    public:
        /**
         * @brief Constructs a BufferedFileReader and opens the file.
         * @param filename Path to the file to open for reading.
         * @param bufferSize Size of the internal read buffer (default 8192 bytes).
         */
        explicit BufferedFileReader(const std::string& filename, std::size_t bufferSize = 8192)
            : bufferSize_(bufferSize), buffer_(bufferSize)
        {
            if (!std::filesystem::exists(filename)) {
                std::ofstream createFile(filename, std::ios::out | std::ios::binary);
            }
            input_.open(filename, std::ios::in | std::ios::binary);
            if (input_) {
                input_.rdbuf()->pubsetbuf(buffer_.data(), bufferSize_);
                open_ = true;
            }
            else {
                open_ = false;
            }
        }

        /**
         * @brief Checks if the file was successfully opened.
         * @return true if open, false otherwise.
         */
        bool isOpen() const { return open_; }

        /**
         * @brief Reads a line from the file.
         * @param line Output string to hold the line read.
         * @return true if a line was read successfully, false on EOF or error.
         */
        bool readLine(std::string& line) {
            ASSERT(open_);
            if (!open_) return false;
            if (!std::getline(input_, line)) return false;
            return true;
        }

        /**
         * @brief Reads up to maxBytes bytes into data buffer.
         * @param data Pointer to destination buffer.
         * @param maxBytes Maximum bytes to read.
         * @return Number of bytes read, or -1 if error.
         */
        std::streamsize read(char* data, std::streamsize maxBytes) {
            ASSERT(open_);
            if (!open_) return -1;
            if (!input_.read(data, maxBytes)) {
                return input_.gcount();
            }
            return maxBytes;
        }

        /**
         * @brief Reads a 32-bit integer from the file.
         *
         * Reads 4 bytes in little-endian order.
         *
         * @return The integer read, or -1 if failed (EOF or error).
         */
        int32_t readInt() {
            ASSERT(open_);
            if (!open_) return -1;
            char buffer[sizeof(int32_t)];
            if (!input_.read(buffer, sizeof(int32_t))) {
                return -1;
            }
            // Assemble little-endian int32_t
            int32_t value =
                (static_cast<uint8_t>(buffer[0])) |
                (static_cast<uint8_t>(buffer[1]) << 8) |
                (static_cast<uint8_t>(buffer[2]) << 16) |
                (static_cast<uint8_t>(buffer[3]) << 24);
            return value;
        }

        /**
        * @brief Reads a boolean value from the file.
        *
        * Reads 1 byte and interprets zero as false, non-zero as true.
        *
        * @return 1 for true, 0 for false, -1 on failure.
        */
        int readBool() {
            ASSERT(open_);
            if (!open_) return -1;
            char val;
            if (!input_.read(&val, 1)) {
                return -1;
            }
            return (val != 0) ? 1 : 0;
        }
        /**
         * @brief Checks if end-of-file has been reached.
         * @return true if EOF reached, false otherwise.
         */
        bool eof() const { return input_.eof(); }

    private:
        std::ifstream input_;
        bool open_ = false;
        std::size_t bufferSize_;
        std::vector<char> buffer_;
    };

    /**
     * @class BufferedFileWriter
     * @brief Buffered writer for binary files with integer and line writing.
     *
     * This class opens a file for writing in binary mode and buffers writes
     * for efficiency. It provides methods to write lines and 32-bit integers.
     */
    class BufferedFileWriter {
    public:
        /**
         * @brief Constructs a BufferedFileWriter and opens the file.
         * @param filename Path to the file to open for writing.
         * @param bufferSize Size of the internal write buffer (default 8192 bytes).
         */
        explicit BufferedFileWriter(const std::string& filename, std::size_t bufferSize = 8192)
            : bufferSize_(bufferSize), buffer_(bufferSize)
        {
            output_.open(filename, std::ios::out | std::ios::binary);
            if (output_) {
                output_.rdbuf()->pubsetbuf(buffer_.data(), bufferSize_);
                open_ = true;
            }
            else {
                open_ = false;
            }
        }

        /**
         * @brief Checks if the file was successfully opened.
         * @return true if open, false otherwise.
         */
        bool isOpen() const { return open_; }

        /**
         * @brief Writes a line (string + newline) to the file.
         * @param line The string to write.
         */
        void writeLine(const std::string& line) {
            ASSERT(open_);
            if (!open_) return;
            output_ << line << '\n';
        }

        /**
         * @brief Writes raw bytes to the file.
         * @param data Pointer to bytes to write.
         * @param size Number of bytes to write.
         */
        void write(const char* data, std::streamsize size) {
            ASSERT(open_);
            if (!open_) return;
            output_.write(data, size);
        }

        /**
         * @brief Writes a 32-bit integer to the file in little-endian order.
         * @param value The integer to write.
         * @return 0 on success, -1 on failure.
         */
        int writeInt(int32_t value) {
            ASSERT(open_);
            if (!open_) return -1;
            char buffer[sizeof(int32_t)];
            buffer[0] = static_cast<char>(value & 0xFF);
            buffer[1] = static_cast<char>((value >> 8) & 0xFF);
            buffer[2] = static_cast<char>((value >> 16) & 0xFF);
            buffer[3] = static_cast<char>((value >> 24) & 0xFF);
            output_.write(buffer, sizeof(int32_t));
            return output_ ? 0 : -1;
        }


        /**
         * @brief Writes a boolean value to the file.
         *
         * Writes 1 byte: 1 for true, 0 for false.
         *
         * @param value Boolean value to write.
         * @return 0 on success, -1 on failure.
         */
        int writeBool(bool value) {
            ASSERT(open_);
            if (!open_) return -1;
            char val = value ? 1 : 0;
            output_.write(&val, 1);
            return output_ ? 0 : -1;
        }

        /**
         * @brief Flushes the internal buffer to the file.
         */
        void flush() {
            ASSERT(open_);
            if (!open_) return;
            output_.flush();
        }

    private:
        std::ofstream output_;
        bool open_ = false;
        std::size_t bufferSize_;
        std::vector<char> buffer_;
    };

} // namespace bfio