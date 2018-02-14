#include <stdio.h>
#include "errors_sp.h"

void sp_print_error(int error_code)
{
	switch(error_code) {
		case ERROR_NULL:
			fprintf(stderr, "NULL pointer to data structure provided.\n");
			break;
		case ERROR_ALLOC:
			fprintf(stderr, "Failed to allocate memory.\n");
			break;
		default:
			fprintf(stderr, "Unknown error code\n");
			break;
	}
	return;
}
