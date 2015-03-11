 /*tlamijan*/
#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
#include <stdio.h>

int main(void) {
 
  unsigned char ot[1024];  // open text
  unsigned char st[1024];  // sifrovany text
  unsigned char key[EVP_MAX_KEY_LENGTH] = "Super tajny klic";  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH] = "vector unknown";  // inicializacni vektor
  const char * filename = "Mad_scientist.bmp";
  const char * outfilename = "Mad_scientist_cbc.bmp";

  int otLength = 0;
  int stLength = 0;
  int tmpLength = 0;
  int readlen = 0;
  char header[14];
  unsigned int offset = 0;
 
  EVP_CIPHER_CTX ctx; // struktura pro kontext
 
	FILE * fin = fopen(filename,"rb");
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
	

 EVP_EncryptInit(&ctx, EVP_des_cbc(), key, iv);  // nastaveni kontextu pro sifrovani
  do{
	  readlen = fread(ot,1,1024,fin);
	  EVP_EncryptUpdate(&ctx,  st, &stLength, ot, readlen);  // sifrovani ot
	  fwrite(st,1,stLength,fout);
  }while(readlen == 1024);
  
  EVP_EncryptFinal(&ctx, &st[stLength], &tmpLength);  // ziskani sifrovaneho textu z kontextu
  fwrite(&st[stLength],1,tmpLength,fout);
  stLength += tmpLength;

  fclose(fin);
  fclose(fout);
  exit(0);
 }
