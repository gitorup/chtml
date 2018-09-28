#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "chtml.h"

chtml_t *chtm_create(void)
{
	chtml_t *node = (chtml_t *)malloc(sizeof(chtml_t));
	if (node) {
		memset(node, 0, sizeof(chtml_t));
	}

	return node;
}

void chtml_delete(chtml_t *chtml)
{
    chtml_t *next;

    while (chtml) {
        next = chtml->next;
        free(chtml);
        chtml = next;
    }
}
