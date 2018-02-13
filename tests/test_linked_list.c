#include "../0_linked_list/linked_list.h"

/////////////////////////////////////////
//TODO: make tests multithreaded
int main()
{
        int ret;
        list_t *L = NULL;

        ret = list_destroy(L);
        assert(ret == EXIT_FAILURE);

        L = list_init();
        if (L == NULL) {
                fprintf(stderr, "Empty list.\n");
                return EXIT_FAILURE;
        }

	//check ret
        ret = list_destroy(L);

        return EXIT_SUCCESS;
}

