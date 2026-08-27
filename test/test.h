#ifndef TEST_H
#define TEST_H
#include "string.h"
#include "stdio.h"

#define TEST(name) static void name(void)

#define RUN(name) do { \
    inc_test_run(); \
    name(); \
    if (!get_test_failed()) inc_test_passed(); \
    set_test_failed(0); \
} while(0)

#define ASSERT_EQ(a, b) do { \
    if((a) != (b)) { \
        printf("FAIL %s:%d: %s != %s\n", __FILE__, __LINE__, #a, #b); \
        set_test_failed(1); \
    } \
} while(0)

#define ASSERT_LT(a, b) do { \
    if((a) >= (b)) { \
        printf("FAIL %s:%d: %s not <  %s\n", __FILE__, __LINE__, #a, #b); \
        set_test_failed(1); \
    } \
} while(0)

#define ASSERT_GT(a, b) do { \
    if((a) <= (b)) { \
        printf("FAIL %s:%d: %s not >  %s\n", __FILE__, __LINE__, #a, #b); \
        set_test_failed(1); \
    } \
} while(0)

#define ASSERT_EQ_STRING(a, b, n) do { \
    if (strncmp(a, b, n) != 0) { \
        printf("FAIL %s:%d: %s != %s", __FILE__, __LINE__, #a, #b); \
        set_test_failed(1); \
    } \
} while(0)

void inc_test_passed();
void inc_test_run();
void set_test_failed(int value);
int get_test_failed();

void test_init();
void test_results();

#endif
