#include "ctype.h"
#define UL_OFFSET 0X20

int tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + UL_OFFSET;
    }
    return c;
}

int toupper(int c) {
    if (c >= 'a' && c <= 'z') {
        return c - UL_OFFSET;
    }
    return c;
}
