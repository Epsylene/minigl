#pragma once
#include "mglpch.hpp"

#include "log.hpp"

/// Assert macro that logs an error message and aborts the
/// program
#define MGL_ASSERT(x, msg, ...) { if(!(x)) { error("ERROR: " msg, ##__VA_ARGS__); assert(false); } }

namespace minigl
{
    template<typename T> using Box = std::unique_ptr<T>;
    template<typename T> using Ref = std::shared_ptr<T>;

    /// Create a std::shared_ptr from the given set of
    /// arguments
    template<typename T, typename... Args>
    constexpr Ref<T> ref(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    /// Create a std::unique_ptr from the given set of
    /// arguments
    template<typename T, typename... Args>
    constexpr Box<T> box(Args&&... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}