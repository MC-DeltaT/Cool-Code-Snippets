// From "Beyond struct: Meta-programming a struct Replacement in C++20" by John Bandela: https://youtu.be/FXfrojjIo80


#include <algorithm>
#include <cstddef>
#include <string_view>


// Fixed-length string for use as template arguments at compile time.
template<std::size_t N>
struct ConstantString {
    char data[N] = {};

    constexpr ConstantString(char const (&str)[N + 1]) {
        std::copy_n(str, N, data);
    }

    [[nodiscard]]
    constexpr std::string_view string_view() const {
        return {data, N};
    }
};

// Deduction guide to trim off null terminator from a string literal.
template<std::size_t N>
ConstantString(char const (&)[N]) -> ConstantString<N - 1>;


template<std::size_t N1, std::size_t N2>
[[nodiscard]]
constexpr bool operator==(ConstantString<N1> const& lhs, ConstantString<N2> const& rhs) {
    return lhs.string_view() == rhs.string_view();
}


template<ConstantString S1, ConstantString S2>
constexpr bool equal() {
    return S1 == S2;
}


int main() {
    static_assert(equal<"foo", "foo">());
    static_assert(!equal<"foo", "hello">());
}
