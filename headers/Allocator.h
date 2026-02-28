#ifndef FIRSTLAB_ALLOCATOR_H
#define FIRSTLAB_ALLOCATOR_H

typedef void *(*Allocator)();
typedef void (*Deallocator)(void *);

#endif //FIRSTLAB_ALLOCATOR_H