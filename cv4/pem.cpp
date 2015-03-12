/*tlamijan*/
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>
#include <openssl/pem.h>

#define BUFFER_SIZE 1024

void writeData(FILE * file,void * data,int len){
  fwrite(data,1,len,file);
}


int main(int argc, char * argv[]) {
  unsigned char st[BUFFER_SIZE];  // sifrovany text
  unsigned char * key;  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH];  // inicializacni vektor
  unsigned char readBuffer[BUFFER_SIZE];
  const char * filename = argv[1];
  const char * outfilename = argv[3];
  const char * keyfile = argv[2];
	if(argc != 4){
    fprintf(stderr, "Error shoud be: pem soubor_s_daty soubor_s_verejnym_klicem vystupni_soubor \n" );
    exit(1);
  }
  int stLength = 0;
  int tmpLength = 0;
  int readlen = 0;
  int keyLength = 0;
  FILE * fplainin = fopen(filename,"rb");
  FILE * fcyphedout = fopen(outfilename,"w+b");
  EVP_PKEY * pubkey;
  EVP_CIPHER_CTX ctx;
  FILE * fpubkey = fopen(keyfile,"rb");
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
  /*
  printf("Key length: %d\n",keyLength);
  for(int i=0 ; i < keyLength ; i++){
    printf("%x",key[i]);
  }
   printf("\n");
   */
 // printf("Seal Init\n");
  int nid = EVP_CIPHER_CTX_nid(&ctx);
  fwrite(&nid,sizeof(nid),1,fcyphedout);
  writeData(fcyphedout,(void*)iv,EVP_MAX_IV_LENGTH);
  fwrite(&keyLength,sizeof(keyLength),1,fcyphedout);
  writeData(fcyphedout,(void*)key,keyLength);
  printf("NID: %d\n",nid);
  
  while((readlen =fread(readBuffer,1,BUFFER_SIZE,fplainin))!=0){
	  EVP_SealUpdate(&ctx, st,&stLength, readBuffer, readlen);
	  fwrite(st,1,stLength,fcyphedout);
	 // printf("Seal update\n");
  }
  EVP_SealFinal(&ctx, st, &tmpLength);
  //printf("Seal final\n");
  fwrite(st,1,tmpLength,fcyphedout);
  fclose(fplainin);
  fclose(fcyphedout);
  
  /*
	Encrypting end
  */
  //EVP_CIPHER_CTX ctx;
 
  free(key);
 }
