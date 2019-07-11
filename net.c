/* net.c */

#include "net.h"

/*----------------------------------------------------------------------------
FUNCION: Crea una lista y vuelca la información de los archivos en ellas. Si
		 el id de un usuario se repite se lo incluye una sola vez.

EJECUCION: ./social_network --eliminate/-e [i:/u:] --output/-o [single/multi]
			[..files..]

----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
				
	user_s* target_user;	/* Usuario a eliminar */
	size_t n_files;				/* Cantidad de archivos de configuración */
	output_t output;			/* Modo de salida del programa */
	list_s net;					/* Red de usuarios */
	status_t st;				/* Variable de estado de ejecución */


	puts(MSG_SOCIAL_NETWORK_WELCOME);
	/* Validación de argumentos */
	if((st=validate_args(argc,argv,&target_user,&output,&n_files))!=ST_OK)
	{
		print_error(st);
		help();
		return EXIT_FAILURE;	/* Error de argumentos es terminal */
	}

	/* Inicialización de la red */
	if(LIST_create(&net)!=LIST_RV_SUCCESS)
	{
		print_error(ST_ERROR_INITIALIZE_NET);
		help();
		destroy_user(target_user);
		return EXIT_FAILURE;
	}

	/* Ciclo de apertura y almacenamiento */
	if((st=load_net(n_files, argc, argv, &net,target_user))!=ST_OK)
	{
		print_error(st);
		help();
		destroy_user(target_user);
		return EXIT_FAILURE;
	}

	/* Impresión de la red*/
	if((st=print_net(&net,output))!=ST_OK)
	{
		print_error(st);
		LIST_destroy(&net,(void(*)(void*))destroy_user);
		help();
		destroy_user(target_user);
		return EXIT_FAILURE;
	}
	
	LIST_destroy(&net,(void(*)(void*))destroy_user);
	destroy_user(target_user);
	return EXIT_SUCCESS;

}


/*-----------------------------------------------------------------------------
FUNCION: Valida argumentos por CLA, identifica si se elimina por nombre de 
		 usuario o por id, identifica si la impresión es multi o single. 

ARGUMENTO: argc			Cantidad de argumentos pasados a main.
ARGUMENTO: argv 		Vector de punteros a char con los argumentos.
ARGUMENTO: target_id	Id a eliminar.
ARGUMENTO: target_user	Nombre de usuario (username) a eliminar.
ARGUMENTO: output 		Formato de salida.

FORMATO: Las siguientes combinaciones de ejecución están contempladas por el
		 programa.

		 ./social_network --eliminate i:[id] --output [single/multi] [..files..]
		 ./social_network -o [single/multi] -e u:[user] [..files..]

RETORNO: status_s
		 ST_OK	Si se interpretan los argumento correctamente.
		 ST_CLA_INVALID...	Si algún argumento no es reconocido.
		 ST_CLA_MIN...		Si algún/os argumento/s falta.

COMENTARIOS: No se abren archivos.
			 Se pide memoria para target_user (si se invoca en modo u: [user]).
			 Como mínimo se debe pasar un archivo en [..files..].
-----------------------------------------------------------------------------*/
status_t validate_args(int argc, char* argv[], user_s** target_user, output_t* output, size_t* n_files )
{
		
	size_t i;
	int aux_id;
	char* peoc;
	char* aux_ptr;
	user_s* aux_user;
	short unsigned int key_output;
	short unsigned int key_eliminate;

	key_output=0;
	key_eliminate=0;

	if(!argv || !target_user || !output )
	{
		/* *target_user=empty_user;*/
		*n_files=0;
		return ST_ERROR_NULL_POINTER;
	}

	if(argc<CLA_MIN_ARGUMENTS)
	{
		/* *target_user=empty_user;*/
		*n_files=0;
		return ST_ERROR_CLA_MIN_ARGUMENTS;
	}

	if((aux_user=(user_s*)calloc(1,sizeof(user_s)))==NULL)
	{
		return ST_ERROR_NO_MEM;
	}
	*aux_user=empty_user;

	for(i=0;i<CLA_MIN_ARGUMENTS;i++)													
	{
 		if (strcmp(argv[i],CLA_ELIMINATE_TAG) == 0 || strcmp(argv[i],CLA_ELIMINATE_TAG_SHORT) == 0) 
 		{																			
 			key_eliminate=(i+1);															
 			continue;																
 		}																			
 		if (strcmp(argv[i],CLA_OUTPUT_TAG) == 0 || strcmp(argv[i],CLA_OUTPUT_TAG_SHORT) == 0) 
 		{
 			key_output=(i+1);
 			continue;
 		}	
	}

	if(key_eliminate==0 || key_output==0)
	{
		/* *target_user=empty_user;*/
		destroy_user(aux_user);
		*n_files=0;
		return ST_ERROR_INVALID_CLA_OPTION;						
	}
	if (strncmp(argv[key_eliminate],CLA_ELIMINATE_ID_OPTION, strlen(CLA_ELIMINATE_ID_OPTION)) == 0) 
 	{		
		aux_ptr=(argv[key_eliminate]+strlen(CLA_ELIMINATE_ID_OPTION));
		aux_id=strtoul(aux_ptr, &peoc, DECIMAL_BASE);				
		if(*peoc!='\0' || aux_id<0)
		{ 
			/* *aux_user=empty_user;*/
			destroy_user(aux_user);
			*n_files=0;	
			return ST_ERROR_INVALID_CLA_ID_OPTION;
 		}
 		aux_user->id=aux_id;
 		aux_user->username=NULL;
 	}	
 	else if(strncmp(argv[key_eliminate],CLA_ELIMINATE_USER_OPTION, strlen(CLA_ELIMINATE_USER_OPTION)) == 0)
 	{
 		aux_ptr=(argv[key_eliminate]+strlen(CLA_ELIMINATE_USER_OPTION));
 		if((aux_user->username=strdup(aux_ptr))==NULL)
 		{
 			/* *target_user=empty_user;*/
 			destroy_user(aux_user);
			*n_files=0;
 			return ST_ERROR_INVALID_CLA_USER_OPTION;
 		}
 		aux_user->id=0;

 	} 
 	else
 	{
 		/* *target_user=empty_user;*/
		*n_files=0;
		destroy_user(aux_user);
 		return ST_ERROR_NULL_POINTER;		
 	}

	if (strcmp(argv[key_output],CLA_OUTPUT_SINGLE_OPTION) == 0)
		*output = SINGLE;
 	else if(strcmp(argv[key_output],CLA_OUTPUT_MULTI_OPTION) == 0)
 		*output = MULTI;
 	else
 	{
 		/* *target_user=empty_user;*/
		*n_files=0;
		destroy_user(aux_user);
		if(aux_user->username)
			free(aux_user->username);
 		return ST_ERROR_INVALID_CLA_OUTPUT_OPTION;
 	}

 	*n_files = argc - CLA_MIN_ARGUMENTS +1;
 	*target_user=aux_user;
	return ST_OK;
}



/*----------------------------------------------------------------------------
FUNCION: Abre y almacena los archivos de configuración, evitando repeticiones.

ARGUMENTO: n_files	Cantidad de archivos de configuración.
ARGUMENTO: argc		Cantidad de elementos en el arreglo argv.
ARGUMENTO: argv 	Nombres de los archivos de configuración.
ARGUMENTO: net 		Puntero a la red de usuarios.
----------------------------------------------------------------------------*/
status_t load_net(size_t n_files, int argc, char** argv, list_s* net, user_s* target)
{
	size_t i;			/* Contador */
	size_t error_files;	/* Archivos que no se pudieron abrir */
	size_t list_size;	/* Cantidad de nodos agregados */
	size_t start;		/* Posición del primer archivo en el arreglo argv */
	status_t st;		/* Variable de estado de ejecución */
	FILE* infile;		/* Flujo de entrada */
	list_s aux_net;		/* lista auxiliar */

	list_size=0;
	error_files=0;
	start = CLA_MIN_ARGUMENTS - 1;

	if(!argv || !net)
		return ST_ERROR_NULL_POINTER;
	if(argc<CLA_MIN_ARGUMENTS)
		return ST_ERROR_CLA_MIN_ARGUMENTS;
	if(n_files<=0)
		return ST_ERROR_MIN_FILES;

	if(LIST_create(&aux_net)!=LIST_RV_SUCCESS)
		return ST_ERROR_INITIALIZE_NET;

	for(i=0; i<n_files; i++)
	{
		/* Apertura de un archivo de configuración */
		if((st=open_config_file(argv[start+i],&infile))!=ST_OK)
		{
			print_error(st);
			debug_file(argv[start+i]);
			/*save_log()*/
			error_files++;
			continue;	/* Error de apertura saltea el archivo */
		}
		/* Deteccion de repeticion, eliminación y agregado a la lista */
		if((st=add_users_to_list(infile,&aux_net, &list_size, target))!=ST_EOF)
		{
			print_error(st);
			/*save_log()*/
			fclose(infile);
			continue;	/* Error al cargar saltea el archivo */
		}
		fclose(infile);
	}

	*net=aux_net;

	if(n_files==error_files)
		return ST_ERROR_INITIALIZE_NET;

	return ST_OK;
}


/*----------------------------------------------------------------------------
FUNCION: Imprime la lista net en modo single o multi.

MODO: single	Impresión de la lista por stdout.
MODO: multi		Impresión de cada usuario en un archivo.

ARGUMENTO: list 	Puntero a la lista a imprimir.
ARGUMENTO: mode		Modo de impresión.
----------------------------------------------------------------------------*/
status_t print_net(list_s* net, output_t mode)
{
	status_t st;

	if(!net)
		return ST_ERROR_NULL_POINTER;
	if(!LIST_isempty(*net))
	{
		printf("%s\n",MSG_EMPTY_NET);
		return ST_OK;
	}
	switch(mode)
	{
		case(SINGLE):
			printf("%s:\n",MSG_LOADED_USERS);
			if(LIST_travel(net,(void(*)(void*,void*))print_user,stdout)!=LIST_RV_SUCCESS)
				return ST_ERROR_PRINT_NET;
			break;

		case(MULTI):
			printf("%s:\n",MSG_OUTPUT_FILES);
			/* La función save_user contiene un print_user más la
			generación del nombre del archivo */
			if(LIST_travel(net,(void(*)(void*,void*))save_user,&st)!=LIST_RV_SUCCESS)
				return ST_ERROR_PRINT_NET;
			if(st!=ST_OK)
				return ST_ERROR_PRINT_NET;
			break;

		default:
			return ST_ERROR_PRINT_NET;
	}

	return ST_OK;

	
}



/*----------------------------------------------------------------------------
FUNCION: Abre un archivo de configuración en modo lectura.

ARGUMENTO: file_name	Nombre del archivo.				
ARGUMENTO: config_file 	Puntero al archivo.
----------------------------------------------------------------------------*/
status_t open_config_file(const char* file_name, FILE** config_file)
{
	
	if(!file_name || !config_file)
		return ST_ERROR_NULL_POINTER;

	if((*config_file=fopen(file_name,FILE_READ_MODE))==NULL)
	{
		*config_file=NULL;
		return ST_ERROR_OPEN_FILE;
	}

	return ST_OK;
}



/*----------------------------------------------------------------------------
FUNCION: Lee usuarios de un archivo de configuración y los
		 almacena en la lista. Si la lista ya está creada la actualiza.

ARGUMENTO: infile		Flujo de entrada.
ARGUMENTO: list 		Puntero a lista a actualiza/crear.
ARGUMENTO: list_size	Cantidad de nodos de la lista.

RETORNO: status_t
----------------------------------------------------------------------------*/
status_t add_users_to_list(FILE* infile, list_s* list, size_t* list_size,const user_s* target)
{

	user_s* aux_user;	/* Usuario temporal */
	status_t st;		/* Variable de estado de ejcución */
	char* line;
	if(!infile || !list || !list_size)
		return ST_ERROR_NULL_POINTER;
	
	while((st=get_user(infile,&aux_user))!=ST_EOF)
	{
		if(st!=ST_OK)
		{	
			print_error(st);
			/* Busca la  próxima sección */
			/* Permite seguir leyendo si falla una lectura */
			while(get_line(&line, infile)==ST_OK)
			{	
				if(strlen(line)<1)										/* linea en blanco indica fin de la secuencia de valores*/
				{
					free(line);
					break;
				}
				free(line);	
			}
			st=ST_OK;
			continue;
		}
		/* Compara con la info de eliminar */
		/* Si coincide no guarda en la red */
		if(compare_user(target,aux_user)==0)
		{
			destroy_user(aux_user);
			continue;
		}
		/* Evita la inclusion múltiple */
		/* Busca si ya existe el Id o el Username */
		if(LIST_search(*list, aux_user,(int(*)(void*,void*))compare_user)!=NULL)
		{
			destroy_user(aux_user);
			continue;
		}
		/* Agrega a la red */
		if((LIST_insert_first_node(list, (void*)aux_user))!=LIST_RV_SUCCESS)
		{
			LIST_destroy(list, (void(*)(void*))destroy_user);
			return ST_ERROR_ADD_USER;
		}
	}
	if(st!=ST_EOF)
		return ST_ERROR_ADD_USER;
	else return ST_EOF;	
	
	return ST_EOF;

}


/*---------------------------------------------------------------------------
FUNCION: Imprime errores.

ARGUMENTO: st   Variable de estado de error.
----------------------------------------------------------------------------*/
void print_error(status_t st)
{
  
  switch(st)
  {
    case ST_ERROR_NULL_POINTER:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_NULL_POINTER);
      break;

    case ST_ERROR_READ:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_READ);
      break;

    case ST_ERROR_GET_USER:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_GET_USER);
      break;
	
    case ST_ERROR_ADD_USER:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_ADD_USER);
      break;
	
    case ST_ERROR_INVALID_DATE:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_DATE);
      break;
	
    case ST_ERROR_INVALID_DATE_RANGE:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_DATE_RANGE);
      break;

    case ST_ERROR_OPEN_FILE:
      fprintf(stderr,"%s:",MSG_ERROR_OPEN_FILE);
      break;

    case ST_ERROR_CREATE_FILE:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_CREATE_FILE);
      break;
  
    case ST_ERROR_INVALID_ARGUMENTS:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_ARGUMENTS);
      break;
  
    case ST_ERROR_INVALID_ID:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_ID);
      break;

    case ST_ERROR_INVALID_USERNAME:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_USERNAME);
      break;

    case ST_ERROR_CLA_MIN_ARGUMENTS:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_CLA_MIN_ARGUMENTS);
      break;
  
    case ST_ERROR_INVALID_CLA_ID_OPTION:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_CLA_ID_OPTION);
      break;
  
    case ST_ERROR_INVALID_CLA_USER_OPTION:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_CLA_USER_OPTION);
      break;

    case ST_ERROR_INVALID_CLA_OUTPUT_OPTION:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_CLA_OUTPUT_OPTION);
      break;

    case ST_ERROR_INVALID_CLA_OPTION:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_CLA_OPTION);
      break;

    case ST_ERROR_MIN_FILES:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_MIN_FILES);
      break;
  
    case ST_ERROR_PRINT:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_PRINT);
      break;

    case ST_ERROR_PRINT_NET:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_PRINT_NET);
      break;

    case ST_ERROR_INITIALIZE_NET:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INITIALIZE_NET);
      break;

    case ST_ERROR_NO_MEM:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_NO_MEM);
      break;
  
    case ST_ERROR_INSERT:
      break;

    case ST_ERROR_INVALID_IDENTIFIER:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_IDENTIFIER);
      break;

    case ST_ERROR_INVALID_OPTION:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_OPTION);
      break;

    /* Si se lo toma como error */  
    case ST_EOF:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_EOF);
      break;

    case ST_ERROR_INVALID_MSG_ID:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_MSG_ID);
      break;

    case ST_ERROR_INVALID_FRIENDS:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_FRIENDS);
      break;

    case ST_ERROR_INVALID_MESSAGE:
      fprintf(stderr,"%s: %s\n",MSG_ERROR_WORD,MSG_ERROR_INVALID_MESSAGE);
      break;

    case ST_OK:
    	break;
  
  }
}


/*----------------------------------------------------------------------------
FUNCION: Da indicaciones de donde se produjo un error al leer archivo.

ARGUMENTO: file_name 	Nombre del archivo.
----------------------------------------------------------------------------*/
void debug_file(const char*file_name)
{
	printf("%s\n\n",file_name);
}

/*----------------------------------------------------------------------------
FUNCION: Imprime uso del programa.
----------------------------------------------------------------------------*/
void help(void)
{
	puts(MSG_HELP);
}