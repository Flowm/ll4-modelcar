#include "utilities.h"
#include <base/env.h>
#include <base/printf.h>

//#include <iostream>

void printError(const char *msg) {
    PERR("ERROR %s", msg);
    //std::cerr << "ERROR >> " << msg << std::endl;
}
