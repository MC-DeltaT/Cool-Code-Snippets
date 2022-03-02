// Using a templated lambda to deduce an std::integer_sequence on the spot.


#include <array>
#include <cstddef>
#include <tuple>
#include <utility>


int main() {
    std::array<int, 5> const array{1, 2, 3, 4, 5};
    auto const t = [&array] <std::size_t... I> (std::index_sequence<I...>) {
        return std::make_tuple(std::get<I>(array)...);
    }(std::make_index_sequence<array.size()>{});
}
