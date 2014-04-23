#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>
#include <openssl/pem.h>
 /*
tlamijan
*/
#define BUFFER_SIZE 1024
#define KEYLEN 256
int main(void) {
  unsigned char st[BUFFER_SIZE];  // sifrovany text
  unsigned char key[KEYLEN];  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH];  // inicializacni vektor
  unsigned char readBuffer[BUFFER_SIZE];
  const char * outfilename = "crypted.txt";
  const char * f2out = "plain2.txt";
  const char * keyfile = "key.txt";
	
  int stLength = 0;
  int tmpLength = 0;
  int readlen = 0;
  int keyLength = 0;
  EVP_CIPHER_CTX ctx;
  FILE * fkeyfile = fopen(keyfile,"rb");

  FILE * fcyphedin = fopen(outfilename,"rb");
  FILE * fplainout = fopen(f2out,"w+b");
  
  EVP_PKEY * privkey;
  FILE * fprivkey = fopen("privkey.pem","rb");
  
  privkey = PEM_read_PrivateKey(fprivkey,NULL,NULL,NULL);
  fclose(fprivkey);
  fread(iv,1,EVP_MAX_IV_LENGTH,fkeyfile);
  keyLength = fread(key,1,KEYLEN,fkeyfile);
  printf("Keylength: %d\n",keyLength);
  EVP_OpenInit(&ctx,
	       EVP_des_cbc(),
	       key,
	       keyLength,
	       iv,
	       privkey);
  
  //printf("Open init\n"); 

  while((readlen = fread(readBuffer,1,BUFFER_SIZE,fcyphedin)) != 0){
	  EVP_OpenUpdate(&ctx, st,&stLength, readBuffer, readlen);
	  fwrite(st,1,stLength,fplainout);
	 // printf("Open update\n");
  }
  EVP_OpenFinal(&ctx, st, &tmpLength);
  fwrite(st,1,tmpLength,fplainout);
  //printf("Open final\n");
  fclose(fcyphedin);
  fclose(fplainout);

}  