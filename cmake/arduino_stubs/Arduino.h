#pragma once

#include <algorithm>
#include <type_traits>
#include <cstdint>
#include <iostream>
#include <string>

#define F(value) value

template <typename T, typename U>
constexpr auto min(T lhs, U rhs) {
    using Common = std::common_type_t<T, U>;
    return std::min(static_cast<Common>(lhs), static_cast<Common>(rhs));
}

class TwoWire {};

extern TwoWire Wire;

class SerialStub {
public:
    void begin(uint32_t) {}

    template <typename T>
    void print(const T& value) {
        std::cout << value;
    }

    template <typename T>
    void println(const T& value) {
        std::cout << value << '\n';
    }

    void println() {
        std::cout << '\n';
    }
};

extern SerialStub Serial;

void delay(uint32_t milliseconds);
