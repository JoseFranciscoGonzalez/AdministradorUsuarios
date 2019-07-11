/* my_limits.h */

#ifndef MY_LIMITS__H
#define MY_LIMITS__H

/*---------------------ORDEN DEL IDENTIFICADOR MESSAGE----------------------*/
/*---------------message = msg_id/AAAA-MM-DD/usr_id/message-----------------*/
#define MSG_ID_POSITION 0	
#define MSG_USER_POSITION 2	
#define MSG_DATE_POSITION 1	
#define MSG_STRING_POSITION 3
/*--------------------------------------------------------------------------*/


/*-----------------------POSICIONES DEL TIMESTAMP---------------------------*/
/*------------------------------AAAA-MM-DD----------------------------------*/
#define DATE_DELIMITER '-'
#define YEAR_POSITION 0
#define MONTH_POSITION 1
#define DAY_POSITION 2
/*--------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
#define CLA_MIN_ARGUMENTS 6
#define FILE_READ_MODE "rt"
#define FILE_CREATE_MODE "wt"
#define TXT_FILE_EXTENSION ".txt"
#define SHORT_STRING 20
#define DECIMAL_BASE 10
#define CHOP_SIZE 25
/*--------------------------------------------------------------------------*/


#endif
