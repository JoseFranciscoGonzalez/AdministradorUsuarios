/*array.c*/

/*---------------------------------------------------------------------------
Primitivas de la estructura array_s. Un arreglo mutable.
----------------------------------------------------------------------------*/

#include "array.h"

/*---------------------------------------------------------------------------
PRIMITIVA: Crea una estructura array_s con un vector array_s de tamaño size
		   pasado como argumento.

ARGUMENTO: size  Tamaño inicial del arreglo.

RETORNO: array_s* Puntero a una estructura array_s.
----------------------------------------------------------------------------*/
array_s* ARRAY_create(size_t size)
{
	array_s* aux;

	if(!size)
		return NULL;
	if((aux=(array_s*)calloc(1,sizeof(array_s)))==NULL)	
		return NULL;
	if((aux->adt_array=(void**)malloc(size*sizeof(void*)))==NULL)
	{
		free(aux);
		return NULL;
	}

	aux->alloc=size;
	aux->used=0;

	return aux;
}


/*---------------------------------------------------------------------------
PRIMITIVA: Devuelve el campo used de una estructura user_s.

ARGUMENTO: target	Puntero a la estructura.

RETORNO: size_t		Valor de used.
----------------------------------------------------------------------------*/
size_t ARRAY_used(array_s* target)
{
	if(!target)
		return -1;	
	
	return (target->used);
}


/*---------------------------------------------------------------------------
PRIMITIVA: Destruye una estructura array_s.

ARGUMENTO: target 	Puntero a la estructura.
----------------------------------------------------------------------------*/
void ARRAY_destroy(array_s* target)
{
	size_t i;

	if(target!=NULL)
	{
		if(((target)->adt_array)!=NULL)
		{
			for(i=0; i<(target->used);i++)
			{
				if((((target)->adt_array)[i])!=NULL)
					free(((target)->adt_array)[i]);
			}
			free(target->adt_array);
		}
		(target)->adt_array=NULL;
		free(target);
		target=NULL;
	}									

}


/*---------------------------------------------------------------------------
PRIMITIVA: Determina si un array_s está vacio.

ARGUMENTO: target  Puntero a la estructura.

RETORNO: bool_t 	TRUE si está vacio. FALSE caso contrario.
----------------------------------------------------------------------------*/
bool_t ARRAY_isempty(array_s* target)
{
	return target->used==0;
}


/*---------------------------------------------------------------------------
PRIMITIVA: Devuelve un puntero a un elemento del arreglo.

ARGUMENTO: target  Puntero a la estructura.
ARGUMENTO: i 	   Posición en el arreglo.

RETORNO: void*     Puntero al elemento. NULL si no existe.
----------------------------------------------------------------------------*/
void* ARRAY_get(const array_s* target, int i)
{
	if(!target || target->used==0 )						/* no se puede buscar si no se cargó nada */
		return NULL;
	if(i< target->used && i>=0)							/* si es menor a la cantidad de usados da el puntero */
		return (target->adt_array)[i]; 
	else if(i<0 && -i<target->used)						/* si es neg. busca desde el final */
	{
		return (target->adt_array)[target->used-i];
	}
	return NULL;
}


/*---------------------------------------------------------------------------
PRIMITIVA: Guarda en la próxima posición disponible. Agrega memoria si es
		   necesario.

ARGUMENTO: target  Puntero a la estructura.
ARGUMENTO: data    Puntero al elemento a insertar.

RETORNO: bool_t    FALSE si falla. TRUE si no falla.
----------------------------------------------------------------------------*/
bool_t ARRAY_insert(array_s* target, void* data)
{
	void** aux;											/* auxiliar para realocar memoria */

	if(!target)
		return FALSE;
	if(target->used == target->alloc)					/* si se alcanzó el máximo del vector se agrega memoria */
	{
		if((aux=(void**)realloc(target->adt_array,(target->alloc)*(sizeof(void*)*ARRAY_GROW_SPEED)))==NULL)
			return FALSE;

		target->adt_array=aux;
		target->alloc *= ARRAY_GROW_SPEED;				/*se multiplica el tamaño previo por una constante */

	}

	((target->adt_array)[target->used])=data;			/* Asigno el puntero void pasado por arg. al último puntero del arreglo*/
	target->used++;

	return TRUE;

}

