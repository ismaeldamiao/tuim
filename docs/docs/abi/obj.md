# Object model for Tuim

[I. F. F. dos Santos](mailto:ismaellxd@gmail.com)

Last modified: September 15, 2025.

![Crates.io License](https://img.shields.io/crates/l/MIT)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ismaeldamiao/tuim)
![GitHub Release](https://img.shields.io/github/v/release/ismaeldamiao/tuim)

!> Tuim is a project in active development,
for now it is designed only for test purposes.

## Abstract

This article exposes the convenctions followed
in the development of The Tuim Project
and how developers may follow that conventions to target environments
based on Tuim.

## Introduction

## Initialization in C

If the C compiler ABI targets reference[??] then
a object descriptor may be represented as...
a `object.h` file shall look as:

```c
#ifndef OBJECT_H
#define OBJECT_H

#include <stddef.h>

void *new(const void *class, ...);
void delete(void *instance);

size_t AlignOf(const void *instance);
size_t SizeOf (const void *instance);

void *ctor(void *instance, ...);
void *dtor(void *instance);

#endif // OBJECT_H
```

The `new` function may be implemented as follows...
and a `class.c` 

```c
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct Class {
   size_t alignment, size;
   void *(*ctor)(void *, va_list *);
   void *(*dtor)(void *);
   uint32_t id;
} Class;

size_t AlignOf(const void *instance){
   const Class *class = *(const void **)instance;
   return class->alignment;
}
size_t SizeOf (const void *instance){
   const Class *class = *(const void **)instance;
   return class->size;
}

void *ctor(void *instance, ...){
   const Class *class = *(const void **)instance;
   va_list args;
   va_start(args, instance);
   instance = class->ctor(instance, &args);
   va_end(args);
   return instance;
}
void *dtor(void *instance){
   const Class *class = *(const void **)instance;
   return class->dtor(instance);
}

void *new(const void *_class, ...){
   const Class *class = _class;
   void *instance;
   va_list args;

   instance = aligned_alloc(class->alignment, class->size);
   if(instance == NULL){
      // a particular language may instead "thow exception", "cath error", etc...
      return NULL;
   }
   *(const void **)instance = _class;

   va_start(args, _class);
   instance = class->ctor(instance, &args);
   va_end(args);

   return instance;
}

void delete(void *instance){
   if(instance != NULL){
      const Class *class = *(const void **)instance;
      free(class->dtor(instance));
   }
}
```

Method 1 (heap initialization)

```c
void *btn = new(Button);
/* ... */
delete(btn);
```

Mathod 2 (stack initialization)

```c
char buffer[SizeOf(Button)];
void *btn = buffer;
ctor(btn);
/* ... */
dtor(btn);
```


## References

## Appendex

### Object-Oriented Programming in C

#### Basic example

A object with only public data:

```c
typedef struct {
   char *name;
   double mass;
} star_t;
star_t *star_ctor(star_t *instance, const char *name);
star_t *star_dtor(star_t *instance);
```

... may be used like:

```c
star_t ACruxC;
star_ctor(&ACruxC, "HR 4729");
ACruxC.mass = 9.65;
/* ... */
star_dtor(&sACruxC);
```

Object with only private data:

```c
typedef struct star_s star_t;
star_t *star_new(const char *name);
void star_delete(start_t *instance);
double star_mass(star_t *instance, double mass);
```

... may be used like:

```c
star_t *ACruxC = star_new("HR 4729");
star_mass(ACruxC, 9.65);
/* ... */
star_delete(ACruxC);
```

Object with both, public and private data:

```c
typedef struct {
   // public attributes
   double mass;
   // private data
   alignas(max_align_t) char private[];/* flexible array member:
                        The standard grant that a array of characters
                        may be casted to a pointer to any object */
} star_t;
star_t *star_new(const char *name);
void star_delete(start_t *instance);
```

Note that if there is some private data then the `new` allocator is needed.
... may be used like:

```c
star_t *sn = star_new("HR 4729");
sn->mass = 9.65;
/* ... */
star_delete(sn);
```

Encapsulamento, Abstração, Herança, Polimorfismo

#### Class Descriptor Table

The implementation of the Class Descriptor Table in C looks like:

```c
struct Class {
   size_t alignment, size;
   void *(*ctor)(void *, va_list *);
   void *(*dtor)(void *);
   uint32_t id;
};
```

The implementation of the `star` object shall be

```c
extern const void *star_class;
typedef struct {
   struct Class *class; // points to the same place as star_class
   alignas(max_align_t) char private[];
} star_t;
```

#### Erase-me

```c
typedef struct {
   // public interface
   struct {
      void *(*ctor)(void *instance, va_list args);
      void *(*dtor)(void *instance);
   } vtable;
   size_t size; // sizeof(start_t)
   // private data
   alignas(max_align_t) char private[];/* flexible array member:
                        The standard grant that a array of characters
                        may be casted to a pointer to any object */
} obj_t;
```

... may be used like:

```c
star_t *sn = star_new("SN 1987A");
/* ... */
star_delete(sn);
```

