#include <stdio.h>
#include <string.h>

#include "chtml.h"

int main(int argc, char *argv[])
{
    chtml_t *chtml = NULL;

    chtml = chtm_create();
    if (chtml == 0) {
        printf("chtml_create failed\n");
        return -1;
    }

    chtml_delete(chtml);
	return 0;
}

