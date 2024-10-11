#ifndef CRT_H
#define CRT_H

#ifdef __cplusplus
extern "C" {
#endif

extern void* crt(void *main, int argc, char **argv, int *ret);

#ifdef __cplusplus
}
#endif

#endif // CRT_H
