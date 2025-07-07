#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>

#if __STDC_VERSION__ < 202311L
#define true 1
#define false 0
typedef int bool;
#endif /* __STDC_VERSION__ < 202311L */

#if __STDC_VERSION__ >= 202311L
   #define maybe_unused [[maybe_unused]]
#elif defined(__GNUC__)
   #define maybe_unused __attribute__((unused))
#endif

#ifndef LIST_TYPE
   #define LIST_TYPE void *
#endif

#define addressof(ptr) ((uintptr_t)(void*)(ptr))

/* Node for a XOR linked list */
struct list_node {
   LIST_TYPE data;
   uintptr_t link;
};

/* The list */
struct list {
   struct list_node *first, *last;
};

/* Function to add data to the end of the list, create a new list if NULL
   at input */
maybe_unused
static struct list *list_add(struct list *list, LIST_TYPE data){
   struct list_node *new;
   void *malloc(size_t), free(void*);

   /* Allocate memory to the new node */
   new = malloc(sizeof(struct list_node));
   if(new == NULL){
      return NULL;
   }
   new->data = data;

   /* If creating a new list */
   if(list == NULL){
      new->link = (uintptr_t)0U; /* addressof(NULL) ^ addressof(NULL) */

      list =  malloc(sizeof(struct list));
      if(list == NULL){
         free(new);
         return NULL;
      }
      list->first = list->last = new;
      return list;
   }
   new->link = addressof(list->last) ^ addressof(NULL);

   /* Add the node at the end of the list:
   current = list->last;
   previous = (void*)(current->link ^ addressof(NULL));
   current->link = addressof(previous) ^ addressof(new); */

   list->last->link ^= addressof(NULL) ^ addressof(new);
   list->last = new;
   return list;
}

/* Function to find an element of the list satisfing a property */
maybe_unused
static LIST_TYPE list_find(const struct list *list, bool(*property)(LIST_TYPE)){
   struct list_node *previous, *current, *next;
   LIST_TYPE null = { };

   if(list == NULL) return null;

   previous = NULL;
   current = list->first;
   do {
      if(property(current->data)) return current->data;
      next = (void*)(addressof(previous) ^ current->link);
      previous = current;
      current = next;
   } while(current != NULL);

   return null;
}

#endif /* LIST_H */
