#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>
#include <my_sys.h>
#include <ctype.h>
#include <gmp.h>
#include "paillier.h"
  

/* These must be right or mysqld will not find the symbol! */

// The normal function example
  
/* For creation/deleting use: 
   CREATE FUNCTION MyTest RETURNS INTEGER SONAME 'TestUDF.dll' and
  DROP MyTest 
  
  For testing something like:
  SELECT data1, MyTest(data1) FROM test.x ;*/





// The aggregate function example



my_bool SUM_HE_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void SUM_HE_deinit(UDF_INIT *initid);
char* SUM_HE(UDF_INIT *initid, UDF_ARGS *args, char* result, unsigned long *max_length, char *is_null, char *error);
void SUM_HE_clear(UDF_INIT *initid, char *is_null, char *error);
void SUM_HE_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
  /* For creation/deleting use: char* SUMHE(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error
  CREATE AGGREGATE FUNCTION MyTestAgg RETURNS INTEGER SONAME 'TestUDF.dll' and
  DROP MyTestAgg 
  
  For testing something like:
  SELECT MyTestAgg(data1), data2 FROM test.x  GROUP BY data2;*/
  
  
  /* AGGREGATE FUNCTION EXAMPLE */
typedef struct sum{
  paillier_pubkey_t* pubkey;
  paillier_ciphertext_t* totalsum;
} sum;

void destructor(sum *mysum){
  paillier_freepubkey(mysum->pubkey);
  paillier_freeciphertext(mysum->totalsum);
  free(mysum);
}
  
my_bool SUM_HE_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  // The most important thing to do here is setting up the memory you need...
  // Lets say we need a lonlong type variable to keep a checksum
  const char* hexkey = "8dfd1c1beb4a6d9357f8579e39ad9aa5";
  sum* mysum = (sum*)malloc(sizeof(sum));
  mysum->pubkey = paillier_pubkey_from_hex((char*)hexkey);
  mysum->totalsum = paillier_create_enc_zero();
  
  // store it as a char pointer in the pointer variable
  // Make sure that you don`t run in typecasting troubles later!!
  initid->ptr = (char*)mysum;  
  // check the arguments format
  if (args->arg_count != 1)
  {
    strcpy(message,"MyTestAgg() requires one arguments");
    return 1;
  }
  
  if (args->arg_type[0] != STRING_RESULT)
  {
    strcpy(message,"MyTestAgg() requires a string");
    return 1;
  }       
  return 0;    
  }
  
void SUM_HE_deinit(UDF_INIT *initid)
{
  sum* mysum = (sum*)initid->ptr;
  // Here you have to free the memory you allocated in the initialization function
  destructor(mysum);
}
  
char* SUM_HE(UDF_INIT *initid, UDF_ARGS *args, char* result, unsigned long *max_length, char *is_null, char *error)
{
  sum* mysum = (sum*)initid->ptr;
  char* my_str_enc_txt;
  my_str_enc_txt = mpz_get_str(NULL, 10, mysum->totalsum->c);
  int i = strlen(my_str_enc_txt);
  *max_length = (unsigned long)i;
  result = my_str_enc_txt;
  return result;
}

void SUM_HE_clear(UDF_INIT *initid, char *is_null, char *error)
{
  sum* mysum = (sum*)initid->ptr;
  /* The clear function resets the sum to 0 for each new group
     158  Of course you have to allocate a longlong variable in the init function and
     159  assign it to the pointer as seen above */
  paillier_freeciphertext(mysum->totalsum);
  mysum->totalsum = paillier_create_enc_zero();
  

}
  
void SUM_HE_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
  // For each row the current value is added to the sum
  
  
  
  
  sum* mysum = (sum*)initid->ptr;
  paillier_ciphertext_t* tmp;
  
  tmp = (paillier_ciphertext_t*) malloc(sizeof(paillier_ciphertext_t));
  mpz_init_set_str(tmp->c, (char *)args->args[0], 10);
  paillier_mul(mysum->pubkey , mysum->totalsum, mysum->totalsum, tmp);  
  paillier_freeciphertext( tmp );
  
}

