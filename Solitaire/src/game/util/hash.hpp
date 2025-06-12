#pragma once
#include <string>
#include <string_view>

/**
 * @file hash_util.hpp
 * @brief Provides constexpr and runtime FNV-1a hash functions for strings and character arrays.
 */

 /**
  * @brief Computes the length of a null-terminated string at compile-time.
  * @param data Null-terminated C-string.
  * @return Length of the string excluding the null terminator.
  */
static inline constexpr unsigned int len(const char* data)
{
    unsigned int count = 0;
    while (data[count] != '\0') ++count;
    return count;
}

/**
 * @brief Computes the FNV-1a hash of a byte sequence at compile-time.
 * @param data Pointer to the input data.
 * @param len Length of the input data.
 * @return 32-bit hash value.
 */
static inline constexpr unsigned int hash(const char* data, unsigned int len)
{
    constexpr unsigned int basis = 0x811c9dc5;
    constexpr unsigned int prime = 0x01000193;

    unsigned int hash = basis;

    for (unsigned int i = 0; i < len; i++)
    {
        hash ^= static_cast<unsigned char>(data[i]);
        hash *= prime;
    }

    return hash;
}

/**
 * @brief Computes the FNV-1a hash of a null-terminated C-string at compile-time.
 * @param data Null-terminated C-string.
 * @return 32-bit hash value.
 */
static inline constexpr unsigned int hash(const char* data)
{
    return hash(data, len(data));
}

/**
 * @brief Computes the FNV-1a hash of a std::string_view at runtime.
 * @param sv The input string view.
 * @return 32-bit hash value.
 */
static inline unsigned int hash(std::string_view sv)
{
    return hash(sv.data(), static_cast<unsigned int>(sv.size()));
}

/**
 * @brief Computes the FNV-1a hash of a std::string at runtime.
 * @param s The input string.
 * @return 32-bit hash value.
 */
static inline unsigned int hash(const std::string& s)
{
    return hash(s.data(), static_cast<unsigned int>(s.size()));
}
