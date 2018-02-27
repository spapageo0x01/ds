#include <stdlib.h>
#include <pthread.h>
#include "../error/errors_sp.h"

//https://en.wikipedia.org/wiki/Heap_(data_structure)#Operations

void heap_init(void);
void heap_destroy(void);

void heap_insert(void);
void heap_delete_max(void);
void heap_delete_min(void);

void heap_extract_max(void);
void heap_extract_min(void);

void heap_find_max(void);
void heap_find_min(void);

void heap_heapify(void);


void heap_isempty(void);
void heap_size(void);