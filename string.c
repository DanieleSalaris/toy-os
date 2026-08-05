#include <stdint.h>
#include "string.h"

void *memcpy_asc(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    for (size_t i=0; i<n; i++) {
        *d = *s;
        d++;
        s++;
    }
    return dest;
}

void *memcpy_desc(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    for (size_t i=0; i<n; i++) {
        *d = *s;
        d--;
        s--;
    }
    return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
    if (dest < src) {
        return memcpy_asc(dest, src, n);
    }
    return memcpy_desc(dest, src, n);
}

