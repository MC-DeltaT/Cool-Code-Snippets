// Buffer for storing type-erased values without dynamic allocation.


#include <cstddef>
#include <utility>
#include <iostream>
#include <concepts>


struct Visitor {
    template<typename T>
    void operator()(T&& value) {
        std::cout << value << std::endl;
    }
};


template<std::size_t Size, std::size_t Align>
class SmallObjectBuffer {
public:
    ~SmallObjectBuffer() {
        if (_visitor_func) {
            _visitor_func(_data, -1, nullptr);
        }
    }
    
    SmallObjectBuffer() noexcept :
        _visitor_func{nullptr}
    {}
    
    template<typename T, typename... Args> requires (sizeof(T) <= Size) && (alignof(T) <= Align) && std::constructible_from<T, Args...>
    SmallObjectBuffer(std::in_place_type_t<T>, Args&&... args) :
        _visitor_func{&_generic_visitor<T>}
    {
        new(_data) T{std::forward<Args>(args)...};
    }
    
    SmallObjectBuffer(SmallObjectBuffer const&) = delete;
    SmallObjectBuffer(SmallObjectBuffer&&) = delete;
    
    SmallObjectBuffer& operator=(SmallObjectBuffer const&) = delete;
    SmallObjectBuffer& operator=(SmallObjectBuffer&&) = delete;
    
    void visit(Visitor& visitor) {
        _visitor_func(_data, 1, &visitor);
    }
    
private:
    alignas(Align) char _data[Size];
    void (*_visitor_func)(void*, int, Visitor*);
    
    template<typename T>
    static void _generic_visitor(void* data, int operation, Visitor* visitor) {
        auto const p = static_cast<T*>(data);
        if (operation == -1) {
            p->~T();
        }
        else if (operation == 1) {
            (*visitor)(*p);
        }
    }
};


struct Foo {
    ~Foo() {
        std::cout << "~Foo()" << std::endl;
    }
    
    Foo(int v) :
        v{v}
    {
        std::cout << "Foo(" << v << ")" << std::endl;
    }
    
    int v;
};

template<class Stream>
Stream& operator<<(Stream& stream, Foo const& foo) {
    std::cout << foo.v;
    return stream;
}


int main() {
    SmallObjectBuffer<24, 8> o{std::in_place_type<Foo>, 42};
    Visitor v;
    o.visit(v);
}
