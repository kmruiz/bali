#include <stdlib.h>
#include <string.h>

#include <bali_utilities.h>
#include <bali_utilities/bump_arena.h>

void bali_bump_arena_init(bali_bump_arena_t *arena, bsize_t capacity)
{
  BALI_DCHECK(arena != nullptr);
  BALI_DCHECK_BSIZE_BOUNDS(capacity);

  arena->capacity = capacity;
  arena->used = 0;
  arena->region = calloc(1, capacity);

  BALI_RCHECK_NOT_NULL(arena->region);
}

void bali_bump_arena_free(bali_bump_arena_t *arena)
{
  BALI_DCHECK(arena != nullptr);
  BALI_DCHECK(arena->region != nullptr);

  free(arena->region);
  memset(arena, 0, sizeof(*arena));
}

void bali_bump_arena_recycle(bali_bump_arena_t *arena)
{
  BALI_DCHECK(arena != nullptr);
  BALI_DCHECK(arena->region != nullptr);

  memset(arena->region, 0, arena->capacity);
  arena->used = 0;
}

void *bali_bump_arena_malloc(bali_bump_arena_t *arena, bsize_t bytes)
{
  BALI_DCHECK(arena != nullptr);
  BALI_DCHECK(arena->region != nullptr);

  bsize_t new_used = arena->used + bytes;
  if (new_used > arena->capacity) {
    return nullptr;
  }

  void *result = &arena->region[arena->used];
  arena->used = new_used;
  return result;
}

