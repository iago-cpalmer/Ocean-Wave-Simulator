#ifndef ARENA_H
#define ARENA_H

#include <cstddef>
#include <cstdint>

/*
 * Example usage:
 *
 * Arena arena = {0};   // Create arena
 *
 * V2i* v1 = (V2i*)arena_alloc(&arena, sizeof(V2i));        // Store a V2i
 * v1->x = 1;                                               // Set value to its X component
 * or
 * V2i* v1 = new(arena_alloc(&arena, sizeof(V2i)))V2i(1,0); // Store and initialize with constructor a V2i
 *
 *  // Now v1 can be used as any other V2i
 *  // You can duplicate it into another V2i
 *  V2i* v2 = arena_memdup(&arena, v1, sizeof(V2i));
 *
 *  // Once the contents of the arena are no longer needed:
 *  arena_reset(&arena);
 *
 *  // Once the arena itself is no longer needed, destroy all its regions.
 *  // Note that it not frees the arena itself, only its regions.
 *  free_arena(&arena);
 *
 */

// By default a region's data block occupies 8KB. Define if different size is needed
// If an allocation of a bigger object is done, a region with that size will be created instead
#ifndef REGION_DATA_SIZE
  #define REGION_DATA_SIZE 8*1024
#endif


typedef struct Region Region;

struct Region{
    Region* next;
    size_t capacity;
    size_t occupied;
    uintptr_t data[];
};

typedef struct {
    Region* begin;
    Region* end;
} Arena;

// ------------------------------------
// Region code
//  ------------------------------------

Region* new_region(size_t size);

void region_reset(Region* r);

// ------------------------------------
// Arena code
//  ------------------------------------

void* arena_alloc(Arena* arena, size_t size_bytes);

// Resets the regions, it does not free them.
void arena_reset(Arena* arena);

// Frees all the regions of the arena. NOT THE ARENA ITSELF!!
void free_arena(Arena* arena);

void* arena_memdup(Arena* arena, const void* src, size_t size_bytes);

#ifdef ARENA_IMPLEMENTATION


#endif

#endif // ARENA_H
