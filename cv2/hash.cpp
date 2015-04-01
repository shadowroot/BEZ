/*tlamijan*/
#include <iostream>
#include <openssl/evp.h>
#include <string>

using namespace std;


void genhash(string& str){
	for(int i=str.length()-1 ; i >= 0 ; i--){
		if(str[i] <= 'z'){
			str[i]++;
			return;
		}
		else{
			str[i] = '0';
		}
	}
	str += "0";
}


int main(){
	std::string text = "0";
	const char * hashFn = "sha256";

	EVP_MD_CTX ctx;
	const EVP_MD *type;
	unsigned char hash[EVP_MAX_MD_SIZE]; // char pole pro hash - 64 bytu (max pro sha 512)
	int length;  // vysledna delka hashe
	 
	  /* Inicializace OpenSSL hash funkci */
	  OpenSSL_add_all_digests();
	  /* Zjisteni, jaka hashovaci funkce ma byt pouzita */
	  type = EVP_get_digestbyname(hashFn);
	 
	  /* Pokud predchozi prirazeni vratilo -1, tak nebyla zadana spravne hashovaci funkce */
	  if(!type) {
	    printf("Sifra %s neexistuje.\n", hashFn);
	    exit(1);
	  }
	 
	  while(hash[0] != 0xAA || hash[1] != 0xBB){
	  	genhash(text);
	  	EVP_DigestInit(&ctx, type);
	  	EVP_DigestUpdate(&ctx, text.c_str(), text.size());  // zahashuje text a ulozi do kontextu
	  	EVP_DigestFinal(&ctx, hash, (unsigned int *) &length);  // zjiskani hashe z kontextu
	  	//printf("Hash textu \"%s\" je: \n", text.c_str());
	  }
	 
	  /* Vypsani vysledneho hashe */
	  printf("Hash textu \"%s\" je: \n", text.c_str());
	  for(int i = 0; i < length; i++){
	    printf("%02x", hash[i]);
	  }
	  printf("\n");


	return 0;
}
