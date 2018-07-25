#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "malloc.h"
#include "libft.h"

static inline void *init_new_meta_block(struct zone_s *block_ptr, size_t size);
void *find_available_block(zone_ptr, size);
void *find_available_zone(size_t size_request, struct zone_s *zone_head, enum zone_type_e zone_type);
void *init_new_zone(enum zone_type_e zone_type, struct zone_s *prev_zone);
void *get_ptr_from_zone(size_t size, enum zone_type_e zone_type);
void *get_ptr(size_t size);
void *malloc(size_t size);




pthread_mutex_t mutex_malloc = PTHREAD_MUTEX_INITIALIZER;
struct malloc_meneger_s malloc_meneger_g =
{
    .zone_heads = {NULL, NULL, NULL}
};

//TODO: creat if for first initialization
static inline void *init_new_meta_block(struct zone_s *page_ptr, size_t size)
{
	struct block_s *block_ptr;

	block_ptr = page_ptr->md_block_head;


	ft_memset(block_ptr, 0x0, sizeof(struct block_s));

	block_ptr->size = size;
	block_ptr->next = NULL;
	block_ptr->prev = NULL;
	block_ptr->free = 0;

	return (void *)block_ptr;
}



void *find_available_block(struct zone_s *zone_ptr, size_t size)
{
	void *retval = NULL;






	return retval;
}




void *find_available_zone(size_t size_request, struct zone_s *zone_head, enum zone_type_e zone_type)
{
	struct zone_s *zone_ptr;
	void *retval = NULL;

	zone_ptr = zone_head;
	while (zone_ptr) {
		if (size_request < zone_ptr->space_left) {
//			retval = init_new_meta_block(zone_ptr, size_request);
			retval = find_available_block(zone_ptr, size_request);
			break ;
		}
		if (zone_ptr->next) {
			zone_ptr = zone_ptr->next;
		} else {
			zone_ptr->next = init_new_zone(zone_type, zone_ptr);
			if (!zone_ptr->next) {
				retval = NULL;
				break ;
			}
			zone_ptr = zone_ptr->next;
		}
	}
	return retval;
}



void *init_new_zone(enum zone_type_e zone_type, struct zone_s *prev_zone)
{
    void           *raw_ptr;
    struct zone_s  *zone;
    size_t          size;

    switch (zone_type) {
    case TINY:  size = TINY_ZONE  ;    break;
    case SMALL: size = SMALL_ZONE ;    break;
    case LARGE: size = LARGE_ZONE ;    break;

    default:    raw_ptr = NULL    ;    goto end;
    }
    ALIGN_PAGE_SIZE(size);
    raw_ptr = mmap(NULL, size, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (raw_ptr == MAP_FAILED) {
		raw_ptr = NULL;
		goto end;
	}
//	init_head_md();
	zone = (struct zone_s *)raw_ptr;
	zone->origin_size = size;
	zone->space_left = size - sizeof(struct zone_s);
	zone->next = NULL;
	zone->prev = prev_zone;
	if (prev_zone) {
		prev_zone->next = (struct zone_s *)raw_ptr;
	}
	printf("%s:%d:%p\n", __func__, __LINE__, raw_ptr); //debug
end:
    return raw_ptr ;
}

void *get_ptr_from_zone(size_t size, enum zone_type_e zone_type)
{
    void *retval;

    if (!malloc_meneger_g.zone_heads[zone_type]) {
        malloc_meneger_g.zone_heads[zone_type] = init_new_zone(zone_type, NULL);
        if (!malloc_meneger_g.zone_heads[zone_type]) {
            return NULL;
        }
    }
	printf("%s:%d\n", __func__, __LINE__); //debug

    retval = find_available_zone(size, malloc_meneger_g.zone_heads[zone_type], zone_type);


    return retval;
}




void *get_ptr(size_t size)
{
    void *retval;

    if (size < TINY_TRESHHOLD) {
        retval = get_ptr_from_zone(size, TINY);
    } else if (size < SMALL_TRESHHOLD) {
        retval = get_ptr_from_zone(size, SMALL);
//    } else if (size < LARGE_TRESHHOLD) {
//        retval = get_ptr_from_zone(size, LARGE);
    } else {
        retval = get_ptr_from_zone(size, LARGE);
;
    }


    return retval;
}



void *malloc(size_t size)
{
	void *retval;

	pthread_mutex_lock(&mutex_malloc);
	retval = NULL;
	if (size <= 0) {
	    goto end;
	}

	if (size < (size_t)-32) {
		ALIGN_META_INFO(size);
		printf("size == %zu\n", size); //debug
	}

    get_ptr(size);


	if (NULL == retval) {
//		errno = ENOMEM;

	}

end:
    pthread_mutex_unlock(&mutex_malloc);
    return retval;
}
