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

// Enables automatic path normalization during concatenation with `operator/` or `operator/=`.
// This removes the need to manually call the normalize function to standardize path separators.
// On Windows, both `\` and `/` separators are valid and mixable, but this can lead to inconsistent paths.
// Caution: This adds overhead by copying paths via `generic_string()`, reconstructing paths anew.
// Note: Most KO editors save paths with Windows `\`, so this aids in standardizing paths when reading and concatenating.
#define FS_AUTO_NORMALIZE

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

namespace fs {

using namespace std::filesystem;

// The `+` operator is not available for fs::path:
// https://www.reddit.com/r/cpp/comments/9bwou7/why_doesnt_stdfilesystempath_have_an_operator/
// so I implemented it to redirect std::filesystem::path to use pathx, incorporating additional
// useful functionality and optimizations.
class pathx : public std::filesystem::path {
  private:
    string_type & _Text_get() { return const_cast<string_type &>(native()); }

  public:
    using std::filesystem::path::path;

    pathx() = default;
    pathx(const pathx &) = default;
    pathx(pathx &&) = default;
    ~pathx() = default;

    // Conversion constructors for implicit conversion from path to pathx.
    pathx(const path & p) noexcept
        : path(p) {}
    pathx(path && p) noexcept
        : path(std::move(p)) {}

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // BEGIN: Operators

    // Assignment operators
    pathx & operator=(const pathx &) = default;
    pathx & operator=(pathx &&) noexcept = default;
    pathx & operator=(string_type && _Source) noexcept {
        path::operator=(std::move(_Source));
        return *this;
    }
    template <n3std::PathConstructible _Src> pathx & operator=(const _Src & _Source) {
        path::operator=(_Source);
        return *this;
    }

    // Append operators
    pathx & operator/=(const pathx & _Other) {
#if defined(FS_AUTO_NORMALIZE)
        *this = path::operator/=(_Other).generic_string();
#else
        path::operator/=(_Other);
#endif
        return *this;
    }
    template <n3std::PathConstructible _Src> pathx & operator/=(const _Src & _Source) {
#if defined(FS_AUTO_NORMALIZE)
        *this = path::operator/=(path{_Source}).generic_string();
#else
        path::operator/=(path{_Source});
#endif
        return *this;
    }

    // Concat operators
    pathx & operator+=(const path & _Added) { return operator+=(_Added.native()); }
    pathx & operator+=(const string_type & _Added) {
        path::operator+=(_Added);
        return *this;
    }
    pathx & operator+=(const std::wstring_view _Added) {
        path::operator+=(_Added);
        return *this;
    }
    pathx & operator+=(const value_type * const _Added) {
        path::operator+=(_Added);
        return *this;
    }
    pathx & operator+=(const value_type _Added) {
        path::operator+=(_Added);
        return *this;
    }
    template <n3std::PathConstructible _Src> pathx & operator+=(const _Src & _Added) {
        path::operator+=(path{_Added}.native());
        return *this;
    }
    template <n3std::CharType CharT> pathx & operator+=(const CharT _Added) {
        path::operator+=(path{&_Added, &_Added + 1}.native());
        return *this;
    }

    // END: Operators
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <n3std::PathConstructible T> [[nodiscard]] friend pathx operator+(pathx lhs, T && rhs) noexcept {
        if constexpr (n3std::PathType<std::decay_t<T>>) {
            lhs._Text_get() += rhs.native();
        } else {
            lhs._Text_get() += path{std::forward<T>(rhs)}.native();
        }
        return lhs;
    }

    inline pathx & normalize(value_type from = '\\', value_type to = '/') & {
        std::ranges::replace(_Text_get(), from, to);
        return *this;
    }

    // Reference collapsing for temporaries
    inline pathx && normalize(value_type from = '\\', value_type to = '/') && {
        std::ranges::replace(_Text_get(), from, to);
        return std::move(*this);
    }

    // Const overload to return a modified copy
    inline pathx normalize(value_type from = '\\', value_type to = '/') const & {
        pathx copy = *this;
        copy.normalize(from, to);
        return copy;
    }

    inline pathx & make_lower() {
        n3std::to_lower(_Text_get());
        return *this;
    }

    [[nodiscard]] inline pathx lower() const {
        pathx copy(*this);
        copy.make_lower();
        return copy;
    }

    inline pathx & make_relative(const path & base, bool ignore_case = false) {
        if (base.empty()) {
            return *this;
        }

        std::string lhs = this->generic_string();
        std::string rhs = base.generic_string();
        if (ignore_case ? n3std::istarts_with(lhs, rhs) : lhs.starts_with(rhs)) {
            _Text_get().erase(0, rhs.size() + 1);
        }

        return *this;
    }

    [[nodiscard]] inline pathx relative(const path & base, bool ignore_case = false) const {
        fs::pathx copy(*this);
        copy.make_relative(base, ignore_case);
        return copy;
    }

    inline bool contains(const path & segment) const {
        for (const auto & seg : *this) {
            if (seg == segment) {
                return true;
            }
        }

        return false;
    }

    inline bool icontains(const path & segment) const {
        for (const auto & seg : *this) {
            if (n3std::iequals(seg, segment)) {
                return true;
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // BEGIN: Overloads - Declare some of the most common overloads to return pathx

    pathx & make_preferred() noexcept {
        path::make_preferred();
        return *this;
    }
    pathx & remove_filename() noexcept {
        path::remove_filename();
        return *this;
    }
    pathx & replace_filename(const path & _Replacement) { // remove any filename from *this and append _Replacement
        path::replace_filename(_Replacement);
        return *this;
    }
    pathx & replace_extension() noexcept {
        path::replace_extension();
        return *this;
    }
    pathx & replace_extension(const path & _Replacement) {
        path::replace_extension(_Replacement);
        return *this;
    }

    [[nodiscard]] pathx root_name() const { return path::root_name(); }
    [[nodiscard]] pathx root_directory() const { return path::root_directory(); }
    [[nodiscard]] pathx root_path() const { return path::root_path(); }
    [[nodiscard]] pathx relative_path() const { return path::relative_path(); }
    [[nodiscard]] pathx parent_path() const { return path::parent_path(); }
    [[nodiscard]] pathx filename() const { return path::filename(); }
    [[nodiscard]] pathx stem() const { return path::stem(); }
    [[nodiscard]] pathx extension() const { return path::extension(); }

    [[nodiscard]] pathx        lexically_normal() const { return path::lexically_normal(); }
    [[nodiscard]] inline pathx lexically_relative(const path & _Base) const { return path::lexically_relative(_Base); }
    [[nodiscard]] pathx lexically_proximate(const path & _Base) const { return path::lexically_proximate(_Base); }
};

[[nodiscard]] inline pathx absolute(const path & _Input, std::error_code & _Ec) {
    return std::filesystem::absolute(_Input, _Ec);
}

[[nodiscard]] inline pathx absolute(const path & _Input) {
    return std::filesystem::absolute(_Input);
}

[[nodiscard]] inline pathx canonical(const path & _Input) {
    return std::filesystem::canonical(_Input);
}

[[nodiscard]] inline pathx canonical(const path & _Input, std::error_code & _Ec) {
    return std::filesystem::canonical(_Input, _Ec);
}

[[nodiscard]] inline pathx read_symlink(const path & _Symlink_path, std::error_code & _Ec) {
    return std::filesystem::read_symlink(_Symlink_path, _Ec);
}

[[nodiscard]] inline pathx read_symlink(const path & _Symlink_path) {
    return std::filesystem::read_symlink(_Symlink_path);
}

[[nodiscard]] inline pathx temp_directory_path(std::error_code & _Ec) {
    return std::filesystem::temp_directory_path(_Ec);
}

[[nodiscard]] inline pathx temp_directory_path() {
    return std::filesystem::temp_directory_path();
}

[[nodiscard]] inline pathx current_path(std::error_code & _Ec) {
    return std::filesystem::current_path(_Ec);
}

[[nodiscard]] inline pathx current_path() {
    return std::filesystem::current_path();
}

inline void current_path(const path & _To, std::error_code & _Ec) noexcept {
    std::filesystem::current_path(_To, _Ec);
}

inline void current_path(const path & _To) {
    std::filesystem::current_path(_To);
}

[[nodiscard]] inline pathx weakly_canonical(const path & _Input, std::error_code & _Ec) {
    return std::filesystem::weakly_canonical(_Input, _Ec);
}

[[nodiscard]] inline pathx weakly_canonical(const path & _Input) {
    return std::filesystem::weakly_canonical(_Input);
}

[[nodiscard]] inline pathx proximate(const path & _Path, const path & _Base = std::filesystem::current_path()) {
    return std::filesystem::proximate(_Path, _Base);
}

[[nodiscard]] inline pathx proximate(const path & _Path, const path & _Base, std::error_code & _Ec) {
    return std::filesystem::proximate(_Path, _Base, _Ec);
}

[[nodiscard]] inline pathx proximate(const path & _Path, std::error_code & _Ec) {
    return std::filesystem::proximate(_Path, _Ec);
}

[[nodiscard]] inline pathx relative(const path & _Path, const path & _Base = std::filesystem::current_path()) {
    return std::filesystem::relative(_Path, _Base);
}

[[nodiscard]] inline pathx relative(const path & _Path, const path & _Base, std::error_code & _Ec) {
    return std::filesystem::relative(_Path, _Base, _Ec);
}

[[nodiscard]] inline pathx relative(const path & _Path, std::error_code & _Ec) {
    return std::filesystem::relative(_Path, _Ec);
}

// END: Overloads
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline pathx mktemp_file(std::string_view prefix = "temp", std::string_view suffix = ".tmp", size_t length = 4,
                         const pathx & dir = temp_directory_path()) {
    constexpr std::string_view chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    static thread_local std::mt19937                          generator{std::random_device{}()};
    static thread_local std::uniform_int_distribution<size_t> dist(0, chars.size() - 1);

    if (!dir.empty() && (!exists(dir) || !is_directory(dir))) {
        create_directories(dir);
    }

    pathx result;
    do {
        std::string filename;
        filename.reserve(prefix.size() + length + suffix.size());
        filename.append(prefix);
        for (size_t i = 0; i < length; ++i) {
            filename += chars[dist(generator)];
        }
        filename.append(suffix);

        result = dir / filename;
    } while (exists(result));

#if defined(FS_AUTO_NORMALIZE)
    return result.generic_string();
#else
    return result;
#endif
}

// Redirect path usages to pathx
using path = pathx;

} // namespace fs

// Extending standard functionality
namespace std {

// Allows std::format to support std::filesystem::path and derived types.
template <n3std::PathType T> struct formatter<T> : formatter<std::string> {
    auto format(const T & p, format_context & ctx) const { return formatter<std::string>::format(p.string(), ctx); }
};

namespace filesystem {

// Trick ADL to prioritize our custom overload so that we return pathx instead of path
[[nodiscard]] fs::pathx operator/(const n3std::PathConstructible auto & lhs,
                                  const n3std::PathConstructible auto & rhs) {
#if defined(FS_AUTO_NORMALIZE)
    return (static_cast<path>(lhs) / static_cast<path>(rhs)).generic_string();
#else
    return static_cast<path>(lhs) / static_cast<path>(rhs);
#endif
}

} // namespace filesystem

} // namespace std

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
