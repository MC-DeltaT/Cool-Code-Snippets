// This is some really dumb code to visit a runtime integer value as a compile time constant.
// Obviously doesn't scale to large integer types. But maybe you can use a binary search to optimise it a bit more.

#include <array>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>
#include <type_traits>


template<std::integral T, T V, class Func>
decltype(auto) constexprise_value_impl(T v, Func f) {
    if (v == V) {
        return f(std::integral_constant<T, V>{});
    }
    else {
        return constexprise_value_impl<T, static_cast<T>(V + 1)>(v, f);
    }
}


template<std::integral T, class Func>
decltype(auto) constexprise_value(T v, Func f) {
    return constexprise_value_impl<T, std::numeric_limits<T>::min()>(v, f);
}


int main() {
    std::uint8_t v = 65;
    constexprise_value(v, [](auto v) {
        std::array<char, v> a{};
        std::cout << v << std::endl;
    });

}
