#include "optional.h"
#include <iostream>

inline int alive_count = 0;

struct Tracked {
    int value;

    Tracked() : value(0)            { ++alive_count; std::cout << "1"; }
    Tracked(int v) : value(v)       { ++alive_count; std::cout << "2"; }
    Tracked(const Tracked& o) : value(o.value) { ++alive_count; std::cout << "3"; }
    Tracked(Tracked&& o) : value(o.value) { o.value = -1; ++alive_count; std::cout << "4"; }

    Tracked& operator=(const Tracked& o) { value = o.value; return *this; }
    Tracked& operator=(Tracked&& o) { value = o.value; o.value = -1; return *this; }

    ~Tracked() { --alive_count; }

    bool operator==(const Tracked& o) const { return value == o.value; }
    bool operator!=(const Tracked& o) const { return value != o.value; }
};

int main() {
    Optional<Tracked> a(Tracked(1));
    // Optional<Tracked> b(Tracked(2));
    // b = a;
    a->~Tracked();
    std::cout << alive_count;
    return 0;
}