#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include <ctype.h>
#include "paillier.h"
  

/* These must be right or mysqld will not find the symbol! */

// The normal function example
  
/* For creation/deleting use: 
   CREATE FUNCTION MyTest RETURNS INTEGER SONAME 'TestUDF.dll' and
  DROP MyTest 
  
  For testing something like:
  SELECT data1, MyTest(data1) FROM test.x ;*/





// The aggregate function example

extern "C" my_bool SUMHE_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
extern "C" void SUMHE_deinit(UDF_INIT *initid);
extern "C" longlong SUMHE_Agg(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
extern "C" void SUMHE_clear(UDF_INIT *initid, char *is_null, char *error);
extern "C" void SUMHE_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
  
  /* For creation/deleting use: 
  CREATE AGGREGATE FUNCTION MyTestAgg RETURNS INTEGER SONAME 'TestUDF.dll' and
  DROP MyTestAgg 
  
  For testing something like:
  SELECT MyTestAgg(data1), data2 FROM test.x  GROUP BY data2;*/
  
  
  /* AGGREGATE FUNCTION EXAMPLE */
struct sum{
  paillier_pubkey_t* pubkey;
  paillier_ciphertext_t* totalsum;
};

void destructor(sum *mysum){
  paillier_freepubkey(pubkey);
  paillier_freeciphertext(totalsum);
  free(mysum);
}
  
my_bool SUMHE_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  // The most important thing to do here is setting up the memory you need...
  // Lets say we need a lonlong type variable to keep a checksum
  char* hexkey = "8dfd1c1beb4a6d9357f8579e39ad9aa5";
  sum* mysum = calloc(1, sizeof(sum));
  mysum->pubkey = paillier_pubkey_from_hex(hexkey);
  mysum->totalsum = (paillier_ciphertext_t*) malloc(sizeof(paillier_ciphertext_t));
  mpz_init(mysum->totalsum->c);
  // store it as a char pointer in the pointer variable
  // Make sure that you don`t run in typecasting troubles later!!
  initid->ptr = (char*)mysum;
  
  // check the arguments format
  if (args->arg_count != 1)
  {
    strcpy(message,"MyTestAgg() requires one arguments");
    return 1;
  }
  
  if (args->arg_type[0] != STRING_RESEULT)
  {
    strcpy(message,"MyTestAgg() requires a string");
    return 1;
  }       
  return 0;    
  }
  
void SUMHE_deinit(UDF_INIT *initid)
{
  // Here you have to free the memory you allocated in the initialization function
  destructor((sum*)intid->ptr);
}
  
char* SUMHE(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
  char* my_str_enc_txt;
  my_str_enc_txt = mpz_get_str(NULL, 10, ((sum*)initid->ptr)->totalsum->c);
  return my_str_enc_txt;
}

void SUMHE_clear(UDF_INIT *initid, char *is_null, char *error)
{
  /* The clear function resets the sum to 0 for each new group
     158  Of course you have to allocate a longlong variable in the init function and
     159  assign it to the pointer as seen above */
  paillier_freeciphertext(((sum*)initid->ptr)->totalsum);
  ((sum*)initid->ptr)->totalsum = (paillier_ciphertext_t*) malloc(sizeof(paillier_ciphertext_t));
  mpz_init(((sum*)initid->ptr)->totalsum->c);

}
  
void SUMHE_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
  // For each row the current value is added to the sum
  int i;
  mpz_t my_post_mpz_t_enc_txt;
  char* my_str_enc_txt = (char*)args->args[0];
  paillier_ciphertext_t* my_post_enc_txt;
  my_post_enc_txt = (paillier_ciphertext_t*) malloc(sizeof(paillier_ciphertext_t));
  mpz_init(my_post_enc_txt->c);
  i = mpz_init_set_str(my_post_mpz_t_enc_txt, my_str_enc_txt, 10);
  mpz_set(my_post_enc_txt->c, my_post_mpz_t_enc_txt);
  paillier_mul( ((sum*)initid->ptr)->pubkey , ((sum*)initid->ptr)->totalsum, ((sum*)initid->ptr)->totalsum, my_post_enc_txt);
  paillier_freeciphertext( my_post_enc_txt );
  mpz_clear( my_post_mpz_t_enc_txt );
}

