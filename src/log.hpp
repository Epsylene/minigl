#pragma once

#include <fmt/core.h>
#include <fmt/os.h>
#include <fmt/color.h>

namespace minigl
{
    template<typename T>
    constexpr void print(T&& arg) {
        fmt::print("{}", std::forward<T>(arg));
    }

    template<typename... Ts>
    constexpr void print(fmt::format_string<Ts...> str, Ts&&... args) {
        fmt::print(str, std::forward<Ts>(args)...);
    }

    template<typename... Ts>
    constexpr void println(fmt::format_string<Ts...> str, Ts&&... args) {
        fmt::println(str, std::forward<Ts>(args)...);
    }

    template<typename... Ts>
    constexpr void trace(fmt::format_string<Ts...> str, Ts&&... args) {
        fmt::print(fmt::fg(fmt::color::gray), str, std::forward<Ts>(args)...);
        fmt::println("");
    }

    template<typename... Ts>
    constexpr void info(fmt::format_string<Ts...> str, Ts&&... args) {
        fmt::print(fmt::fg(fmt::color::green), str, std::forward<Ts>(args)...);
        fmt::println("");
    }

    template<typename... Ts>
    constexpr void error(fmt::format_string<Ts...> str, Ts&&... args) {
        fmt::print(fmt::fg(fmt::color::red), str, std::forward<Ts>(args)...);
        fmt::println("");
    }
}