#pragma once

#include <bali_utilities.h>

typedef struct bali_bump_arena_t {
  bsize_t used;
  bsize_t capacity;
  char *region;
} bali_bump_arena_t;

BALI_API void bali_bump_arena_init(bali_bump_arena_t *arena, bsize_t capacity);
BALI_API void bali_bump_arena_free(bali_bump_arena_t *arena);
BALI_API void bali_bump_arena_recycle(bali_bump_arena_t *arena);
BALI_API void *bali_bump_arena_malloc(bali_bump_arena_t *arena, bsize_t bytes);
