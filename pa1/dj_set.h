//
//  dj_set.h
//  
//
//  Created by Aidan Daly on 2/25/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef _dj_set_h
#define _dj_set_h

typedef struct node node;

node *makeset(int x);
node *find(node *n);
void dj_union(node *x, node *y);

#endif
