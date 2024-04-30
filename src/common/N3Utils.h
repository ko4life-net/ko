
#pragma once

#include <string>
#include <ranges>
#include <algorithm>

namespace N3 {

static bool iequals(const std::string_view & lhs, const std::string_view & rhs) {
    auto to_lower{std::ranges::views::transform(::tolower)};
    return std::ranges::equal(lhs | to_lower, rhs | to_lower);
}

static std::string BytesToHex(const uint8_t * const byData, const size_t iSize) {
    if (!byData || iSize == 0) {
        return "<null>";
    }

    static const char * hex_digits = "0123456789ABCDEF";

    std::string result;
    result.reserve(iSize * 2);
    for (size_t i = 0; i < iSize; ++i) {
        result += hex_digits[(byData[i] & 0xF0) >> 4];
        result += hex_digits[byData[i] & 0x0F];
    }

    return result;
}

} // namespace N3
