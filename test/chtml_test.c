#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chtml.h"

void main_test(int argc, char *argv[])
{
    chtml_t *chtml = NULL;
    char *out;

    chtml = chtml_create();
    if (chtml == 0) {
        printf("chtml_create failed\n");
        return ;
    }

    out = chtml_print(chtml);
    if (out) {
        printf("out size: %ld, out: %s\n", strlen(out), out);
        free(out);
    }

    chtml_delete(chtml);
}

void main_test1(int argc, char *argv[])
{
    chtml_t *chtml = NULL;
    chtml_t *head, *body;
    char *out;

    chtml = chtml_create();
    if (chtml == 0) {
        printf("chtml_create failed\n");
        return ;
    }

    head = chtml_create_head();
    chtml_insert(head, chtml_create_meta("charset=\"utf-8\""));
    chtml_insert(head, chtml_create_tag("title", "title", "This is a test html!", NULL));
    chtml_insert(chtml, head);

    body = chtml_create_body();
    chtml_insert(body, chtml_create_tag("h1", "h1", "List of ./load", NULL));
    chtml_insert(chtml, body);

    out = chtml_print(chtml);
    if (out) {
        printf("out size: %ld, out: %s\n", strlen(out), out);
        free(out);
    }

    chtml_delete(chtml);
}

int main(int argc, char *argv[])
{
    main_test(argc, argv);
    main_test1(argc, argv);

    return 0;
}

