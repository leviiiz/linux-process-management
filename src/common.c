#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void print_banner(const char *module_name) {
    printf("========================================\n");
    printf("  %s\n", module_name);
    printf("  Linux Process Management Project\n");
    printf("========================================\n\n");
}
void init_random() { srand(time(NULL)); }
int random_range(int min, int max) { return rand() % (max - min + 1) + min; }
