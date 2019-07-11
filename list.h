/*list.h*/

#ifndef LIST__H
#define LIST__H

/*------------------------------ENCABEZADOS---------------------------------*/
#include <stdio.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------*/

/*--------------------------------TIPOS-------------------------------------*/
/*-------------------------TIPO DE DATO ABSTRACTO---------------------------*/
typedef struct node
{
struct node* next;
void* data;
}node_s, *list_s;

typedef enum
{
LIST_RV_SUCCESS,
LIST_RV_ILLEGAL,
LIST_RV_NO_SPACE,
LIST_RV_NULL
}list_rtval_t;
/*--------------------------------------------------------------------------*/

/*------------------------------PROTOTIPOS---------------------------------*/
int LIST_isempty(const list_s list);
list_rtval_t LIST_create(list_s* list);
list_rtval_t LIST_create_node(node_s** ptr_node, void* new_data);
list_rtval_t LIST_destroy_node(node_s**ptr_node, void(*destroyer)(void*));
list_rtval_t LIST_destroy(list_s* ptr_list, void (*destroyer)(void*));
list_rtval_t LIST_insert_first_node(list_s* ptr_list, void* data);
void* LIST_search(list_s list, void* target, int (*cmp)(void*,void*));
list_rtval_t LIST_travel(const list_s* list, void(*func)(void*,void*), void* arg1);
node_s* LIST_next(node_s* node);
size_t LIST_size(list_s start);
/*--------------------------------------------------------------------------*/

#endif
