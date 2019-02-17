#include <string.h>

int ifFlag(int argc, char* argv, char* flag) {
	// i = 0 is assumed to be ./binary
	// skipping it
	for(int i = 1; i < argc; i++) {
		if(!strcmp(flag, argv[i])) {
			return i;
		}
	}

	return 0;
}