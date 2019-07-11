/* user.h */

#ifndef USER__H
#define USER__H

/*------------------------------ENCABEZADOS---------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "list.h"
#include "array.h"
#include "types.h"
#include "lang_setup.h"
#include "my_limits.h"
/*--------------------------------------------------------------------------*/

/*--------------------------------TIPOS-------------------------------------*/
/*-------------------------TIPO DE DATO ABSTRACTO---------------------------*/
typedef struct
{
char* username;
char* name;
int id;
array_s* friends;
list_s messages;
}user_s;

typedef struct
{
int msg_id;
time_t timestamp;
int usr_id;
char* message;
}message_s;

/* Inicialización estática */
static const user_s empty_user;
/*--------------------------------------------------------------------------*/

/*------------------------------PROTOTIPOS---------------------------------*/
status_t get_user(FILE* infile, user_s** );
status_t get_line( char** s, FILE* f);
status_t get_messages(const char* string, list_s* msg_list);
status_t get_date(char **fields_array, message_s *msg);
array_s* get_friends(const char* string);
status_t print_user(const user_s* user,FILE* outfile);
status_t print_message(message_s* msg, FILE* outfile);
void save_user(user_s* user, status_t* st);
void destroy_user(user_s* target);
void destroy_messages(message_s* target);
int compare_user(const user_s* target ,const user_s* user);
status_t split(const char* cadena, char delimitador, char*** arreglo, size_t **l);
char* strdup(const char*sc);
status_t destroy_array_from_string(char*** campos,size_t i);
/*--------------------------------------------------------------------------*/

#endif
