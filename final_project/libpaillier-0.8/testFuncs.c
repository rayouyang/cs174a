#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include "paillier.h"
#include "paillier.c"

int main(){
  int i =0;
  int j =0;
  paillier_prvkey_t** prvkey;
  paillier_pubkey_t** pubkey;
  pailler_get_rand_t get_rand;
  int modulusbits = 128;
  void* mybuf;
  paillier_ciphertext_t* myres;
  paillier_keygen(modulusbits, pubkey, prvkey, paillier_get_rand_devurandom( mybuf, 100));
}
