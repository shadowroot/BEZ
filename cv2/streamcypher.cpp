#include <stdlib.h>
#include <openssl/evp.h>
#include <string.h>
#include <fstream>
 
using namespace std;

int main(void) {
	const int buffLen = 1024;
  unsigned char known[1024] = "abcdefghijklmnopqrstuvwxyz0123";
  unsigned char knownst[1024];
  unsigned char ot[1024] = "Supertajna zprava z BEZ supert";  // open text
  unsigned char st[1024];  // sifrovany text
  unsigned char key[EVP_MAX_KEY_LENGTH] = "SuperTajnyKlic";  // klic pro sifrovani
  unsigned char iv[EVP_MAX_IV_LENGTH] = "SuperTajneIV";  // inicializacni vektor

  ofstream outFile;
  ifstream inFile;


  unsigned char coleagueot[1024];
 
  int otLength = strlen((const char*) ot);
  int stLength = 0;
  int tmpLength = 0;
  int knownstlen = 0;
 
  EVP_CIPHER_CTX ctx; // struktura pro kontext
 
  

  printf("OT: %s\n", ot);
 
  /* Sifrovani */
  EVP_EncryptInit(&ctx, EVP_rc4(), key, iv);  // nastaveni kontextu pro sifrovani
  EVP_EncryptUpdate(&ctx,  st, &stLength, ot, otLength);  // sifrovani ot
  EVP_EncryptFinal(&ctx, &st[stLength], &tmpLength);  // ziskani sifrovaneho textu z kontextu
 
  printf ("Zasifrovano %d znaku.\nMY_ST:", stLength);
  for(int i=0; i < stLength ; i++){
   printf ("%02x", st[i]);
  }
  outFile.open("mycyphed.bin");
  outFile.write((char *)st,stLength);
  outFile.close();
  puts("\n");
  
  
  EVP_EncryptInit(&ctx, EVP_rc4(), key, iv);  // nastaveni kontextu pro sifrovani
  EVP_EncryptUpdate(&ctx,  knownst, &knownstlen, known, (int)strlen((const char *)known));  // sifrovani ot
  EVP_EncryptFinal(&ctx, &knownst[knownstlen], &tmpLength);  // ziskani sifrovaneho textu z kontextu
  printf("KNOWN:%s\n", known);
  printf ("Zasifrovano %d znaku.\nKNOWNST:", knownstlen);
  for(int i=0; i < knownstlen ; i++){
   printf ("%02x", knownst[i]);
  }
  outFile.open("myknown.bin");
  outFile.write((char*)knownst,knownstlen);
  outFile.close();
  
  outFile.open("known.txt");
  outFile << known;
  outFile.close();
  printf("\n");

  inFile.open("known-edux.bin");
	inFile.read((char*)knownst,buffLen);
	inFile.close();

  inFile.open("cyphed-edux.bin");
  inFile.read((char*)st, buffLen);
  inFile.close();

  for(int i=0 ; i < buffLen; i++){
	coleagueot[i] = (st[i] ^ knownst[i] ^ known[i]);
  }
	
  printf("OT:%s\n", coleagueot);
  // /* Desifrovani */
  // EVP_DecryptInit(&ctx, EVP_rc4(), key, iv);  // nastaveni kontextu pro desifrovani
  // EVP_DecryptUpdate(&ctx, ot, &otLength,  st, stLength);  // desifrovani st
  // EVP_DecryptFinal(&ctx, &ot[otLength], &tmpLength);  // ziskani desifrovaneho textu z kontextu
  
  // /* Vypsani zasifrovaneho a rozsifrovaneho textu. */
  // printf("DT: %s\n", ot);
 
  return 0;
 }
