#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    int m, n;
    char* error;

    void* handler = dlopen("./libmymath.so", RTLD_LAZY);
    if (!handler) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror();   /* Clear any existing error */

    // add
    int (*add) (int, int) = dlsym(handler, "add");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    dlerror();

    // sub
    int (*sub) (int, int) = dlsym(handler, "sub");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    dlerror();

    // mul
    int (*mul) (int, int) = dlsym(handler, "mul");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    dlerror();

    // div
    double (*div) (int, int) = dlsym(handler, "div");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    dlerror();

    printf("input two numbers: ");
    scanf("%d %d", &m, &n);
    printf("%d + %d = %d\n", m, n, add(m, n));
    printf("%d - %d = %d\n", m, n, sub(m, n));
    printf("%d * %d = %d\n", m, n, mul(m, n));
    printf("%d / %d = %lf\n", m, n, div(m, n));

    // close dynamic library file.
    dlclose(handler);
    return 0;
}