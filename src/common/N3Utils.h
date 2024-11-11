
#pragma once

#include <string>
#include <ranges>
#include <algorithm>

namespace n3std {

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
