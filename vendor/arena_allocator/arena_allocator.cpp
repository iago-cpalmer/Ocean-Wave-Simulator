#include "arena_allocator.h"

#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cassert>

void* arena_alloc(Arena* arena, size_t size_bytes)
{
    size_t size = (size_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
    Region* current = arena->begin;

    // Find first free region with enough space to allocate the new size
    while(current != nullptr && current->capacity - current->occupied < size)
    {
        current = current->next;
    }

    if(current == nullptr)
    {
        // No region found, new must be created
        current = new_region(size_bytes);
        if(arena->end != nullptr)
        {
            arena->end->next = current;
        }
        arena->end = current;

        if(arena->begin == nullptr)
        {
            arena->begin = current;
        }
    }
    void* data = &current->data[current->occupied];
    current->occupied += size;

    return data;
}

Region* new_region(size_t size_bytes)
{
    size_t final_size_bytes = std::max(size_bytes, (size_t)REGION_DATA_SIZE);
    size_t qword_count = (final_size_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
    size_t size_bytes_total = sizeof(Region) + (qword_count * sizeof(uintptr_t));

    Region* r = (Region*)malloc(size_bytes_total);
    assert(r);
    r->capacity = qword_count;
    r->occupied = 0;
    r->next = nullptr;

    return r;
}

void region_reset(Region* r)
{
    r->occupied = 0;
}

void arena_reset(Arena* arena)
{
    Region* r = arena->begin;
    while(r)
    {
        region_reset(r);
        r = r->next;
    }
}

void free_arena(Arena* arena)
{
    Region* r = arena->begin;
    while(r)
    {
        Region* c = r;
        r = r->next;
        free(c);
    }

    arena->begin = nullptr;
    arena->end = nullptr;
}

void* arena_memdup(Arena* arena, const void* src, size_t size_bytes)
{
    void* dst = arena_alloc(arena, size_bytes);
    memcpy(dst, src, size_bytes);
    return dst;
}
