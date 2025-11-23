#ifndef _CLEANUP_H
#define _CLEANUP_H

typedef void (*cleanup_fun) (void *);

extern void do_cleanups (void);
extern int push_cleanup (cleanup_fun, void *);
extern void pop_cleanup (void);
extern void pop_all_cleanups (void);

#endif _CLEANUP_H
