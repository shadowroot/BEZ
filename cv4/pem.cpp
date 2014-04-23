#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>
#include <openssl/pem.h>
 /*
tlamijan
*/
#define BUFFER_SIZE 1024

int readData(FILE * file,void * buffer, int len){
  if(fread(buffer,1,len,file) < (unsigned)len){
    return 1;
  }
  return 0;
}

void writeData(FILE * file,void * data,int len){
  fwrite(data,1,len,file);
}

void closeFile(FILE * file){
  fclose(file);
}

int main(void) {
  unsigned char st[BUFFER_SIZE];  // sifrovany text
  unsigned char * key;  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH];  // inicializacni vektor
  unsigned char readBuffer[BUFFER_SIZE];
  const char * filename = "plain.txt";
  const char * outfilename = "crypted.txt";
  const char * keyfile = "key.txt";
	
  int stLength = 0;
  int tmpLength = 0;
  int readlen = 0;
  int keyLength = 0;
  FILE * fplainin = fopen(filename,"rb");
  FILE * fcyphedout = fopen(outfilename,"w+b");

  EVP_PKEY * pubkey;
  EVP_CIPHER_CTX ctx;
  FILE * fpubkey = fopen("pubkey.pem","rb");
  pubkey = PEM_read_PUBKEY(fpubkey, NULL, NULL, NULL); //No password protection of the key itself
  fclose(fpubkey);
 // printf("Reading pubkey\n");
  //EVP_PKEY_CTX * ctx = EVP_PKEY_CTX_new(pubkey,NULL);
  keyLength = EVP_PKEY_size(pubkey);
  key = (unsigned char*) malloc(keyLength);
	
  EVP_SealInit(&ctx,
	EVP_des_cbc(),
	&key, &keyLength, iv,
	&pubkey,
	1);
  FILE * fkeyfile = fopen(keyfile,"w+b");
  /*
  printf("Key length: %d\n",keyLength);
  for(int i=0 ; i < keyLength ; i++){
    printf("%x",key[i]);
  }
   printf("\n");
   */
 // printf("Seal Init\n");
  writeData(fkeyfile,(void*)iv,EVP_MAX_IV_LENGTH);
  writeData(fkeyfile,(void*)key,keyLength);
  printf("Keylength: %d\n",keyLength);
  fclose(fkeyfile);
  while((readlen =fread(readBuffer,1,BUFFER_SIZE,fplainin))!=0){
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
  //EVP_CIPHER_CTX ctx;
 
  free(key);
 }
