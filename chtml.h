#ifndef _CHTML_H_
#define _CHTML_H_

#ifdef __cplusplus
extern "C" {
#endif

/* print log */
#define chtml_log(...)     printf(__VA_ARGS__)

/* chtml type */
#define chtml_object	1
#define chtml_head		2
#define chtml_body		3

typedef struct chtml {
	struct chtml *prev, *next;
	struct chtml *child;

	char *label;
	int type;
} chtml_t;

extern chtml_t *chtml_create(void);
extern char *chtml_print(chtml_t *chtml);
extern void chtml_delete(chtml_t *chtml);

#ifdef __cplusplus
}
#endif

#endif // _CHTML_H_

