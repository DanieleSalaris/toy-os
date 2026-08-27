#include "formats.h"
#include "string.h"
#include <stdarg.h>
#define HEX_BUF_SIZE 8
#define INT_BUF_SIZE 10

char digit_mapper[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

typedef enum {
    F_INVALID,
    F_DEC,
    F_HEX
} format_e;

format_e  parse_format(char s[]) {
    if (s[0] != '%') return F_INVALID;
    switch (s[1]) {
        case 'd':
            return F_DEC;
        case 'x':
            return F_HEX;
    }
    return F_INVALID;
}

int count_formats(char s[]) {
    while(*s != '\0') {
        if (parse_format(s) != F_INVALID)
            s;
        s++;
    }
}

int sprintf(char s, ...) {
}

size_t fnumber(char buf[], int v, size_t size, int base) {
    size_t i;
    size_t digits = size;
    // populating buf in reversed order
    for (i=1; i<=size; i++) {
        buf[size-i] = digit_mapper[v % base];
        v /= base;
    }
    // skipping leading zeros (the last 0 is  ignored to handle 0 case)
    for (i=0; i<size-1 && buf[i]=='0'; i++) {
        digits--; 
    }
    return digits;
}
int fhex(char s[], int v, size_t len) {
    char hexbuf[HEX_BUF_SIZE];
    size_t digits = fnumber(hexbuf, v, HEX_BUF_SIZE, 16);
    size_t skipzeros = HEX_BUF_SIZE - digits;
    // excluding prefix
    len -= 2;
    // unable to fit the formatted hex
    if (digits > len) {
        return 0;
    }
    s[0] = '0';
    s[1] = 'x';
    memcpy(s+2, hexbuf+skipzeros, digits);
    return digits+2;
}

int fdec(char s[], int v, size_t len) {
    char intbuf[INT_BUF_SIZE];
    size_t digits = fnumber(intbuf, v, INT_BUF_SIZE, 10);
    size_t skipzeros = INT_BUF_SIZE - digits;
    if (digits > len) {
        return 0;
    }
    memcpy(s, intbuf+skipzeros, digits);
    return digits;
}
