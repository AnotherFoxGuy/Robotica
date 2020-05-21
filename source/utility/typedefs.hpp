#pragma once

#include <cstdint>
#include <cassert>
#include <memory>
#include <filesystem>


namespace robotica {
    namespace fs = std::filesystem;


     using u8 = uint8_t;
     using i8 = int8_t;
     using u16 = uint16_t;
     using i16 = int16_t;
     using u32 = uint32_t;
     using i32 = int32_t;
     using u64 = uint64_t;
     using i64 = int64_t;


     template <typename Returns, typename... Args> 
     using Fn = Returns(*)(Args...);

     template <typename Class, typename Returns, typename... Args>
     using MemFn = Returns(Class::*)(Args...);

     template <typename Class, typename T>
     using MemVar = T(Class::*);


     template <typename T, typename Deleter = std::default_delete<T>>
     using unique = std::unique_ptr<T, Deleter>;

     template <typename T> using shared = std::shared_ptr<T>;
     template <typename T> using weak = std::weak_ptr<T>;
}