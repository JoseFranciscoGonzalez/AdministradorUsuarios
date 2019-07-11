/* lang_eng.h */

#ifndef LANG_ENG__H
#define LANG_ENG__H


/*-------------------------CONFIG FILE SINTAX---------------------------*/
#define ID_OPTION "id"
#define NAME_OPTION "name"
#define FRIENDS_OPTION "friends"
#define MESSAGE_OPTION	"messages"

/*-----------------------------------------------------------------------*/


/*-----------------------------DELIMITERS-------------------------------*/
#define COMMENT_CHAR '#'
#define LEFT_SECTION_CHAR '['
#define RIGHT_SECTION_CHAR ']'
#define MSG_DELIMITER '/'
#define PRINT_MSG_FORMAT "%s/"
/*-----------------------------------------------------------------------*/


/*-------------------------------TAGS CLA-------------------------------*/
#define CLA_ELIMINATE_TAG "--eliminate"
#define CLA_ELIMINATE_TAG_SHORT "-e"
#define CLA_ELIMINATE_ID_OPTION "i:"
#define CLA_ELIMINATE_USER_OPTION "u:"
#define CLA_OUTPUT_TAG "--output"
#define CLA_OUTPUT_TAG_SHORT "-o"
#define CLA_OUTPUT_SINGLE_OPTION "single"
#define CLA_OUTPUT_MULTI_OPTION "multi"
/*-----------------------------------------------------------------------*/


/*---------------------------------ERRORS--------------------------------*/
#define MSG_ERROR_WORD "Error"

#define MSG_ERROR_NULL_POINTER "Null pointer detected"
#define MSG_ERROR_READ "Can't read from file"
#define MSG_ERROR_GET_USER "Can't get user from file"
#define MSG_ERROR_ADD_USER "Can't add user to net"
#define MSG_ERROR_OPEN_FILE "Can't open file"
#define MSG_ERROR_CREATE_FILE "Can't create file"
#define MSG_ERROR_PRINT "Can't print user"
#define MSG_ERROR_PRINT_NET "Can't print net"
#define MSG_ERROR_INITIALIZE_NET "Can't create net"
#define MSG_ERROR_NO_MEM "There's no available memory"
#define MSG_ERROR_INVALID_IDENTIFIER "Invalid identifier."
#define MSG_ERROR_INVALID_OPTION "Invalid option"
#define MSG_ERROR_EOF "The input flow has been closed"
#define MSG_ERROR_INVALID_MSG_ID "Invalid msg id" 


#define MSG_ERROR_INVALID_DATE "Invalid date"
#define MSG_ERROR_INVALID_DATE_RANGE "Date out of range"
#define MSG_ERROR_INVALID_ARGUMENTS "Invalid execution arguments"
#define MSG_ERROR_CLA_MIN_ARGUMENTS "Missing CLA argument"
#define MSG_ERROR_INVALID_CLA_ID_OPTION "Invalid eliminate specifier"
#define MSG_ERROR_INVALID_CLA_USER_OPTION "Invalid eliminate specifier"
#define MSG_ERROR_INVALID_CLA_OPTION "Invalid CLA option"
#define MSG_ERROR_INVALID_CLA_OUTPUT_OPTION "Invalid output option"
#define MSG_ERROR_INVALID_ID "Invalid Id"
#define MSG_ERROR_INVALID_USERNAME "Invalid Username"
#define MSG_ERROR_MIN_FILES "No input files"
#define MSG_ERROR_INVALID_FRIENDS "Invalid friends"
#define MSG_ERROR_INVALID_MESSAGE "Invalid message"
/*-----------------------------------------------------------------------*/

/*-----------------------------MSG HELP----------------------------------*/
#define MSG_HELP "USE: ./social_network --eliminate <filter> --output <fmt> [..files..]\n\n--eliminate -e ,eliminates an user.\n<filter>\n\ti:[id] u:[username] ,indicates target\n\n--output -o  ,output mode\n<fmt>\n\tsingle ,screen output\n\tmulti ,single file output for each user\n\nfiles  ,config files at least one\n"
/*-----------------------------------------------------------------------*/

/*------------------------------USR MSG----------------------------------*/
#define MSG_SOCIAL_NETWORK_WELCOME "#############Social Network##############\nAlgoritmos y programación I (95.11)\nTrabajo Práctico Final\nTema: Red Social\nFecha:22-11-17\nIntegrantes: González, Pinto, Mion\n#########################################\n"
#define MSG_LOADED_USERS "Loaded users"
#define MSG_OUTPUT_FILES "Output files"
#define MSG_EMPTY_NET "Empty net, no loaded users"
/*-----------------------------------------------------------------------*/

#endif
