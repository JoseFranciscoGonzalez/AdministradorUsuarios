/* user.c */

/*---------------------------------------------------------------------------
Primitivas de la estructura user_s.
----------------------------------------------------------------------------*/

#include "user.h"

/*---------------------------------------------------------------------------
FUNCION: Vuelca la información con tenida en un archivo de
configuración en una estructura user_s.
ARGUMENTO: infile	Flujo de entrada.
ARGUMENTO: user 	Puntero doble a una estructura user_s.

COMENTARIO: get_user pide memoria para la estructura.
COMENTARIO: Carga la estructura y devuelve ST_OK si leyo una pero 
			hay más por leer(no se acabo el archivo).
			Carga la estructura y devuelve ST_EOF si leyo la ultima
			estructura.
----------------------------------------------------------------------------*/
status_t get_user(FILE* infile, user_s** user)
{
	
	user_s* user_aux;		
	char* line;
	char* aux_ptr;
	char* peoc;
	array_s* aux_adt;
	size_t i;

	char *opt[] = {ID_OPTION,NAME_OPTION,FRIENDS_OPTION, MESSAGE_OPTION};	/* Identificadores del archivo .ini */
	
	if(!infile || !user )
		return ST_ERROR_NULL_POINTER;
	
	if((user_aux=(user_s*)calloc(1,sizeof(user_s)))==NULL)
	{
		return ST_ERROR_NO_MEM;
	}
	*user_aux=empty_user;

	while(get_line(&line, infile)==ST_OK)
	{
		
		if(feof(infile))
		{
			destroy_user(user_aux);
			free(line);
			return ST_EOF;
		}

		if(strlen(line)<1)										/* linea en blanco indica fin de la secuencia de valores*/
			break;
	
		if(line[0]==COMMENT_CHAR)								/* verifica si la línea es un comentario */
		{
			free(line);
			continue;
		}

		/* USERNAME */
		if(line[0]==LEFT_SECTION_CHAR || line[strlen(line)-1]==RIGHT_SECTION_CHAR)	
		{	
			user_aux->username=strdup(&(line[1]));				/* Guarda el nombre de usuario */
			aux_ptr=strchr(user_aux->username,RIGHT_SECTION_CHAR);
			*aux_ptr='\0';
			free(line);
			continue;
		}
		
		/* VALUES */
		for(i=0;i<sizeof(opt)/sizeof(opt[0]);i++)				/* Compara con las opciones disponibles */
		{
			if(strncmp(line,opt[i],strlen(opt[i]))==0)
				break;
			else 
			{
				continue;
			}
		}
	
		if(i==(sizeof(opt)/sizeof(opt[0])))						/* Si no hubo ning coincidencia no se guarda el usuario */
		{	
			destroy_user(user_aux);
			free(line);
			return ST_ERROR_INVALID_IDENTIFIER;
		}
		
		switch(i)												/* Dependiendo del tipo de dato lo guarda en la estructura */
		{
			/* ID */
			case(0):
				aux_ptr=(line+strlen(ID_OPTION)+2);
				user_aux->id=strtol(aux_ptr, &peoc, DECIMAL_BASE);				
				if((*peoc)!='\0' || (user_aux->id)<0) 	
				{
					user_aux->id=0;
					free(line);
					destroy_user(user_aux);
					return ST_ERROR_INVALID_ID;
				}
				free(line);
				break;

			/* NAME */
			case(1):
				aux_ptr=(line+strlen(NAME_OPTION)+2);
				user_aux->name=strdup(aux_ptr);
				free(line);
				break;

			/* FRIENDS */
			case(2):
				aux_ptr=(line+strlen(FRIENDS_OPTION)+2);
				if((aux_adt=get_friends(aux_ptr))==NULL)
				{
					destroy_user(user_aux);
					free(line);
					return ST_ERROR_INVALID_FRIENDS;
				}
				user_aux->friends=aux_adt;
				free(line);
				break;

			/* MESSAGE */
			case(3):
				aux_ptr=(line+strlen(MESSAGE_OPTION)+2);
				if((get_messages(aux_ptr,&(user_aux->messages)))!=ST_OK)
				{
					destroy_user(user_aux);
					free(line);
					return ST_ERROR_INVALID_MESSAGE;
				}
				free(line);
				break;

			default:
				destroy_user(user_aux);
				free(line);
				return ST_ERROR_INVALID_OPTION;
		}

	}

	*user=user_aux;

	free(line);

	return ST_OK;

}


/*---------------------------------------------------------------------------
FUNCION: Lee una linea de un archivo de largo indefinido.

ARGUMENTO: s  		Puntero doble al comiezo de la cadena.
ARGUMENTO: f 		Flujo de entrada.

RETORNO: status_t 	ST_ERROR_NULL_POINTER si hay argumento nulo.
					ST_ERROR_NO_MEM si no hay memoria.
					ST_ERROR_READ si se prende el flag de error del archivo.
					ST_OK en éxito.
----------------------------------------------------------------------------*/
status_t get_line( char** s, FILE* f)
{
	char* aux;
	size_t start; 

	if((*s=(char*)malloc((CHOP_SIZE+1)*sizeof(char)))==NULL)
		return ST_ERROR_NO_MEM;

	start=0;

	while(fgets(*s+start,CHOP_SIZE+1,f)!=NULL)
	{
		if((*s+start)[strlen(*s+start)-1]=='\n')
		{
			(*s+start)[strlen(*s+start)-1]='\0';
			return ST_OK;
		}

		start+=CHOP_SIZE;
		if((aux=(char*)realloc(*s,(start+1+CHOP_SIZE)*sizeof(char)))==NULL)
		{
			free(*s);
			return ST_ERROR_NO_MEM;
		}
		*s=aux;
	}
	if(ferror(f))
	{
		free(*s);
		return ST_ERROR_READ;
	}

	return ST_OK;
}


/*---------------------------------------------------------------------------
FUNCION: Recibe una cadena con mensajes, los separa y los devuelve como una 
		 lista.

ARGUMENTO: string  	Cadena.
ARGUMENTO: msg_list Puntero a una lista.

RETORNO: status_t 	ST_ERROR_NULL_POINTER si hay argumento nulo.
					ST_ERROR_NO_MEM si no hay memoria.
					ST_ERROR_INVALID_MSG_ID si no se puede convertid el Id del msj.
					ST_ERROR_INVALID_DATE si no se puede convertir la fecha.
					..etc
					ST_OK en éxito.
----------------------------------------------------------------------------*/
status_t get_messages(const char* string, list_s* msg_list)
{
	char* aux;
	char* peoc;
	char** fields_array;
	message_s* msg;
	size_t *copied_fields;

	if(!string || !msg_list)
		return ST_ERROR_NULL_POINTER;
	
	if((aux=strdup(string))==NULL)
	{
		return ST_ERROR_NO_MEM;
	}

	if((split(aux,MSG_DELIMITER,&fields_array,&copied_fields))!=ST_OK)		/* Se separan los campos */
	{																		/* y se guardan en un vector */
		free(aux);
		return ST_ERROR_NO_MEM;									
	}


	if((msg=(message_s*)calloc(1,sizeof(message_s)))==NULL)
	{
		free(aux);
		destroy_array_from_string(&fields_array,*copied_fields);
		free(copied_fields);
		return ST_ERROR_NO_MEM;
	}

	msg->msg_id=strtoul(fields_array[MSG_ID_POSITION], &peoc, DECIMAL_BASE);
	if(*peoc!='\0')
	{	
		free(aux);
		free(msg);
		destroy_array_from_string(&fields_array,*copied_fields);
		free(copied_fields);
		return ST_ERROR_INVALID_MSG_ID;
	}

	if(get_date(fields_array, msg)!=ST_OK)
	{
		free(aux);
		free(msg);
		destroy_array_from_string(&fields_array,*copied_fields);
		free(copied_fields);
		return ST_ERROR_INVALID_DATE;
	}

	msg->usr_id=strtoul(fields_array[MSG_USER_POSITION], &peoc, DECIMAL_BASE);
	if(*peoc!='\0')
	{
		free(aux);
		free(msg);
		destroy_array_from_string(&fields_array,*copied_fields);
		free(copied_fields);
		return ST_ERROR_INVALID_ID;
	}

	msg->message=strdup(fields_array[MSG_STRING_POSITION]);

	if(LIST_insert_first_node(msg_list, (void*)msg)!=LIST_RV_SUCCESS)
	{
		free(aux);
		free(msg);
		destroy_array_from_string(&fields_array, *copied_fields);
		free(copied_fields);
		/*LIST_destroy(&msg_list,(void(*)(void*)) destroy_messages);*/
		return ST_ERROR_INSERT;
	}

	free(aux);
	destroy_array_from_string(&fields_array,*copied_fields);
	free(copied_fields);
	return ST_OK;

}

/*---------------------------------------------------------------------------
FUNCION: Recibe una fecha en cadena y la convierte en una estructura message_s.

ARGUMENTO: fields_array 	Vector de punteros a char. En una posición 
							predefinida se la fecha. (MEJORAR)
ARGUMENTO: msg 				Puntero a estructura message_s.

RETORNO: status_t 	ST_ERROR_INVALID_DATE si no se puede convertir la fecha.
					ST_OK en éxito.
----------------------------------------------------------------------------*/
status_t get_date(char **fields_array, message_s *msg)
{
	struct tm date={0};			/* estructura tm inicializada en cero */
	time_t tm_date;				/* fecha convertida en time_t */
	size_t *copy_fields;		/* puntero a un contador de campos copiados */
	char **time_array;			/* vector de punteros a los campos separados */
	char *ptr_eoc;				/* puntero al final de cadena */

	if(fields_array==NULL || !msg)
	{	
		return ST_ERROR_NULL_POINTER;
	}

	
	if(split(fields_array[MSG_DATE_POSITION],DATE_DELIMITER,&time_array,&copy_fields)!=ST_OK)		
	{																						
		return ST_ERROR_NO_MEM;												
	}
	/* guarda el año */
	date.tm_year = (strtol(time_array[YEAR_POSITION],&ptr_eoc,DECIMAL_BASE)-1900);	
	if(*ptr_eoc!='\0')														
	{
		destroy_array_from_string(&time_array,*copy_fields);
		free(copy_fields);
		return ST_ERROR_INVALID_DATE;
	}
	/* guarda el mes */
	date.tm_mon = (strtol((time_array)[MONTH_POSITION],&ptr_eoc,DECIMAL_BASE)-1);	
	if(*ptr_eoc!='\0')														
	{	
		destroy_array_from_string(&time_array,*copy_fields);
		free(copy_fields);
		return ST_ERROR_INVALID_DATE;
	}
	/* guarda el día */
	date.tm_mday = strtol((time_array)[DAY_POSITION],&ptr_eoc,DECIMAL_BASE);	
	if(*ptr_eoc!='\0')														
	{
		destroy_array_from_string(&time_array,*copy_fields);
		free(copy_fields);
		return ST_ERROR_INVALID_DATE;
	}
	
	destroy_array_from_string(&time_array,*copy_fields);
	free(copy_fields);
	
	tm_date = mktime(&date);								/* convierte a time_t */
    	if(tm_date == (time_t)-1)
    		return ST_ERROR_INVALID_DATE_RANGE;
    
    (msg)->timestamp = tm_date;						/* guarda en la estructura exterior */
	
    return ST_OK;


}	


/*---------------------------------------------------------------------------
FUNCION: Devuelve una vector de adt a partir de la info. contenida en una
		 cadena del tipo: id/id/id/id.....

ARGUMENTO: string 	Cadena.

RETORNO: array_s* 	NULL en error. Puntero a la estructura en éxito.
----------------------------------------------------------------------------*/
array_s* get_friends(const char* string)
{

	char* aux;
	char* peoc;
	char** fields_array;				/* cadenas del split */
	size_t* copied_fields;				/* campos del split copiados */
	size_t i;							/* contador */
	int* id;							/* arreglo de enteros */
	array_s* friends_array;			/* arreglo dinamico */


	if((aux=strdup(string))==NULL)										/* copia local de la cadena */
	{
		return NULL;
	}

	if((split(aux,MSG_DELIMITER,&fields_array,&copied_fields))!=ST_OK)	/* Se separan los campos */
	{																	/* y se guardan en un vector */
		free(aux);
		return NULL;									
	}
	
	if((friends_array = ARRAY_create(*copied_fields))==NULL)				/* Crea una arreglo adt */
	{
		free(aux);
		ARRAY_destroy(friends_array);
		destroy_array_from_string(&fields_array, *copied_fields);
		free(copied_fields);
		return NULL;
	}

	if((id=(int*)malloc(sizeof(int)*(*copied_fields)))==NULL)			/* Memoria para ids */
	{
		free(aux);
		ARRAY_destroy(friends_array);
		destroy_array_from_string(&fields_array, *copied_fields);
		free(copied_fields);
		return NULL;
	}

	for(i=0;i<(*copied_fields);i++)										/* Convierte cadena a int */
	{	
		id[i]=strtol(fields_array[i],&peoc,DECIMAL_BASE);
		if(*peoc!='\0')
		{
			free(aux);
			free(id);
			ARRAY_destroy(friends_array);
			destroy_array_from_string(&fields_array, *copied_fields);
			free(copied_fields);
			ARRAY_destroy(friends_array);
			return NULL;
		}
		
		if((ARRAY_insert(friends_array,(void*)&id[i]))==FALSE)		/* Posiciona el puntero a int en el arreglo */
		{
			free(aux);
			free(id);
			ARRAY_destroy(friends_array);
			destroy_array_from_string(&fields_array, *copied_fields);
			free(copied_fields);
			return NULL;
		}
		
	}
	destroy_array_from_string(&fields_array, *copied_fields);
	free(copied_fields);
	free(aux);
	return friends_array;

}


/*----------------------------------------------------------------------------
FUNCION: Imprime una estructura user_s en el flujo pasado por argumento

ARGUMENTO: user 		Puntero a usuario.				
ARGUMENTO: outfile	 	Flujo de salida.
----------------------------------------------------------------------------*/
status_t print_user(const user_s* user,FILE* outfile)
{

	size_t i;

	if(!outfile || !user )
		return ST_ERROR_NULL_POINTER;

	/* USERNAME */
	fprintf(outfile,"[%s]\n",user->username);
	/* ID */
	fprintf(outfile,"%s = %i\n",ID_OPTION, user->id);
	/* NAME */
	fprintf(outfile,"%s = %s\n", NAME_OPTION, user->name);
	/* FRIENDS */
	fprintf(outfile, "%s = ", FRIENDS_OPTION);
	for(i=0; i<ARRAY_used((user->friends)); i++)
	{
		fprintf(outfile,"%i/" ,*(int*)ARRAY_get((user->friends),i));
	}
	fprintf(outfile,"\n");
	/* MESSAGES */
	if(LIST_travel(&(user->messages),(void(*)(void*,void*)) print_message,(FILE*)outfile)!=LIST_RV_SUCCESS)
	{
		return ST_ERROR_PRINT;
	}
	fprintf(outfile, "\n");
	return ST_OK;

}



/*---------------------------------------------------------------------------
FUNCION: Imprime una estructura message_s.
ARGUMENTO: msg 	Puntero a estructura message_s.
ARGUMENTO: outfile	Flujo por donde se imprime.
RETORNO: status_t.
---------------------------------------------------------------------------*/
status_t print_message(message_s* msg, FILE* outfile)
{

	struct tm *t;									/* puntero estructura time */
	time_t date;									/* variable de fecha */
	char aux[SHORT_STRING];							/* cadena auxiliar para imprimir fecha*/

	if(!msg || !outfile)
		return ST_ERROR_NULL_POINTER;

	date=((msg)->timestamp);						/* conversion del time_t a cadena */			
  	t=localtime(&date);
  	strftime(aux,100,"%Y-%m-%d",t);
	fprintf(outfile,"%s = %i/%s/%i/%s\n",MESSAGE_OPTION,msg->msg_id,aux,msg->usr_id,msg->message);
	
	return ST_OK;
}



/*----------------------------------------------------------------------------
FUNCION: Crea un archivo [id]-[username].text e imprime el respectivo usuario 
		 en él.

ARGUMENTO: user 	Puntero a estructura a guardar.
ARGUMENTO: st 		Puntero a varialbe de estado.
----------------------------------------------------------------------------*/
void save_user(user_s* user, status_t* st)
{
	char* file_name;					/* Archivo de salida */
	char ext[]=TXT_FILE_EXTENSION;		/* Extension del archivo de salida */
	char id[SHORT_STRING];				/* Id, cantidad máxima de char por int */
	FILE* fileout;

	if(!user || !st)
	{	
		*st=ST_ERROR_NULL_POINTER;
		return;
	}
	if(sprintf(id,"%i-",user->id)<0)
	{
		*st= ST_ERROR_INVALID_ID;
		return;
	}
	if((file_name=(char*)malloc(sizeof(char)*(strlen(user->username)+strlen(id)+strlen(ext)+1)))==NULL)
	{
		*st= ST_ERROR_NO_MEM;
		return;
	}
	if(strcpy(file_name,id)==NULL)
	{
		free(file_name);
		*st=ST_ERROR_INVALID_USERNAME;
		return;
	}
	if(strcat(file_name,user->username)==NULL)
	{
		free(file_name);
		*st=ST_ERROR_INVALID_USERNAME;
		return;
	}
	if(strcat(file_name,ext)==NULL)
	{
		free(file_name);
		*st=ST_ERROR_INVALID_USERNAME;
		return;
	}
	if((fileout=fopen(file_name,FILE_CREATE_MODE))==NULL)
	{
		free(file_name);
		*st=ST_ERROR_CREATE_FILE;
		return;
	}
	if(print_user(user,fileout)!=ST_OK)
	{
		free(file_name);
		fclose(fileout);
		*st=ST_ERROR_PRINT;
		return;
	}
	/* Imprime por pantalla el archivo generado */
	fprintf(stdout,"%s\n",file_name);
	
	free(file_name);
	fclose(fileout);
	*st=ST_OK;

}


/*----------------------------------------------------------------------------
FUNCION: Destruye una estructura de usuario user_s.

ARGUMENTO: target	Puntero a estructura a eliminar.
----------------------------------------------------------------------------*/
void destroy_user(user_s* target)
{	
	if(!target)
		return;
	
	if(target->name!=NULL)
	{
		free(target->name);
		target->name=NULL;
	}
	if((target->username)!=NULL)
	{
		free(target->username);
		target->username=NULL;
	}
	if(ARRAY_used((target->friends))!=-1)
	{
		free(ARRAY_get((target->friends),0));		
		((target->friends)->used)=0;		/*esto no se puede hacer CORREGIR*/
	}
	ARRAY_destroy((target->friends));
	LIST_destroy(&(target->messages),(void(*)(void*))destroy_messages);

	free(target);

	return;
}



/*---------------------------------------------------------------------------
FUNCION: Destruye una estructura message_s.

ARGUMENTO: target	Puntero a estructura a eliminar.
---------------------------------------------------------------------------*/
void destroy_messages(message_s* target)
{
	if(!target)
		return;

	if(target->message)
		free(target->message);

	free(target);
	target=NULL;

}


/*---------------------------------------------------------------------------
FUNCION: Compara un usuario con un Id o un Usernane

RETORNO: 0	Si encuentra coincidencia
		 -1 Caso contrario
---------------------------------------------------------------------------*/
int compare_user(const user_s* target ,const user_s* user)
{	
	if(!target || !user)
		return -1;
	
	/* Si no hay info del objetivo sale */
	if(target->id==0 && target->username==NULL)
		return -1;
	/* Compara Id */
	if(target->id!=0)
	{
		if(target->id==user->id)
			return 0;
	}
	/* Compara Username*/
	if(target->username!=NULL)
	{
		if(strcmp(target->username,user->username)==0){
			return 0;
		}
	}
	
	return -1;

}



/*----------------------------------------------------------------------------
FUNCION: Separa los campos de una cadena separados por delimitadores.

ARGUMENTO: cadena		Cadena.
ARGUMENTO: delimitador	Caracter separador de campos.
ARGUMENTO: arreglo		Vector de punteros a char dinámico.
ARGUMENTO: l 			Cantidad de campos copiados.

RETORNO: status_t
----------------------------------------------------------------------------*/
status_t split(const char* cadena, char delimitador, char*** arreglo, size_t **l)
{
	char** campos;
	size_t n,i;
	char* aux, *q, *linea, delim[2];

	if(cadena==NULL|| l==NULL)
	{
		return ST_ERROR_NULL_POINTER;
	}
	for(i=0,n=1;cadena[i];i++){
		if(cadena[i]==delimitador){
			n++;
		}
	}
	if((campos=(char**)malloc(sizeof(char*)*n))==NULL){
		return ST_ERROR_NO_MEM;
	}
	if((linea=strdup(cadena))==NULL){
		free(campos);
		*arreglo=NULL;
		*l=0;
		return ST_ERROR_NO_MEM;
	}
	delim[0]=delimitador;
	delim[1]='\0';
	for(i=0, q=linea; (aux=strtok(q,delim))!=NULL;i++,q=NULL){
		if((campos[i]=strdup(aux))==NULL){
			destroy_array_from_string(&campos,i);
			*arreglo=NULL;
			*l=0;
			return ST_ERROR_NO_MEM;
		}
	}
	free(linea);
	
	if((*l=(size_t*)malloc(sizeof(size_t)))==NULL) 
	{
		return ST_ERROR_NULL_POINTER;
	}
	**l=i;
	*arreglo=campos;
	return ST_OK;
}


/*----------------------------------------------------------------------------
FUNCION: Copia una cadena con memoria dinámica.

ARGUMENTO: sc	Cadena original.

RETORNO: char* 	Cadena copiada. NULL si falla.		
----------------------------------------------------------------------------*/
char* strdup(const char*sc)
{
	char *s;
	if(sc==NULL)
		return NULL;
	if((s=(char*)malloc(sizeof(char)*(strlen(sc)+1)))==NULL)
		return NULL;
	strcpy(s,sc);
	return s;
}


/*----------------------------------------------------------------------------
FUNCION: Destruye un arreglo de cadenas.

ARGUMENTO: campos	Arreglo a destruir.
ARGUMENTO: i 		Tamaño del arreglo.

RETORNO: status_t 		
----------------------------------------------------------------------------*/
status_t destroy_array_from_string(char*** campos,size_t i)
{
	size_t j;
	if(campos==NULL || *campos==NULL)
		return ST_OK;
	for(j=0;j<i;j++){
		free((*campos)[j]);
		(*campos)[j]=NULL;
	}
	free(*campos);
	(*campos)=NULL;
	return ST_OK;
}