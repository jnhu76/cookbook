#include "myclass.h"
#include <iostream>

extern "C" MyClass* create_object(int x) {
    return new MyClass(x);
}

extern "C" void destroy_object(MyClass *obj) {
    delete obj;
}

void MyClass::DoSomething() {
    std::cout << x << "\n";
}
