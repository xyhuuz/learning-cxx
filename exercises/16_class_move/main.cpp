#include "../exercise.h"

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>
#include <cassert>

class DynFibonacci {
    size_t *cache;
    int cached;

public:
    // 实现动态设置容量的构造器
    DynFibonacci(int capacity): cache(new size_t[capacity]), cached(2) {
        cache[0] = 0;
        cache[1] = 1;
    }

    // 实现移动构造器
    DynFibonacci(DynFibonacci &&other) noexcept: cache(other.cache), cached(other.cached) {
        other.cache = nullptr;
        other.cached = 0;
    }

    // 实现移动赋值
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        if (this != &other) {
            delete[] cache;  // 释放原来的内存
            cache = other.cache;
            cached = other.cached;
            other.cache = nullptr;  // 避免析构时释放内存
            other.cached = 0;
        }
        return *this;
    }

    // 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;
    }

    // 实现正确的缓存优化斐波那契计算
    size_t operator[](int i) {
        if (i < cached) {
            return cache[i];  // 如果已经计算过了，直接返回
        }

        // 计算并缓存新的斐波那契数
        for (int j = cached; j <= i; ++j) {
            cache[j] = cache[j - 1] + cache[j - 2];
        }
        cached = i + 1;  // 更新已缓存的数量
        return cache[i];
    }

    // 不要修改这个方法
    size_t operator[](int i) const {
        assert(i < cached && "i out of range");
        return cache[i];
    }

    // 不要修改这个方法
    bool is_alive() const {
        return cache != nullptr;
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    assert(fib[10] == 55 && "fibonacci(10) should be 55");

    DynFibonacci const fib_ = std::move(fib);
    assert(!fib.is_alive() && "Object moved");
    assert(fib_[10] == 55 && "fibonacci(10) should be 55");

    DynFibonacci fib0(6);
    DynFibonacci fib1(12);

    fib0 = std::move(fib1);
    fib0 = std::move(fib0);
    assert(fib0[10] == 55 && "fibonacci(10) should be 55");

    return 0;
}//12
