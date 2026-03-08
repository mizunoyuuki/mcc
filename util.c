#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<stdarg.h>

void error(char *fmt, ...){
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        fprintf(stderr, "\n");
        exit(1);
}
