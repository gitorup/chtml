#ifndef _CHTML_H_
#define _CHTML_H_

#ifdef __cplusplus
extern "C" {
#endif

/* chtml type */
#define chtml_object	1
#define chtml_head		2
#define chtml_body		3

typedef struct chtml {
	struct chtml *prev, *next;
	struct chtml *child;

	int type;
} chtml_t;

extern chtml_t *chtm_create(void);
extern void chtml_delete(chtml_t *chtml);

#ifdef __cplusplus
}
#endif

#endif // _CHTML_H_

