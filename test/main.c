#include "stdio.h"
#include "stdlib.h"
#include "format_test.h"
#include "test.h"

int main (void) {
    test_init();
    format_test();
    test_results();
}
