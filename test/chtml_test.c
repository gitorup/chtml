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
    chtml_insert(head, chtml_create_tag("title", "title", "Welcome to OpenResty!", NULL));
    chtml_insert(head, chtml_create_tag("style", "style", "\n\tbody {\n\t\t"
        "width: 35em;\n\t\tmargin: 0 auto;\n\t\tfont-family: Tahoma, Verdana, Arial, sans-serif;\n\t}\n", NULL));
    chtml_insert(chtml, head);

    body = chtml_create_body();
    chtml_insert(body, chtml_create_tag("h1", "h1", "Welcome to OpenResty!", NULL));
    chtml_insert(body, chtml_create_tag("p", "p", "If you see this page, "
            "the OpenResty web platform is successfully installed and working. "
            "Further configuration is required.", NULL));
    chtml_insert(body, chtml_create_tag("p", "p", "For online documentation and support please refer to openresty.org", NULL));
    chtml_insert(body, chtml_create_tag("p", "p", "Thank you for flying OpenResty.", NULL));
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

