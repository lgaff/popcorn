#include <stdio.h>
#include <stdint.h>

int main() {
    unsigned int test_val = 0xffff0000;
    char buf[32];
    
    sprintf(buf, "test %08x\n", test_val);
    return 0;
}

