// Usage of "stateless metaprogramming" via friend function instantiation to create a constexpr function that changes
// return values between invocations without changing its arguments.
// Unlike stateless metaprogramming from previous C++ versions, I think this C++20 code is completely well-formed,
// safe, and works on all major compilers.


#include <concepts>
#include <type_traits>


auto func(int);


template<std::same_as<void> U>
struct S {
    friend auto func(int) {}
};


template<auto X = 0, bool B = requires { func(X); }>
constexpr bool uh_oh() {
    if constexpr (B) {
        return false;
    }
    else {
        S<std::enable_if_t<!B>> s;
        (void)s;
        return true;
    }
}


int main() {
    static_assert(uh_oh());
    static_assert(!uh_oh());
    static_assert(!uh_oh());
    static_assert(!uh_oh());
}
