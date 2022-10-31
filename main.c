#include <stdio.h>
#include <stdlib.h>
#include "opencl.h"
#pragma warning(disable : 4996)
int main()
{
    cpu_run();
    opencl_run();
    return 0;
}