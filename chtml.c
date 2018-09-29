#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "chtml.h"

static chtml_t *chtml_create_object(char *label)
{
    chtml_t *node = (chtml_t *)malloc(sizeof(chtml_t));

    if (node) {
        memset(node, 0, sizeof(chtml_t));
        node->type = chtml_object;
        if (label) node->label = strdup(label);
    }

    return node;
}

chtml_t *chtml_create(void)
{
    return chtml_create_object("html");
}

chtml_t *chtml_create_head(void)
{
    return chtml_create_object("head");
}

chtml_t *chtml_create_body(void)
{
    return chtml_create_object("body");
}

void chtml_append(chtml_t *chtml, chtml_t *node)
{
    while (chtml) {
        if (chtml->next == NULL) {
            chtml->next = node;
            break;
        }
        chtml = chtml->next;
    }
}

void chtml_insert(chtml_t *chtml, chtml_t *node)
{
    if (chtml) {
        if (chtml->child == NULL) {
            chtml->child = node;
        } else {
            chtml_append(chtml->child, node);
        }
    }
}

chtml_t *chtml_insert_tag(chtml_t *chtml, char *label, char *text)
{
	chtml_t *node = NULL;

	node = (chtml_t *)malloc(sizeof(chtml_t));
	if (node) {
	    memset(node, 0, sizeof(chtml_t));
        node->type = chtml_tag;
        if (label) node->label = strdup(label);
        if (text) node->text = strdup(text);
    }

    if (chtml) chtml->child = node;
	return node;
}

void chtml_delete(chtml_t *chtml)
{
    chtml_t *next;

    while (chtml) {
        next = chtml->next;
        if (chtml->child) chtml_delete(chtml->child);
        if (chtml->label) { free(chtml->label); chtml->label = NULL; }
        if (chtml->text) { free(chtml->text); chtml->text = NULL; }
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
        if (chtml->child) chtml_size += chmtl_print_size(chtml->child);
        switch (chtml->type) {
            case chtml_object: {
                if (chtml->label) {
                    chtml_size += 2 * strlen(chtml->label);
                    chtml_size += 5; /* <></> */
                }
                break;
            }

            case chtml_tag:
                if (chtml->label) {
                    chtml_size += 2 * strlen(chtml->label);
                    chtml_size += 5; /* <></> */
                }
                if (chtml->text) chtml_size += strlen(chtml->text);
                break;
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
        if (chtml->label) chtml_size += snprintf(ptr + chtml_size, size - chtml_size, "<%s>", chtml->label);
        if (chtml->text) chtml_size += snprintf(ptr + chtml_size, size - chtml_size, "%s", chtml->text);
        if (chtml->child) chtml_size += chtml_print_all(chtml->child, ptr + chtml_size, size - chtml_size);
        if (chtml->label) chtml_size += snprintf(ptr + chtml_size, size - chtml_size, "</%s>", chtml->label);
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

