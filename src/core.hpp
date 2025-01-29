#pragma once
#include "mglpch.hpp"

#include "log.hpp"

/// Assert macro that logs an error message and aborts the
/// program
#define MGL_ASSERT(x, msg, ...) { if(!(x)) { error("ERROR: " msg, ##__VA_ARGS__); assert(false); } }

// https://stackoverflow.com/a/8814003/7483388
#define dbg_1(x) { print(#x " = {}\n", x); }
#define dbg_2(x, y) { print(#x " = {}, " #y " = {}\n", x, y); }
#define dbg_3(x, y, z) { print(#x " = {}, " #y " = {}, " #z " = {}\n", x, y, z); }
#define dbg_4(x, y, z, w) { print(#x " = {}, " #y " = {}, " #z " = {}, " #w " = {}\n", x, y, z, w); }
#define dbg_n(n, x, y, z, w, func, ...) func

/// Debug macro to print the value of a variable
#define dbg(...) dbg_n(, ##__VA_ARGS__, dbg_4(__VA_ARGS__), dbg_3(__VA_ARGS__), dbg_2(__VA_ARGS__), dbg_1(__VA_ARGS__))

/// Debug print and abort
#define dbg_abort(...) { dbg(__VA_ARGS__); assert(false); }

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