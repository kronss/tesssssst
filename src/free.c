#include "malloc_internal_api.h"
#include <stddef.h> /*offsetof*/

int check_block_ptr(struct zone_s *zone_ptr, struct block_s *block_ptr)
{
    struct block_s *tmp_block_ptr = zone_ptr->md_block_head;
    int ret_val = 1;

    while (tmp_block_ptr) {
        if (block_ptr == tmp_block_ptr)
            goto end;
        tmp_block_ptr = tmp_block_ptr->next;
    }
    ret_val = 0;
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


static int is_all_blocks_free(struct zone_s *zone_ptr)
{
	int ret_val = 1;
	struct block_s *block_ptr = zone_ptr->md_block_head;

	while (block_ptr) {
		if (block_ptr->free == 0) {
			ret_val = 0;
			goto end;
		}
		block_ptr = block_ptr->next;
	}
end:
	return ret_val;
}

static void remove_cur_zone(struct zone_s *zone_ptr)
{
//	struct zone_s *zone_next = zone_ptr->next;
//	struct zone_s *zone_prev = zone_ptr->prev;
//
//	if (zone_ptr->next)
//

}

//TODO: move to internal API
void *get_ptr_to_md(void *ptr)
{
	void *ret_val;

	ret_val = ptr - offsetof(struct block_s, data);
	return ret_val;
}

void free(void *ptr)
{
    struct zone_s *zone_ptr = NULL;
    struct block_s *block_ptr = NULL;

	pthread_mutex_lock(&mutex_malloc);
	if (ptr == NULL) {
		goto end;
	}
	printf("%s:%d: ptr %p\n", __func__, __LINE__, ptr); //debug
	block_ptr = get_ptr_to_md(ptr);
	printf("%s:%d: ptr %p\n", __func__, __LINE__, ptr); //debug

	if (validate_md(&zone_ptr, &block_ptr)) {
		goto end;
	}
    return_free_block_to_pull(zone_ptr, block_ptr);
	if (is_all_blocks_free(zone_ptr))
		remove_cur_zone(zone_ptr);


	printf("%s:%d: bingo! %p\n", __func__, __LINE__, ptr); //debug
end:
	pthread_mutex_unlock(&mutex_malloc);
}
