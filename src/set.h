#ifndef __SET_H__
#define __SET_H__

#include <stdlib.h>

typedef void* set_handle_t;

#ifdef __cplusplus
extern "C" {
#endif

set_handle_t z_set_create();

void z_set_insert(set_handle_t set, uint64_t key);

int z_set_contains(set_handle_t set, uint64_t key);

void z_set_erase(set_handle_t set, uint64_t key);

#ifdef __cplusplus
}
#endif

#endif // __SET_H__