#pragma once
#include <type_traits>
#include <cstddef>
#include <cstring>
#include "../util/concepts.h"
#include "../util/platform.h"

namespace evo::bytes
{
    template<typename T, std::output_iterator<std::byte> I>
    requires std::is_trivially_copyable_v<T>
    I write(I&& dest, T value) {
        std::byte valueB[sizeof(T)];
        std::memcpy(valueB, &value, sizeof(T));

        if constexpr (global_byte_order == std::endian::native) {
            for(size_t i = 0; i < sizeof(T); ++i) 
                *dest++ = valueB[i];
        }
        else {
            for (size_t i = sizeof(T); i --> 0;)
                *dest++ = valueB[i];
        }

        return dest;
    }

    template<typename T, typename I>
    void write(I& dest, T value) {
        dest = write(std::move(dest), value);
    }

    template<typename T, std::input_iterator I>
    requires std::is_trivially_copyable_v<T> && std::same_as<typename std::iterator_traits<I>::value_type, std::byte>
    T read(I& src) {
        T valueT;
        std::byte valueB[sizeof(T)];

        if constexpr (global_byte_order == std::endian::native) {
            for (size_t i = 0; i < sizeof(T); ++i)
                valueB[i] = *src++;
        }
        else {
            for (size_t i = sizeof(T); i --> 0;)
                valueB[i] = *src++;
        }

        std::memcpy(&valueT, valueB, sizeof(T));
        return valueT;
    }

    template<typename T, std::input_iterator I>
    requires std::is_trivially_copyable_v<T> && std::same_as<typename std::iterator_traits<I>::value_type, std::byte>
    T read(I&& src) {
        return read<T, I>(src);
    }
}