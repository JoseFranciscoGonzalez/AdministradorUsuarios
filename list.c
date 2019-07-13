/*list.c*/

/*---------------------------------------------------------------------------
Primitivas de la estructura list_s. Una lista.
----------------------------------------------------------------------------*/

#include "list.h" 

/*---------------------------------------------------------------------------
PRIMITIVA: Determina si una lista está vacia.

ARGUMENTO: list 	Una lista.

RETORNO: int 		0 si está vacia. 1 si no.
----------------------------------------------------------------------------*/
int LIST_isempty(const list_s list)
{
	return (list==NULL)?0:1;
}


/*---------------------------------------------------------------------------
PRIMITIVA: Inicializa una lista.

ARGUMENTO: list  		Un puntero a una lista.

RETORNO: list_trval_t 	LIST_RV_ILLEGAL si la lista ya está inicializada.
						LIST_RV_SUCCESS en éxito.
----------------------------------------------------------------------------*/
list_rtval_t LIST_create(list_s* list)
{
	if(list==NULL)
		return LIST_RV_ILLEGAL;
	*list=NULL;
	return LIST_RV_SUCCESS;			/* Una lista está vacia si apunta a Null */
}



/*---------------------------------------------------------------------------
PRIMITIVA: Crea un nodo.

ARGUMENTO: ptr_node		Un puntero doble a un nodo / Un puntero a una lista.
ARGUMENTO: new_data		Un puntero al dato que será contenido.

RETORNO: list_trval_t 	LIST_RV_ILLEGAL si la lista está vacia..
						LIST_RV_NO_SPACE si no hay memoria.
						LIST_RV_SUCCESS en éxito.
----------------------------------------------------------------------------*/
list_rtval_t LIST_create_node(node_s** ptr_node, void* new_data)
{
	if(ptr_node==NULL)
		return LIST_RV_ILLEGAL;

	if((*ptr_node=(node_s*)calloc(1,sizeof(node_s)))==NULL)
		return LIST_RV_NO_SPACE;

	(*ptr_node)->next=NULL;

	(*ptr_node)->data=new_data;

	return LIST_RV_SUCCESS;
}


/*---------------------------------------------------------------------------
PRIMITIVA: Destruye un nodo y su contenido.

ARGUMENTO: ptr_node		Un puntero doble a un nodo / Un puntero a una lista.
ARGUMENTO: destroyer	Un destructor del tipo de dato contenido.

RETORNO: list_trval_t 	LIST_RV_ILLEGAL si la lista está vacia.
						LIST_RV_SUCCESS en éxito.
----------------------------------------------------------------------------*/
list_rtval_t LIST_destroy_node(node_s**ptr_node, void(*destroyer)(void*))
{
	if(ptr_node==NULL)
		return LIST_RV_ILLEGAL;

	if(destroyer!=NULL)
		(*destroyer)((*ptr_node)->data);

	(*ptr_node)->data=NULL;
	(*ptr_node)->next=NULL;
	free(*ptr_node);

	return LIST_RV_SUCCESS;
}



/*---------------------------------------------------------------------------
PRIMITIVA: Destruye una lista y todo su contenido.

ARGUMENTO: ptr_list		Un puntero doble a un nodo / Un puntero a una lista.
ARGUMENTO: destroyer	Un destructor del tipo de dato contenido.

RETORNO: list_trval_t 	LIST_RV_ILLEGAL si la lista está vacia.
						LIST_RV_SUCCESS en éxito.
----------------------------------------------------------------------------*/
list_rtval_t LIST_destroy(list_s* ptr_list, void (*destroyer)(void*))
{
	node_s* first;

	if(ptr_list==NULL)
		return LIST_RV_ILLEGAL;
	while(*ptr_list != NULL)
	{
		first=*ptr_list;
		*ptr_list = (*ptr_list)->next;

		LIST_destroy_node(&first, destroyer);

	}

	return LIST_RV_SUCCESS;
}


/*---------------------------------------------------------------------------
PRIMITIVA: Crea un nodo y lo ubica al principio de una lista.

ARGUMENTO: ptr_list		Un puntero doble a un nodo / Un puntero a una lista.
ARGUMENTO: data 		Un puntero al contenido del nodo.

RETORNO: list_trval_t 	LIST_RV_ILLEGAL si el puntero ptr_list es nulo.
						LIST_RV_NO_SPACE si no hay memoria.
						LIST_RV_SUCCESS en éxito.
----------------------------------------------------------------------------*/
list_rtval_t LIST_insert_first_node(list_s* ptr_list, void* data)
{
	list_rtval_t rv;
	node_s* node;

	if(ptr_list==NULL)
		return LIST_RV_ILLEGAL;

	if((rv=LIST_create_node(&node,data))!=LIST_RV_SUCCESS)
		return rv;

	node->next=*ptr_list;
	*ptr_list=node;

	return LIST_RV_SUCCESS;
}


/*---------------------------------------------------------------------------
PRIMITIVA: Busca en una lista.

ARGUMENTO: lista		Una lista/Un puntero a un nodo.
ARGUMENTO: cmp			Criterio de comparación.

RETORNO: void* 			NULL si falla.
						Un puntero al contenido del nodo en éxito.
----------------------------------------------------------------------------*/
void* LIST_search(list_s list, void* target, int (*cmp)(void*,void*))
{
	if(!target || !cmp)
		return NULL;
	while(list!=NULL && (*cmp)(target,list->data)!=0)
		list=list->next;

	return (list==NULL)?NULL:list->data;

}


/*---------------------------------------------------------------------------
PRIMITIVA: Recorre todos los nodos de una lista y realiza alguna operación con
		   los datos contenidos.

ARGUMENTO: lista		Una puntero a lista / Un puntero doble a un nodo.
ARGUMENTO: func			Función sobre el dato.

RETORNO: list_rtval_t	LIST_RV_ILLEGAL si falla.
						LIST_RV_SUCCESS en éxito.
----------------------------------------------------------------------------*/
list_rtval_t LIST_travel(const list_s* list, void(*func)(void*,void*), void* arg1)
{
	list_s ptr;

	if(!(list) || !func)
		return LIST_RV_ILLEGAL;

	ptr=*list;

	while(ptr!=NULL)
	{
		(*func)((ptr)->data,arg1);
		ptr= (ptr)->next;
	}
	return LIST_RV_SUCCESS;

}
	

/*---------------------------------------------------------------------------
PRIMITIVA: Devuelve un puntero al siguiente nodo de un nodo.

ARGUMENTO: node 		Una puntero a nodo.

RETORNO: node_s*		NULL si es el último de la lista.
----------------------------------------------------------------------------*/
node_s* LIST_next(node_s* node)
{
	return node->next;	/* No falla si node==NULL?*/
}
	

/*---------------------------------------------------------------------------
PRIMITIVA: Devuelve la cantidad de nodos de una lista.

ARGUMENTO: start 		Comienzo de la lista. 

RETORNO: size_t			Tamaño de la lista.
----------------------------------------------------------------------------*/
size_t LIST_size(list_s start)
{
	size_t size;

	if(start==NULL)
		return 0;

	size=1;
	while((start->next)!=NULL)
	{
		size++;
		start=start->next;
	}

	return size;
}