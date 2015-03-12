 /*tlamijan*/

#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>
#include <openssl/pem.h>

#define BUFFER_SIZE 1024
#define KEYLEN 256
int main(int argc,const char * argv[]) {
  unsigned char st[BUFFER_SIZE];  // sifrovany text
  unsigned char key[KEYLEN];  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH];  // inicializacni vektor
  unsigned char readBuffer[BUFFER_SIZE];
  const char * infilename = argv[1];
  const char * f2out = argv[3];
  const char * keyfile = argv[2];
	if(argc != 4){
    fprintf(stderr, "Error shoud be: pemdec soubor_s_daty soubor_se_soukromym_klicem vystupni_soubor \n" );
    exit(1);
  }
  int stLength = 0;
  int tmpLength = 0;
  int readlen = 0;
  int keyLength = 0;
  EVP_CIPHER_CTX ctx;
  int nid = 0;

  FILE * fcyphedin = fopen(infilename,"rb");
  FILE * fplainout = fopen(f2out,"w+b");
  
  EVP_PKEY * privkey = NULL;
  FILE * fprivkey = fopen(keyfile,"rb");
  
  privkey = PEM_read_PrivateKey(fprivkey,NULL,NULL,NULL);
  fclose(fprivkey);

  fread(&nid,sizeof(int),1,fcyphedin);
  fread(iv,1,EVP_MAX_IV_LENGTH,fcyphedin);
  fread(&keyLength,sizeof(int),1,fcyphedin);
  fread(key,1,keyLength,fcyphedin);
  printf("NID: %d\n",nid);
  printf("Keylength: %d\n",keyLength);
  printf("%p\n",EVP_get_cipherbynid(nid));
  EVP_OpenInit(&ctx,
	       EVP_get_cipherbynid(nid),
         //EVP_des_cbc(),
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