# Arena Allocator

A simple and fast arena allocator written in C++. 

## Example Usage

```c++
#include "arena_allocator.h"

int main() {
    Arena arena = {0};   // Create arena

    // Allocate space for a V2i
    V2i* v1 = (V2i*)arena_alloc(&arena, sizeof(V2i));
    v1->x = 1;

    // Or in C++, allocate and construct with placement new
    V2i* v2 = new(arena_alloc(&arena, sizeof(V2i))) V2i(1, 0);

    // Duplicate v2 into another arena-allocated V2i
    V2i* v3 = (V2i*)arena_memdup(&arena, v2, sizeof(V2i));

    // Reset all arena allocations (does not free memory, just marks for reuse)
    arena_reset(&arena);

    // Fully free all allocated memory regions (but not the Arena struct itself)
    free_arena(&arena);

    return 0;
}
