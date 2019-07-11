/* array.h */
	
#ifndef ARRAY__H
#define ARRAY__H

/*------------------------------ENCABEZADOS---------------------------------*/
#include <stdio.h>
#include <stdlib.h>
/*--------------------------------------------------------------------------*/

/*--------------------------------MACROS------------------------------------*/
#define ARRAY_GROW_SPEED 2
/*--------------------------------------------------------------------------*/

/*--------------------------------TIPOS-------------------------------------*/
/*-------------------------TIPO DE DATO ABSTRACTO---------------------------*/
typedef struct 
{
size_t alloc;
size_t used;
void** adt_array;
}array_s;

typedef enum
{
FALSE=0,
TRUE=1
}bool_t;
/*--------------------------------------------------------------------------*/

/*------------------------------PROTOTIPOS---------------------------------*/
array_s* ARRAY_create(size_t size);
void ARRAY_destroy(array_s* target);
bool_t ARRAY_isempty(array_s* target);
void* ARRAY_get(const array_s* target, int i);
bool_t ARRAY_insert(array_s* target, void* data);
size_t ARRAY_used(array_s* target);
/*--------------------------------------------------------------------------*/

#endif

