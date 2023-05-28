#ifndef __LIST_H__
#define __LIST_H__

#include <stdint.h>

/**
 * @brief Compare two elements in the list
 * 
 * @param a The first element b The second element
 * 
 * @return 0 if a == b, 1 if a > b, -1 if a < b
*/
typedef int (*list_compare_t)(void *a, void *b);

typedef struct list_node {
    void *data;
    struct list_node *next;
} list_node_t;

typedef struct list {
    uint32_t element_size;
    list_node_t *head;
    list_node_t *tail;
    uint32_t size;
} list_t;

typedef struct list_deep {
    uint32_t element_size;
    list_node_t *head;
    list_node_t *tail;
    uint32_t size;
    void (*copy)(void *dst, void *src);
    void (*destroy)(void *data);
} list_deep_t;

typedef list_t* list_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

list_handle_t z_list_create(uint32_t element_size);

list_handle_t z_list_create_deep(uint32_t element_size, void (*copy)(void *dst, void *src), void (*destroy)(void *data));

void z_list_destroy(list_handle_t list);

void z_list_push_back(list_handle_t list, void *data);

void z_list_erase(list_handle_t list, void* data, list_compare_t comp);

void z_list_iterate(list_handle_t list, void (*callback)(void *data, int* stop));

uint32_t z_list_size(list_handle_t list);

#ifdef __cplusplus
}
#endif

#endif // __LIST_H__