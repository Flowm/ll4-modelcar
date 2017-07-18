#include "utilities.h"

#ifdef GENODE
	#include <base/env.h>
	#include <base/printf.h>
#else
	#include <iostream>
#endif

void print_message(const char *msg) {
#ifdef GENODE
	PDBG("%s", msg);
#else
	std::cout << msg << std::endl;
#endif	
}

void print_error(const char *msg) {
#ifdef GENODE
	PDBG("ERROR >> %s", msg);
#else
	std::cout << "ERROR >> " << msg << std::endl;
#endif	
}
