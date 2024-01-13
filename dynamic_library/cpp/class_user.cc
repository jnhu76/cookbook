#include <dlfcn.h>
#include <random>
#include <iostream>
#include "myclass.h"

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,999); 

    void * handler = dlopen("./myclass.so", RTLD_LAZY);

    MyClass* (*create) (int);
    void (*destory) (MyClass*);

    create = (MyClass* (*)(int))dlsym(handler, "create_object");
    destory = (void (*)(MyClass*))dlsym(handler, "destroy_object");

    int tmp = dist6(rng);
    MyClass* myclass = (MyClass*)create(tmp);
    myclass->DoSomething();
    destory(myclass);
    return 0;
}