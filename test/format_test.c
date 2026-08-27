#include "format_test.h"
#include "test.h"
#include "../formats.h"
#include <string.h>

int fhex(char s[], int v, size_t len);
int fdec(char s[], int v, size_t len);

void test_hex(int v, char s[], int bufsize) {
    char buf[bufsize];
    int slen = strlen(s);
    int len = fhex(buf, v, bufsize-1);
    ASSERT_LT(len, bufsize);
    buf[len]='\0';
    ASSERT_EQ_STRING(s, buf, slen);
}

TEST(fhex_0xab) {
    test_hex(0xab, "0xab", 6);
}

TEST(fhex_0x12) {
    test_hex(0x12, "0x12", 6);
}

TEST(fhex_0x012bcf) {
    test_hex(0x012bcf, "0x12bcf", 6);
}

TEST(fhex_0x000) {
    test_hex(0x000, "0x0", 2);
}

void format_test() {
    RUN(fhex_0xab);
    RUN(fhex_0x12);
    RUN(fhex_0x012bcf);
    RUN(fhex_0x000); 
}
