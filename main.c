#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list_sort.h"

struct debug_el {
    struct list_head list;
    int value;
    unsigned serial;
};

#define TEST_LIST_LEN (12) /* not including head */

static int cmp(void *priv, struct list_head *a, struct list_head *b)
{
    struct debug_el *ela, *elb;

    ela = container_of(a, struct debug_el, list);
    elb = list_entry(b, struct debug_el, list);

    return ela->value - elb->value;
}

int main(int argc, char const *argv[])
{
    int i, count = 1, err = -1;
    struct debug_el *el, *tmp;
    struct list_head *cur;

    srand(time(NULL));  // Initialization, should only be called once.
    LIST_HEAD(head);

    for (size_t i = 0; i < TEST_LIST_LEN; i++) {
        el = malloc(sizeof(*el));
        if (!el)
            goto exit;

        /* force some equivalencies */
        el->value = rand();
        el->serial = i;
        list_add_tail(&el->list, &head);
    }

    el = malloc(sizeof(*el));
    if (!el)
        goto exit;

    /* force some equivalencies */
    el->value = rand();
    el->serial = -1;

    list_add(&el->list, &head);

    list_for_each_entry(tmp, &head, list) {
        printf("serial %3d random value %d\n", tmp->serial, tmp->value);
    }

    printf("sort ...\n");
    list_sort(NULL, &head, cmp);

    printf("delete and free serial %3d random value %d\n", el->serial, el->value);
    list_del(&el->list);
    free(el);
    el = NULL;

    list_for_each_entry(el, &head, list) {
        printf("sorted value %10d serial %3d\n", el->value, el->serial);
    }

exit:
    list_for_each_entry_safe(el, tmp, &head, list) {
        printf("free serial %d\n", el->serial);
        free(el);
    }

    return 0;
}
