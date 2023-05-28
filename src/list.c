#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"

static void _list_push_back(list_handle_t list, list_node_t* node) {
    assert(list != NULL && node != NULL);

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

static void _list_clear(list_handle_t list) {
    assert(list != NULL);

    list_node_t* node = list->head;
    while (node != NULL) {
        if (node->data) free(node->data);
        list_node_t* next = node->next;
        free(node);
        node = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

// Create a new list
list_handle_t z_list_create(uint32_t element_size) {
    assert(element_size > 0);
    list_t* list = (list_t*)malloc(sizeof(list_t));
    if (list == NULL) {
        return NULL;
    }

    list->element_size = element_size;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

// Destroy a list
void z_list_destroy(list_handle_t list) {
    if (list == NULL) {
        return;
    }

    _list_clear(list);
    free(list);
}

void z_list_push_back(list_handle_t list, void* data) {
    if (list == NULL || data == NULL) {
        return;
    }

    list_node_t* node = (list_node_t*)malloc(sizeof(list_node_t));
    if (node == NULL) return;

    node->data = malloc(list->element_size);
    if (node->data == NULL) {
        free(node);
        return;
    }
    memcpy(node->data, data, list->element_size);
    node->next = NULL;
    list->size += 1;

    _list_push_back(list, node);
}

void z_list_erase(list_handle_t list, void* data, list_compare_t comp) {
    if (list == NULL || data == NULL || comp == NULL) {
        return;
    }
    
    list_node_t* n = list->head;
    list_node_t* prev = NULL;

    // empty list
    if (n == NULL) return;
    if (comp(n->data, data) == 0) {
        list->head = n->next;
        free(n->data);
        free(n);
        list->size -= 1;
        return;
    }

    prev = n;
    n = n->next;
    while (n != NULL) {
        if (comp(n->data, data) == 0) {
            prev->next = n->next;
            free(n->data);
            free(n);

            list->size -= 1;
            return;
        }
        prev = n;
        n = n->next;
    }

    return;
}

void z_list_iterate(list_handle_t list, void (*callback)(void* data, int* stop)) {
    if (list == NULL || callback == NULL) {
        return;
    }

    int stop = 0;
    list_node_t* node = list->head;
    while (node != NULL) {
        callback(node->data, &stop);
        if (stop) break;
        node = node->next;
    }
}

uint32_t z_list_size(list_handle_t list) {
    if (list == NULL) {
        return 0;
    }

    return list->size;
}