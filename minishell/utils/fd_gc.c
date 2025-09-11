#include <unistd.h>
#include <stdlib.h>
#include "gc.h"

typedef struct s_fd_gc {
    int *fds;
    int count;
    int capacity;
} t_fd_gc;

static t_fd_gc *get_fd_gc_instance(void)
{
    static t_fd_gc instance = {NULL, 0, 0};
    return &instance;
}

void fd_gc_add(int fd)
{
    t_fd_gc *gc = get_fd_gc_instance();
    if (gc->count == gc->capacity) {
        int new_capacity = gc->capacity ? gc->capacity * 2 : 8;
        int *new_fds = gc_malloc(sizeof(int) * new_capacity);
        if (gc->fds) {
            for (int i = 0; i < gc->count; i++)
                new_fds[i] = gc->fds[i];
        }
        gc->fds = new_fds;
        gc->capacity = new_capacity;
    }
    gc->fds[gc->count++] = fd;
}

void fd_gc_cleanup(void)
{
    t_fd_gc *gc = get_fd_gc_instance();
    for (int i = 0; i < gc->count; i++)
        close(gc->fds[i]);
    gc->fds = NULL;
    gc->count = 0;
    gc->capacity = 0;
}
