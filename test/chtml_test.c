#include <stdio.h>
#include <string.h>

#include "chtml.h"

int main(int argc, char *argv[])
{
    chtml_t *chtml = NULL;
    char *out;

    chtml = chtml_create();
    if (chtml == 0) {
        printf("chtml_create failed\n");
        return -1;
    }

    out = chtml_print(chtml);
    printf("out size: %ld, out: %s\n", strlen(out), out);

    chtml_delete(chtml);
	return 0;
}

