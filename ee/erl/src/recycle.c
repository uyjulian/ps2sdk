/*
--------------------------------------------------------------------
By Bob Jenkins, September 1996.  recycle.c
You may use this code in any way you wish, and it is free.  No warranty.

This manages memory for commonly-allocated structures.
It allocates RESTART to REMAX items at a time.
Timings have shown that, if malloc is used for every new structure,
  malloc will consume about 90% of the time in a program.  This
  module cuts down the number of mallocs by an order of magnitude.
This also decreases memory fragmentation, and freeing structures
  only requires freeing the root.
--------------------------------------------------------------------
*/

# include "recycle.h"

#include <string.h>
#include <stdlib.h>

reroot *remkroot(
   size_t size
) {
   reroot *r = (reroot *)remalloc(sizeof(reroot));
   r->list = (recycle *)0;
   r->trash = (recycle *)0;
   r->size = align(size);
   r->logsize = RESTART;
   r->numleft = 0;
   return r;
}

void refree(
   struct reroot *r
) {
   while (r->list)
   {
      recycle *temp = r->list->next;
      free((char *)r->list);
      r->list = temp;
   }
   free((char *)r);
   return;
}

/* to be called from the macro renew only */
char  *renewx(
   struct reroot *r
) {
   recycle *temp;
   if (r->trash)
   {  /* pull a node off the trash heap */
      temp = r->trash;
      r->trash = temp->next;
      (void)memset((void *)temp, 0, r->size);
   }
   else
   {  /* allocate a new block of nodes */
      r->numleft = r->size*((uint32_t)1<<r->logsize);
      if (r->numleft < REMAX) ++r->logsize;
      temp = (recycle *)remalloc(sizeof(recycle) + r->numleft);
      temp->next = r->list;
      r->list = temp;
      r->numleft-=r->size;
      temp = (recycle *)((char *)(r->list+1)+r->numleft);
   }
   return (char *)temp;
}

char   *remalloc(
   size_t len
) {
  char *x = (char *)malloc(len);
  if (!x)
  {
    //exit(SUCCESS);  // let's just bug silently...
  }
  return x;
}

