#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "paillier.h"




typedef struct sum{
  paillier_pubkey_t* pubkey;
  paillier_ciphertext_t* totalsum;
  } sum;

void main(void){
  int i =0;
  int j =0;
  int bits = 128;
  paillier_prvkey_t* prvkey;
  paillier_pubkey_t* pubkey;
  unsigned long int my_str = 85000;
  char* my_test_mul = "10000";
  char* my_str2;
  char* my_pubkey;
  char* my_prvkey;
  char* my_str_enc_txt;
  paillier_plaintext_t* my_plain_txt;
  paillier_plaintext_t* my_test_mul_plain_txt;
  paillier_plaintext_t* my_plain_txt2;
  paillier_ciphertext_t* my_enc_txt;
  paillier_ciphertext_t* my_enc_txt1;
  paillier_ciphertext_t* my_mul_answer;
  paillier_ciphertext_t  *my_post_enc_txt;
  my_post_enc_txt = paillier_create_enc_zero();
  
  //  my_mul_answer = (paillier_ciphertext_t*) malloc(sizeof(paillier_ciphertext_t));
  //mpz_init(my_mul_answer->c);
  void* my_enc_export[128];
  mpz_t my_post_mpz_t_enc_txt;
  //using public hex key to create a string value of the encrypted salary  
  char* hexkey = "8dfd1c1beb4a6d9357f8579e39ad9aa5";
  char* hexprvkey = "46fe8e0df5a536c8ed0acb3b68850816";
  sum* mysum = (sum*)malloc(sizeof(sum));

  mysum->pubkey = paillier_pubkey_from_hex(hexkey);
  mysum->totalsum = (paillier_ciphertext_t*) malloc(sizeof(paillier_ciphertext_t));
  mpz_init(mysum->totalsum->c);
  


  my_plain_txt = paillier_plaintext_from_ui(my_str);
  //my_test_mul_plain_txt = paillier_plaintext_from_str(my_test_mul);
  //paillier_keygen( bits, &pubkey, &prvkey, &paillier_get_rand_devurandom);
  prvkey = paillier_prvkey_from_hex( hexprvkey ,mysum->pubkey);
  my_enc_txt = paillier_enc(NULL, mysum->pubkey, my_plain_txt, paillier_get_rand_devurandom);
  //my_enc_txt1 = paillier_enc(NULL, pubkey, my_test_mul_plain_txt, paillier_get_rand_devurandom);
  //paillier_mul(pubkey, my_mul_answer, my_enc_txt, my_enc_txt1);
  my_str_enc_txt = mpz_get_str(NULL, 10, my_enc_txt->c);
  printf("my cipher txt as a integer of type char* is: %s\n", my_str_enc_txt);
  my_str_enc_txt = "4319101461640545393304994210137115165469707553647123187965056493696177529423";
  
  mpz_init_set_str(my_post_enc_txt->c, my_str_enc_txt,10); 
  my_plain_txt2 = paillier_dec(NULL, mysum->pubkey, prvkey, my_post_enc_txt);
  my_str2 = paillier_plaintext_to_str(my_plain_txt2);
  
  my_pubkey = paillier_pubkey_to_hex( mysum->pubkey );
  my_prvkey = paillier_prvkey_to_hex( prvkey );
  printf("my pub key is : %s\n", my_pubkey);
  printf("my prv key is : %s\n", my_prvkey);
  printf("my post decripted string of the cipher text which was translated from mpz_t to char* back to mpz_t into a new paillier_cipher_text* is: %s\n", my_str2);
  return;
}
