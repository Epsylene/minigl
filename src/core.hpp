#pragma once
#include "mglpch.hpp"

#include <fmt/core.h>

#define MGL_ASSERT(x, msg, ...) { if(!(x)) { error(msg, ##__VA_ARGS__); assert(false); } }

namespace minigl
{
    template<typename T> using Box = std::unique_ptr<T>;
    template<typename T> using Ref = std::shared_ptr<T>;

    template<typename T, typename... Args>
    constexpr Ref<T> ref(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    constexpr Box<T> box(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename... Ts>
    constexpr auto print(const std::string& str, Ts&&... args) {
        return fmt::print(fmt::runtime(str), std::forward<Ts>(args)...);
    }
    
    template<typename... Ts>
    constexpr auto println(const std::string& str, Ts&&... args) {
        return fmt::print(fmt::runtime(str + "\n"), std::forward<Ts>(args)...);
    }

    template<typename... Ts>
    constexpr auto error(const std::string& str, Ts&&... args) {
        return fmt::print(stderr, fmt::runtime(str + "\n"), std::forward<Ts>(args)...);
    }
}