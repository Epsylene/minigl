#pragma once
#include "glspch.hpp"

#define GLS_ASSERT(x, msg) { if(!(x)) { printf(msg "\n"); assert(false); } }

namespace glose
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
}