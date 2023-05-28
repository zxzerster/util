#include "set.h"
#include "list.h"
#include <time.h>
#include <assert.h>
#include <string.h>

#define THRESHOLD 0.85

#define PRIME 100663319

#define _H(x, a, b) (((a) * (x) + (b)) % PRIME)  

typedef struct hash_conatants {
    uint32_t a, b, p;
    uint32_t cadinality;
} hash_conatants_t;

typedef struct set {
    list_t **array;
    uint32_t capacity;
    uint32_t size;
    hash_conatants_t h;
} set_t;

#define SET_SIZE 16

static uint32_t _hash(set_t* set, uint64_t key) {
    return _H(key, set->h.a, set->h.b) % set->capacity;
}  

static int _rehash(set_t* set) {
    uint32_t new_capacity = set->capacity * 2;
    list_t** new_array = (list_t**)malloc(sizeof(list_t*) * new_capacity);
    if (!new_array) {
        return -1;
    }

    set->h.a = rand() % (PRIME - 1) + 1;
    set->h.b = rand() % PRIME;

    for (int i = 0; i < set->capacity; ++i) {
        list_t* chain = set->array[i];
        if (!chain) {
            continue;
        }

        list_node_t* node = chain->head;
        while (node) {
            uint32_t h = _hash(set, *(uint64_t*)node->data);
            assert(h < new_capacity);

            list_t* new_chain = new_array[h];
            if (!new_chain) {
                new_chain = z_list_create(sizeof(uint64_t));
                if (!new_chain) {
                    free(new_array);
                    return -1;
                }
                new_array[h] = new_chain;
            }

            z_list_push_back(new_chain, node->data);
            node = node->next;
        }

        z_list_destroy(chain);
    }

    free(set->array);
    set->array = new_array;
    set->capacity = new_capacity;

    return 0;
}

static int _need_rehash(set_t* set) {
    float load_factor = (float)set->size / (float)set->capacity;
    return load_factor > THRESHOLD ? 1 : 0;
}

static int _list_node_comp(void* a, void* b) {
    return *(uint64_t*)a - *(uint64_t*)b;
}

static void _list_traversal(void* data, int* stop) {

}

set_handle_t z_set_create() {
    set_t *set = (set_t*)malloc(sizeof(set_t));
    if (!set) {
        return NULL;
    }

    srand((unsigned int)time(NULL));

    set->capacity = SET_SIZE;
    set->array = (list_t**)malloc(sizeof(list_t *) * SET_SIZE);
    set->h.a = rand() % (PRIME - 1) + 1;
    set->h.b = rand() % PRIME;
    if (!set->array) {
        free(set);
        return NULL;
    }
    memset(set->array, 0, sizeof(list_handle_t *) * SET_SIZE);

    return set;
}

void z_set_insert(set_handle_t set, uint64_t key) {
    if (!set) {
        return;
    }

    set_t* s = (set_t*)set;

    uint32_t h = _hash(s, key);
    assert(h < s->capacity);

    list_t* chain = s->array[h];
    if (!chain) {
        chain = z_list_create(sizeof(uint64_t));
        if (!chain) {
            return;
        }
        s->array[h] = chain;
    }
    
    if (!z_set_contains(set, key)) {
        z_list_push_back(chain, &key);
        s->size++;
    }
}

int z_set_contains(set_handle_t set, uint64_t key) {
    if (!set) {
        return 0;
    }

    set_t* s = (set_t*)set;

    if (_need_rehash(s)) {
        _rehash(s);
    }

    uint32_t h = _hash(s, key);
    assert(h < s->capacity);

    list_t* chain = s->array[h];
    if (!chain) {
        return 0;
    }

    list_node_t* node = chain->head;
    while (node) {
        if (*(uint64_t*)node->data == key) {
            return 1;
        }
        node = node->next;
    }

    return 0;
}

void z_set_erase(set_handle_t set, uint64_t key) {
    if (!set) {
        return;
    }

    set_t* s = (set_t*)set;

    uint32_t h = _hash(s, key);
    assert(h < s->capacity);

    list_t* chain = s->array[h];
    if (!chain) {
        return;
    }

    list_node_t* node = chain->head;
    while (node) {
        if (*(uint64_t*)node->data == key) {
            z_list_erase(chain, &key, _list_node_comp);
            s->size--;
            return;
        }
        node = node->next;
    }

    return;
}