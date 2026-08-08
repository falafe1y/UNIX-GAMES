#include "../head/input.h"

#include <unistd.h>

int readRawByte()
{
    unsigned char c;
    ssize_t n = read(STDIN_FILENO, &c, 1);

    if (n <= 0)
        return -1;

    return c;
}

