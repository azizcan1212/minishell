#ifndef FD_GC_H
#define FD_GC_H

int fd_gc_add(int fd);
void fd_gc_cleanup(void);
void fd_gc_remove(int fd);

#endif
