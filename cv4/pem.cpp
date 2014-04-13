#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>
#include <openssl/pem.h>
 /*
tlamijan
*/
#define BUFFER_SIZE 1024
int main(void) {
  unsigned char st[BUFFER_SIZE];  // sifrovany text
  unsigned char * key;  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH];  // inicializacni vektor
  unsigned char readBuffer[BUFFER_SIZE];
  const char * filename = "plain.txt";
  const char * outfilename = "crypted.txt";
  const char * f2out = "plain2.txt";
  unsigned char * keydec;
	
  int stLength = 0;
  int tmpLength = 0;
  int readlen = 0;
  int keyLength = 0;
  FILE * fplainin = fopen(filename,"rb");
  FILE * fcyphedout = fopen(outfilename,"w+b");

  EVP_PKEY * pubkey;
  EVP_CIPHER_CTX ctx; // struktura pro kontext
  FILE * fpubkey = fopen("pubkey.pem","rb");
  pubkey = PEM_read_PUBKEY(fpubkey, NULL, NULL, NULL); //No password protection of the key itself
  fclose(fpubkey);
 // printf("Reading pubkey\n");
  keyLength = EVP_PKEY_size(pubkey);
  key = (unsigned char*) malloc(EVP_PKEY_size(pubkey));
	
  EVP_SealInit(&ctx, EVP_des_cbc(),
		    &key, &keyLength, iv,
		    &pubkey, 0);
 // printf("Seal Init\n");
  readlen = BUFFER_SIZE;
  while( readlen == BUFFER_SIZE){
          readlen = fread(readBuffer,1,BUFFER_SIZE,fplainin);
	  EVP_SealUpdate(&ctx, st,&stLength, readBuffer, readlen);
	  fwrite(st,1,stLength,fcyphedout);
	 // printf("Seal update\n");
  }
  EVP_SealFinal(&ctx, st, &tmpLength);
  //printf("Seal final\n");
  fwrite(st,1,tmpLength,fcyphedout);
  fclose(fcyphedout);
  fclose(fplainin);
  /*
	Encrypting end
  */
  EVP_CIPHER_CTX ctx2; // struktura pro kontext
  FILE * fcyphedin = fopen(outfilename,"rb");
  FILE * fplainout = fopen(f2out,"w+b");
  
  EVP_PKEY * privkey;
  FILE * fprivkey = fopen("privkey.pem","rb");
  
  privkey = PEM_read_PrivateKey(fprivkey,NULL,NULL,NULL);
  fclose(fprivkey);
 
  keyLength = EVP_PKEY_size(privkey);
  keydec = (unsigned char*) malloc(EVP_PKEY_size(privkey));
  
  EVP_OpenInit(&ctx2,
	       EVP_des_cbc(),
	       keydec,
	       keyLength,
	       iv,
	       privkey);
  
  //printf("Open init\n");
  readlen = BUFFER_SIZE;
  while(readlen == BUFFER_SIZE){
	  readlen = fread(readBuffer,1,BUFFER_SIZE,fcyphedin);
	  EVP_OpenUpdate(&ctx2, st,&stLength, readBuffer, readlen);
	  fwrite(st,1,stLength,fplainout);
	 // printf("Open update\n");
  }
  EVP_OpenFinal(&ctx2, st, &tmpLength);
  fwrite(st,1,tmpLength,fplainout);
  //printf("Open final\n");
  fclose(fcyphedin);
  fclose(fplainout);
  free(key);
  free(keydec);
 }
