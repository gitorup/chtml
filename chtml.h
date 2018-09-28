#ifndef _CHTML_H_
#define _CHTML_H_

#ifdef __cplusplus
extern "C" {
#endif

/* print log */
#define chtml_log(...)     printf(__VA_ARGS__)

/* chtml type */
#define chtml_object	1
#define chtml_label		2
#define chtml_body		3

typedef struct chtml {
	struct chtml *prev, *next;
	struct chtml *child;

	char *label;
	char *text;
	int type;
} chtml_t;

extern chtml_t *chtml_create(void);
extern chtml_t *chtml_create_head(void);
extern chtml_t *chtml_create_body(void);
extern char *chtml_print(chtml_t *chtml);
extern void chtml_delete(chtml_t *chtml);

extern void chtml_append(chtml_t *chtml, chtml_t *node);
extern void chtml_insert(chtml_t *chtml, chtml_t *node);
extern chtml_t *chtml_insert_label(chtml_t *chtml, char *label, char *text);

#ifdef __cplusplus
}
#endif

#endif // _CHTML_H_

