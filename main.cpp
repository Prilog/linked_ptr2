#include <iostream>
#include <cstdio>

#include "linked_ptr.h"

int main() {
    int* a = new int(1111);
    linked_ptr<int> x = make_linked<int>(a);
    std::cout << *x.get();
    return 0;
}