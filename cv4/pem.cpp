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
  unsigned char * final;
	
  int stLength = 0;
  int tmpLength = 0;
  int readlen = 0;
  char header[14];
  unsigned int offset = 0;
  int keyLength = 0;
  FILE * fplainin = fopen(filename,"rb");
  FILE * fcyphedout = fopen(outfilename,"w+b");

  EVP_PKEY * pubkey;
  EVP_CIPHER_CTX ctx; // struktura pro kontext
  FILE * fpubkey = fopen("pubkey.pem","rb");
  pubkey = PEM_read_PUBKEY(fpubkey, NULL, NULL, NULL); //No password protection of the key itself
  fclose(fpubkey);
	

/*
 
	
	FILE * fout = fopen(outfilename,"w+b");
	if(!fin){
		printf("File not found");
	}

	fread(header,1,14,fin);
	fwrite(header,1,14,fout);
	offset = (unsigned int)*(&header[10]);
	offset -= 14;

	while(offset > 1024){
		fread(ot,1,1024,fin);
		fwrite(ot,1,1024,fout);
		offset -= 1024;
	}
	fread(ot,1,offset,fin);
	fwrite(ot,1,offset,fout);
	EVP_PKEY_encrypt(EVP_PKEY_CTX *ctx,
                               unsigned char *out, size_t *outlen,
                               const unsigned char *in, size_t inlen);
*/
	
	EVP_SealInit(&ctx, EVP_des_cbc(),
                         &key, &keyLength, iv,
                         &pubkey, 0);
	while((readlen = fread(readBuffer,1,BUFFER_SIZE,fplainin)) != -1){
		EVP_SealUpdate(&ctx, st,&stLength, readBuffer, readlen);
		fwrite(st,1,stLength,fcyphedout);
	}
	EVP_SealFinal(&ctx, st, &tmpLength);
	fwrite(st,1,tmpLength,fcyphedout);
	fclose(fcyphedout);
	fclose(fplainin);
  /*
	Encrypting end
  */
  EVP_EncryptFinal(&ctx, &st[stLength], &tmpLength);  // ziskani sifrovaneho textu z kontextu
  fwrite(&st[stLength],1,tmpLength,fcyphedout);
  stLength += tmpLength;

	  FILE * fcyphedin = fopen(outfilename,"rb");
 	 FILE * fplainout = fopen(f2out,"w+b");
	EVP_PKEY * privkey;
	FILE * fprivkey = fopen("privkey.pem","rb");
	PEM_read_PrivateKey(fprivkey,NULL,NULL,NULL);
	EVP_OpenInit(&ctx,EVP_des_cbc(),keydec,keyLength,iv,privkey);

	while((readlen = fread(readBuffer,1,BUFFER_SIZE,fcyphedin)) != -1){
		EVP_SealUpdate(&ctx, st,&stLength, readBuffer, readlen);
		fwrite(st,1,stLength,fplainout);
	}
	EVP_SealFinal(&ctx, st, &tmpLength);
	fwrite(st,1,tmpLength,fplainout);
	fclose(fcyphedin);
	fclose(fplainout);

 }
