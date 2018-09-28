#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "chtml.h"

chtml_t *chtml_create(void)
{
	chtml_t *node = (chtml_t *)malloc(sizeof(chtml_t));
	if (node) {
		memset(node, 0, sizeof(chtml_t));
	}

    node->label = "html";
	return node;
}

void chtml_delete(chtml_t *chtml)
{
    chtml_t *next;

    while (chtml) {
        next = chtml->next;
        if (chtml->child) {
            chtml_delete(chtml->child);
        }
        free(chtml);
        chtml = next;
    }
}

int chmtl_print_size(chtml_t *chtml)
{
    chtml_t *next;
    int chtml_size = 0;

    while (chtml) {
        next = chtml->next;
        if (chtml->child) {
            chtml_size += chmtl_print_size(chtml->child);
        }
        if (chtml->label) {
            chtml_size += 2 * strlen(chtml->label);
            chtml_size += 5; /* <></> */
        }
        chtml = next;
    }

    chtml_size += 1; /* '\0' */
    return chtml_size;
}

int chtml_print_all(chtml_t *chtml, char *ptr, int size)
{
    chtml_t *next;
    int chtml_size = 0;

    if (ptr == NULL || size <= 0) {
        chtml_log("input error, size: %d\n", size);
        return 0;
    }

    while (chtml) {
        next = chtml->next;
        if (chtml->label) {
            chtml_size += snprintf(ptr + chtml_size, size - chtml_size, "<%s>", chtml->label);
        }
        if (chtml->child) {
            chtml_size += chtml_print_all(chtml->child, ptr + chtml_size, size - chtml_size);
        }
        if (chtml->label) {
            chtml_size += snprintf(ptr + chtml_size, size - chtml_size, "</%s>", chtml->label);
        }
        chtml = next;
    }

    return chtml_size;
}

char *chtml_print(chtml_t *chtml)
{
    char *out = NULL;
    int chtml_size = 0;
    int out_size = 0;

    chtml_size = chmtl_print_size(chtml);
    if (chtml_size <= 0) {
        chtml_log("chtml_print data is NULL\n");
        return NULL;
    }

    out = (char *)malloc(chtml_size);
    memset(out, 0, chtml_size);
    chtml_log("chtml print size: %d\n", chtml_size);

    out_size = chtml_print_all(chtml, out, chtml_size);
    out[out_size + 1] = '\0';
    return out;
}

