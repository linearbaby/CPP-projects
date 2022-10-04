//
// Created by artem on 15.10.2020.
//

#ifndef IZ1_RELEASE_MYLIST_H
#define IZ1_RELEASE_MYLIST_H


#include "ship_header.h"

typedef struct node{
    struct node *next;
    struct node *prev;
    info *data;
} node;

node * init_node();
void delete_node(node *);
node *load(FILE*);
node *search_and_delete(node *);
node *add(node*);
int write_list(FILE*, node*);
int find_ships(node *);


//helper functions
node *tohead(node *);
node *totail(node *);
//end of helper functions


#endif //IZ1_RELEASE_MYLIST_H
