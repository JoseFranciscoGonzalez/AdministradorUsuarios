/* net.h */

#ifndef NET__H
#define NET__H

/*------------------------------ENCABEZADOS---------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "list.h"
#include "user.h"
#include "types.h"
#include "my_limits.h"
#include "lang_setup.h"
/*--------------------------------------------------------------------------*/

/*--------------------------------TIPOS-------------------------------------*/
typedef enum
{
SINGLE,
MULTI
}output_t;
/*--------------------------------------------------------------------------*/

/*------------------------------PROTOTIPOS---------------------------------*/
status_t validate_args(int argc, char* argv[],user_s** target_user, output_t* output, size_t* n_files);
status_t load_net(size_t n_files, int argc, char** argv, list_s* net, user_s* target);
status_t print_net(list_s* net, output_t mode);
status_t open_config_file(const char* file_name, FILE** config_file);
status_t add_users_to_list(FILE* infile, list_s* list, size_t* list_size, const user_s* target);
void print_error(status_t st);
void debug_file(const char*file_name);
void help(void);
/*--------------------------------------------------------------------------*/

#endif
