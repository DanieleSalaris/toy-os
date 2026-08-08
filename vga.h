#ifndef VGA_H
#define VGA_H

void terminal_putchar(char c);
void terminal_printstring(const char *s);
void terminal_puthex(int num);
void terminal_putint(int num);
void terminal_breakline();

// debugging functions 
void fill_screen();

#endif
