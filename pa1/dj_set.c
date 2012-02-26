#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dj_set.h"

struct node {
  struct node *prev;
  int id;
  int rank;
};

node *makeset(int x)
{
  node *new = (node *) malloc(sizeof(node));
  new->id = x;
  new->prev = new;
  new->rank = 0;
    
  return new;
}

node *find(node *x)
{
  if (x != x->prev)
    {
      //path compression - otherwise, return find(x->prev)
      x->prev = find(x->prev);
      return x->prev;
    }
  return x;
}

void link(node *x, node *y)
{
  if (x->rank > y->rank)
    {
      link(y,x);
      return;
    }
  if (x->rank == y->rank)
    y->rank++;
  x->prev = y;
}

void dj_union(node *x, node *y)
{
  link(find(x),find(y));
}

/*
  int main(int argc, char **argv)
  {
  node *a = makeset(0);
  node *b = makeset(1);
  node *c = makeset(2);
    
  dj_union(b,a);
  dj_union(b,c);
    
  printf("%d, %d\n",find(a)->id, find(a)->rank);
  printf("%d, %d\n",find(b)->id, find(b)->rank);
    
  return 1;
  }
*/
