#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "chtml.h"

chtml_t *chtml_create_object(int type, char *slabel, char *elabel, char *text, char *attr)
{
    chtml_t *node = (chtml_t *)malloc(sizeof(chtml_t));

    if (node) {
        memset(node, 0, sizeof(chtml_t));
        node->type = type;
        if (slabel) node->slabel = strdup(slabel);
        if (elabel) node->elabel = strdup(elabel);
        if (text) node->text = strdup(text);
        if (attr) node->attr = strdup(attr);
    }

    return node;
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

void chtml_delete(chtml_t *chtml)
{
    chtml_t *next;

    while (chtml) {
        next = chtml->next;
        if (chtml->child) chtml_delete(chtml->child);
        if (chtml->slabel) { free(chtml->slabel); chtml->slabel = NULL; }
        if (chtml->elabel) { free(chtml->elabel); chtml->elabel = NULL; }
        if (chtml->text) { free(chtml->text); chtml->text = NULL; }
        if (chtml->attr) { free(chtml->attr); chtml->attr = NULL; }
        free(chtml);
        chtml = next;
    }
}

static int chtml_print_object_size(chtml_t *chtml)
{
    int chtml_size = 0;

    if (chtml && chtml->slabel) {
        chtml_size += 2 + strlen(chtml->slabel); /* <> */
    }

    if (chtml && chtml->elabel) {   
        chtml_size += 3 + strlen(chtml->elabel); /* </> */
    }

    return chtml_size;
}

static int chtml_print_meta_size(chtml_t *chtml)
{
    int chtml_size = 0;

    if (chtml && chtml->slabel) {
        chtml_size += 2 + strlen(chtml->slabel); /* <> */
    }
    if (chtml && chtml->elabel) {
        chtml_size += 3 + strlen(chtml->elabel); /* </> */
    }
    if (chtml && chtml->attr) chtml_size += (strlen(chtml->attr) + 1); /* one space */

    return chtml_size;
}

static int chtml_print_tag_size(chtml_t *chtml)
{
    int chtml_size = 0;

    if (chtml && chtml->slabel) {
        chtml_size += 2 + strlen(chtml->slabel); /* <> */
    }
    if (chtml && chtml->elabel) {
        chtml_size += 3 + strlen(chtml->elabel); /* </> */
    }
    if (chtml && chtml->text) chtml_size += strlen(chtml->text);
    if (chtml && chtml->attr) chtml_size += (strlen(chtml->attr) + 1); /* one space */

    return chtml_size;
}

static int chmtl_print_size(chtml_t *chtml)
{
    int chtml_size = 0;

    while (chtml) {
        if (chtml->child) chtml_size += chmtl_print_size(chtml->child);
        switch (chtml->type) {
            case chtml_object:  chtml_size += chtml_print_object_size(chtml);   break;
            case chtml_tag:     chtml_size += chtml_print_tag_size(chtml);      break;
            case chtml_meta:    chtml_size += chtml_print_meta_size(chtml);     break;
        }

        chtml = chtml->next;
    }

    chtml_size += 1; /* '\0' */
    return chtml_size;
}

static int chtml_print_all(chtml_t *chtml, char *ptr, int size)
{
    int chtml_size = 0;

    if (ptr == NULL || size <= 0) {
        chtml_log("input error, size: %d\n", size);
        return 0;
    }

    while (chtml) {
        if (chtml->attr) {
            if (chtml->slabel) chtml_size += snprintf(ptr + chtml_size, size - chtml_size, "<%s %s>", chtml->slabel, chtml->attr);
        } else {
            if (chtml->slabel) chtml_size += snprintf(ptr + chtml_size, size - chtml_size, "<%s>", chtml->slabel);
        }
        if (chtml->text) chtml_size += snprintf(ptr + chtml_size, size - chtml_size, "%s", chtml->text);
        if (chtml->child) chtml_size += chtml_print_all(chtml->child, ptr + chtml_size, size - chtml_size);
        if (chtml->elabel) chtml_size += snprintf(ptr + chtml_size, size - chtml_size, "</%s>", chtml->elabel);
        chtml = chtml->next;
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

/* chtml object */
chtml_t *chtml_create(void)         {return chtml_create_object(chtml_object, "html", "html", NULL, NULL);}
chtml_t *chtml_create_head(void)    {return chtml_create_object(chtml_object, "head", "head", NULL, NULL);}
chtml_t *chtml_create_body(void)    {return chtml_create_object(chtml_object, "body", "body", NULL, NULL);}

/* chtml meta */
chtml_t *chtml_create_meta(char *attr)  {return chtml_create_object(chtml_meta, "meta", NULL, NULL, attr);}
/* chtml tag */
chtml_t *chtml_create_tag(char *slabel, char *elabel, char *text, char *attr)  {return chtml_create_object(chtml_tag, slabel, elabel, text, attr);}

/* utility to jump whitespace and cr/lf */
static const char *chtml_skip(const char *in)
{
    while (in && *in && (unsigned char)*in <= 32) in++;
    return in;
}

static chtml_t *chtml_parse_all(chtml_t *chtml, const char *data)
{
    return chtml;
}

chtml_t *chtml_parse(const char *data)
{
    chtml_t *chtml = NULL;

    if (data) {
        chtml = (chtml_t *)malloc(sizeof(chtml_t));
        if (chtml == NULL)
            return chtml;

        return chtml_parse_all(chtml, chtml_skip(data));
    }

    return chtml;
}

chtml_t *chtml_get_tag(chtml_t *chtml, char *label)
{
    chtml_t *node = NULL;

    while (chtml) {
        if (strcasecmp(chtml->slabel, label) == 0)
            return node;
        if (chtml->child) {
            node = chtml_get_tag(chtml->child, label);
            if (node)
                return node;
        }
        chtml = chtml->next;
    }

    return node;
}

