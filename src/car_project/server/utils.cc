#include "utils.h"

#ifdef GENODE
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
	PERR("ERROR >> %s", msg);
#else
	std::cout << "ERROR >> " << msg << std::endl;
#endif	
}
