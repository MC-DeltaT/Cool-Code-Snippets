// By making an immediately invoked lambda consteval, I think it forces the compiler to execute it at compile time.


int main() {
    []() consteval {
        // Changing b to true will cause a compile error, proving the lambda is executed at compile time.
        auto const b = false;
        if (b) {
            throw 1;
        }
    }();
}
