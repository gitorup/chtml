#ifndef _CHTML_H_
#define _CHTML_H_

#ifdef __cplusplus
extern "C" {
#endif

/* print log */
#define chtml_log(...)     printf(__VA_ARGS__)

/* chtml type */
#define chtml_object	1
#define chtml_tag		2
#define chtml_meta		3

typedef struct chtml {
    struct chtml *prev, *next;
    struct chtml *child;

    /* chtml type */
    int type;

    char *label;
    char *text;
    char *attr;
} chtml_t;

extern chtml_t *chtml_create(void);
extern chtml_t *chtml_create_head(void);
extern chtml_t *chtml_create_body(void);
extern chtml_t *chtml_create_tag(char *label, char *text, char *attr);

extern void chtml_delete(chtml_t *chtml);
extern void chtml_append(chtml_t *chtml, chtml_t *node);
extern void chtml_insert(chtml_t *chtml, chtml_t *node);
extern char *chtml_print(chtml_t *chtml);

#ifdef __cplusplus
}
#endif

#endif // _CHTML_H_

