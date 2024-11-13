
#pragma once

#include <string>
#include <ranges>
#include <algorithm>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#if defined(_WIN32) || defined(_WIN64)
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#endif // #if defined(_WIN32) || defined(_WIN64)

namespace n3std {

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

static bool iequals(const std::string_view & lhs, const std::string_view & rhs) {
    auto to_lower{std::ranges::views::transform(::tolower)};
    return std::ranges::equal(lhs | to_lower, rhs | to_lower);
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
