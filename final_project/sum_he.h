

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "paillier.h"
//#include <my_global.h>
//#include <my_sys.h>
//#include <m_string.h>// To get strmov()
//#endif
//#include <mysql.h>
//#include <ctype.h>

static pthread_mutex_t LOCK_hostname;

#ifdef HAVE_DLOPEN

/* These must be right or mysqld will not find the symbol! */

extern "C" my_bool MyTest_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
extern "C" void MyTest_deinit(UDF_INIT *initid);
extern "C" longlong MyTest(UDF_INIT *initid, UDF_ARGS *args,char *is_null, char *error);


#define MAXMETAPH 8

my_bool MyTest_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  /*if (args->arg_count != 1 || args->arg_type[0] != STRING_RESULT)
  {
    strcpy(message,"Wrong arguments to metaphon;  Use the source");
    return 1;
  }
  initid->max_length=MAXMETAPH;*/
  return 0;
}

/****************************************************************************
 ** Deinit function. This should free all resources allocated by
 ** this function.
 ** Arguments:
 ** initidReturn value from xxxx_init
 ****************************************************************************/


void MyTest_deinit(UDF_INIT *initid)
{
}

/***************************************************************************
 ** UDF string function.
 ** Arguments:
 ** initidStructure filled by xxx_init
 ** argsThe same structure as to xxx_init. This structure
 **contains values for all parameters.
 **Note that the functions MUST check and convert all
 **to the type it wants!  Null values are represented by
 **a NULL pointer
 ** resultPossible buffer to save result. At least 255 byte long.
 ** lengthPointer to length of the above buffer.In this the function
 **should save the result length
 ** is_nullIf the result is null, one should store 1 here.
 ** errorIf something goes fatally wrong one should store 1 here.
 **
 ** This function should return a pointer to the result string.
 ** Normally this is 'result' but may also be an alloced string.
 ***************************************************************************/


longlong MyTest(UDF_INIT *initid, UDF_ARGS *args,char *is_null, char *error)
{
  return 0;
}

#endif /* HAVE_DLOPEN */
