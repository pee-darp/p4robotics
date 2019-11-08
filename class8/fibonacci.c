#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

int64_t memo_fib(int target, int64_t *term);
int64_t faster_fib(int target);
int64_t fib(int target);
void test(int case_num);

int main(int argc, char *argv[]) {
    test(1);
    test(2);
    test(3);
    return 0;
}

int64_t memo_fib(int target, int64_t *term) {
    //base case
    if (target <= 1) {
        return target;
    }
    //if fib(target) already computed
    //return fib(target)
    if (term[target] != 0) {
        return term[target];
    }
    //compute fib(target) and store it
    term[target] = memo_fib(target - 1, term) + memo_fib(target - 2, term);
    return term[target];
}

int64_t faster_fib(int target) {
    int64_t fib_2, fib_1, fib;
    fib = 0;
    fib_2 = 1;
    fib_1 = 1;
    for (int i = 2; i < target; i++) {
        fib = fib_1 + fib_2;
        fib_2 = fib_1;
        fib_1 = fib;
    }
    return fib;
}

int64_t fib(int target) {
    if (target <= 2) {
        return 1;
    }
    return fib(target - 2) + fib(target - 1);
}

void test(int case_num) {
    int64_t term[100] = {0};
    clock_t start = clock();
    int iterations = 0;
    int64_t result = 0;
    char fun_name[11];
    //printf("%ld\n", clock() - start);
    while ((clock() - start) / (double)CLOCKS_PER_SEC < 0.5) {
        for (int i = 0; i < 100; i++) {
            switch (case_num) {
            case 1:
                result = fib(38);
                break;
            case 2:
                result = faster_fib(90);
                break;
            case 3:
                result = memo_fib(90, term);
                break;
            default:
                printf("Invalid test case!\n");
            }
        }
        iterations += 100;
    }
    double elapsed = (clock() - start) / (double)iterations / (double)CLOCKS_PER_SEC;
    switch (case_num) {
    case 1:
        strncpy(fun_name, "fib1(38)", 11);
        break;
    case 2:
        strncpy(fun_name, "fib2(90)", 11);
        break;
    case 3:
        strncpy(fun_name, "fib3(90)", 11);
        break;
    default:
        printf("Invalid test case!\n");
    }
    printf("%s got %ld and took %.6f ms per iteration\n", fun_name, result, elapsed * 1000);
}
