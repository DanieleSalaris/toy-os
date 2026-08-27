#include "test.h"
int test_passed = 0;
int test_run = 0;
int test_failed = 0;

void test_init() {
    test_passed = 0;
    test_run = 0;
    test_failed = 0;    
}

void test_results() {
    printf("TEST: %d / %d test passed\n", test_passed, test_run);
}

void inc_test_passed() {
    test_passed++;
}

void inc_test_run() {
    test_run++;
}

void set_test_failed(int value) {
    test_failed = value;
}

int get_test_failed() {
    return test_failed;
}
