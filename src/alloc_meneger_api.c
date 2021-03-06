#include "malloc_internal_api.h"


pthread_mutex_t mutex_malloc = PTHREAD_MUTEX_INITIALIZER;

struct malloc_meneger_s malloc_meneger_g =
{
    .zone_heads = {NULL, NULL, NULL},
	.print_total_alloc = 0,
};

void *get_zone_head(size_t size)
{
    void *retval;

    if (size <= TINY_TRESHHOLD) {
        retval = malloc_meneger_g.zone_heads[TINY];
    } else if (size <= SMALL_TRESHHOLD) {
        retval = malloc_meneger_g.zone_heads[SMALL];
    } else {
        retval = malloc_meneger_g.zone_heads[LARGE];
    }
    return retval;
}


void *get_ptr_to_md(void *ptr)
{
	void *ret_val;
//	ret_val = ptr - offsetof(struct block_s, data);
	ret_val = ptr - ((size_t)&((struct block_s *)0)->data);
	return ret_val;
}


int check_block_ptr(struct zone_s *zone_ptr, struct block_s *block_ptr)
{
    struct block_s *tmp_block_ptr = &zone_ptr->md_block_head;
    int ret_val = 0;

    while (tmp_block_ptr) {
        if (block_ptr == tmp_block_ptr) {
            if (!block_ptr->free) {
            	ret_val = 1;
            }
            	goto end;
        }
        tmp_block_ptr = tmp_block_ptr->next;
    }

end:
    return ret_val;
}

int validate_md(struct zone_s **zone_ptr, struct block_s **block_ptr)
{
	int ret_val = 0;
    enum zone_type_e zone_type;
//    struct zone_s *zone_ptr;

    zone_type = MIN_ZONE_TYPE;
    while (zone_type < MAX_ZONE_TYPE) {
        *zone_ptr = malloc_meneger_g.zone_heads[zone_type];
        while (*zone_ptr) {
            if (check_block_ptr(*zone_ptr, *block_ptr))
                goto end;
            *zone_ptr = (*zone_ptr)->next;
        }
        ++zone_type;
    }
    ret_val = -1;
end:
    return (ret_val);
}
