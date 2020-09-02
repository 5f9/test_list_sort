#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list_sort.h"

struct debug_el {
    unsigned int poison1;
    struct list_head list;
    unsigned int poison2;
    int value;
    unsigned serial;
};

#define TEST_POISON1 0xDEADBEEF
#define TEST_POISON2 0xA324354C
#define TEST_LIST_LEN (512 + 128 + 2) /* not including head */

static int cmp(void *priv, struct list_head *a, struct list_head *b)
{
    struct debug_el *ela, *elb;

    ela = container_of(a, struct debug_el, list);
    elb = container_of(b, struct debug_el, list);

    return ela->value - elb->value;
}

int main(int argc, char const *argv[])
{
    int i, count = 1, err = -1;
    struct debug_el *el;
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
        el->poison1 = TEST_POISON1;
        el->poison2 = TEST_POISON2;
        list_add_tail(&el->list, &head);
    }
    list_sort(NULL, &head, cmp);

    for (cur = head.next; cur->next != &head; cur = cur->next) {
        struct debug_el *ela = container_of(cur, struct debug_el, list);
        printf("%d\n", ela->value);
    }
exit:

    for (cur = head.next->next; cur != &head; cur = cur->next) {
        struct debug_el *ela = container_of(cur->prev, struct debug_el, list);
        free(ela);
    }
    struct debug_el *ela = container_of(cur->prev, struct debug_el, list);
    free(ela);
    return 0;
}
