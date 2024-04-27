
#pragma once

#include <string>
#include <ranges>
#include <algorithm>

namespace N3 {

static bool iequals(const std::string_view & lhs, const std::string_view & rhs) {
    auto to_lower{std::ranges::views::transform(::tolower)};
    return std::ranges::equal(lhs | to_lower, rhs | to_lower);
}

} // namespace N3
