// This header only files is used across all projects.
// Add here only functions that are minimal and platform agnostic.
// You should also test your code with gcc, clang and msvc via:
// godbolt.org
// Make sure to enable there C++20 and all warnings:
// msvc: /std:c++20 /Wall
// gcc and clang: --std=c++20 -Wall
#pragma once

#include <string>
#include <cstring>
#include <ranges>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <cwctype>
#include <cwchar>
#include <random>

#if defined(_WIN32) || defined(_WIN64)
#define VC_EXTRALEAN
#define NOMINMAX
#include <Windows.h>

// Functions mapping matching with POSIX systems
#define strcasecmp _stricmp
#define wcscasecmp _wcsicmp

#elif defined(__APPLE__)
#include <mach-o/dyld.h>

#elif defined(__linux__)
#include <unistd.h>

#endif // #if defined(_WIN32) || defined(_WIN64)

namespace fs = std::filesystem;

namespace n3std {

// Concepts
template <typename T>
concept CharType = std::disjunction_v<std::is_same<T, char>, std::is_same<T, wchar_t>, std::is_same<T, char8_t>,
                                      std::is_same<T, char16_t>, std::is_same<T, char32_t>>;

template <typename T>
concept StringType = std::is_base_of_v<std::basic_string<typename T::value_type>, T>;

template <typename T>
concept StringViewType = std::is_base_of_v<std::basic_string_view<typename T::value_type>, T>;

template <typename T>
concept StringLike = StringType<T> || StringViewType<T>;

template <typename T>
concept PathType = std::is_base_of_v<std::filesystem::path, T>;

template <typename T>
concept PathConstructible = std::constructible_from<std::filesystem::path, T>;

// Forward declaration
struct ToLower;

inline void               to_lower(StringType auto & str);
[[nodiscard]] inline auto to_lower(StringType auto && str);
inline void               to_lower(PathType auto & path);
[[nodiscard]] inline auto to_lower(PathType auto && path);

inline bool istarts_with(const StringLike auto & lhs, const StringLike auto & rhs);
inline bool iends_with(const StringLike auto & lhs, const StringLike auto & rhs);

inline bool iequals(const char * lhs, const char * rhs);
inline bool iequals(const wchar_t * lhs, const wchar_t * rhs);
inline bool iequals(const StringLike auto & lhs, const StringLike auto & rhs);
inline bool iequals(const std::filesystem::path & lhs, const std::filesystem::path & rhs);

} // namespace n3std

namespace n3std {

// Functor for character transformation
struct ToLower {
    template <CharType T> T operator()(T ch) const {
        if constexpr (std::is_same_v<T, char>) {
            return static_cast<T>(std::tolower(static_cast<unsigned char>(ch)));
        } else if constexpr (std::is_same_v<T, wchar_t>) {
            return static_cast<T>(std::towlower(ch));
        } else if constexpr (std::is_integral<T>()) {
            // Handle char8_t, char16_t, char32_t, and other integral types
            return static_cast<T>(std::towlower(static_cast<wchar_t>(ch)));
        } else {
            return ch;
        }
    }
};

inline void to_lower(StringType auto & str) {
    std::transform(str.begin(), str.end(), str.begin(), ToLower{});
}

[[nodiscard]] inline auto to_lower(StringType auto && str) {
    std::transform(str.begin(), str.end(), str.begin(), ToLower{});
    return str;
}

inline void to_lower(PathType auto & path) {
    auto & _Text = const_cast<fs::path::string_type &>(path.native());
    to_lower(_Text);
}

[[nodiscard]] inline auto to_lower(PathType auto && path) {
    auto & _Text = const_cast<fs::path::string_type &>(path.native());
    to_lower(_Text);
    return path;
}

inline bool istarts_with(const StringLike auto & lhs, const StringLike auto & rhs) {
    if (rhs.size() > lhs.size()) {
        return false;
    }

    auto transform_lower = std::ranges::views::transform(ToLower{});
    return std::ranges::equal(lhs | std::views::take(static_cast<int>(rhs.size())) | transform_lower,
                              rhs | transform_lower);
}

inline bool iends_with(const StringLike auto & lhs, const StringLike auto & rhs) {
    if (rhs.size() > lhs.size()) {
        return false;
    }

    auto transform_lower = std::ranges::views::transform(ToLower{});
    return std::ranges::equal(lhs | std::views::drop(static_cast<int>(lhs.size() - rhs.size())) | transform_lower,
                              rhs | transform_lower);
}

inline bool iequals(const char * lhs, const char * rhs) {
    return strcasecmp(lhs, rhs) == 0;
}

inline bool iequals(const wchar_t * lhs, const wchar_t * rhs) {
    return wcscasecmp(lhs, rhs) == 0;
}

inline bool iequals(const StringLike auto & lhs, const StringLike auto & rhs) {
    if (rhs.size() != lhs.size()) {
        return false;
    }

    auto transform_lower = std::ranges::views::transform(ToLower{});
    return std::ranges::equal(lhs | transform_lower, rhs | transform_lower);
}

// Note that fs::path constructors supports implicit conversions from wide-character
// strings to narrow and vice versa. Therefore this overload will be invoked for most scenarios
// where both lhs and rhs types are not the same. This also means that a copy of the object will
// be created to unify both types before comparison.
inline bool iequals(const std::filesystem::path & lhs, const std::filesystem::path & rhs) {
    return iequals(lhs.c_str(), rhs.c_str());
}

inline fs::path get_app_path() {
    static fs::path app_path = [] {
        fs::path::value_type buff[512]{};
#if defined(_WIN32) || defined(_WIN64)
        GetModuleFileNameW(NULL, buff, static_cast<DWORD>(std::size(buff)));
#elif defined(__APPLE__)
        _NSGetExecutablePath(buff, (uint32_t *)sizeof(buff));
#elif defined(__linux__)
        readlink("/proc/self/exe", buff, sizeof(buff) - 1);
#endif // #if defined(_WIN32) || defined(_WIN64)
        return fs::path(buff);
    }();

    return app_path;
}

inline fs::path get_app_dir() {
    static fs::path app_dir = get_app_path().parent_path();

    return app_dir;
}

inline void log_file_write(std::string_view log_msg) {
    static fs::path log_path = get_app_path().replace_extension(".log");

    std::ofstream ofile(log_path, std::ios::app);
    if (ofile.is_open()) {
        ofile << log_msg;
    }
}

inline void log_file_write(const char * log_msg) {
    log_file_write(std::string_view(log_msg));
}

static std::string bytes_to_hex(const uint8_t * const data, const size_t size) {
    if (!data || size == 0) {
        return "<null>";
    }

    static const char * hex_digits = "0123456789ABCDEF";

    std::string result;
    result.reserve(size * 2);
    for (size_t i = 0; i < size; ++i) {
        result += hex_digits[(data[i] & 0xF0) >> 4];
        result += hex_digits[data[i] & 0x0F];
    }

    return result;
}

} // namespace n3std
