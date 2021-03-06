#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "paillier.h"

void main(void){
  int i =0;
  int j =0;
  int bits = 128;
  paillier_prvkey_t* prvkey;
  paillier_pubkey_t* pubkey;
  char* my_str = "cookies";
  char* my_str2;
  char* my_pubkey;
  char* my_prvkey;
  paillier_plaintext_t* my_plain_txt;
  paillier_plaintext_t* my_plain_txt2;
  paillier_ciphertext_t* my_enc_txt;
  void* my_enc_export[128];
  my_plain_txt = paillier_plaintext_from_str(my_str);
  paillier_keygen( bits, &pubkey, &prvkey, &paillier_get_rand_devurandom);
  my_enc_txt = paillier_enc(NULL, pubkey, my_plain_txt, &paillier_get_rand_devurandom);
  my_plain_txt2 = paillier_dec(NULL, pubkey, prvkey, my_enc_txt);
  my_str2 = paillier_plaintext_to_str(my_plain_txt2);
  my_pubkey = paillier_pubkey_to_hex( pubkey );
  my_prvkey = paillier_prvkey_to_hex( prvkey );
  printf("my pub key is : %s\n", my_pubkey);
  printf("my prv key is : %s\n", my_prvkey);
  return;
}
