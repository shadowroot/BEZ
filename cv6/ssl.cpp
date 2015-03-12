 /*tlamijan*/
#include <stdlib.h>
#include <openssl/ssl.h>
#include <string.h>
#include <stdio.h>
#include <openssl/pem.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int main(void) {
  
  char buffer[BUFFER_SIZE];
  int sockfd, portno=443,readsize;
  struct sockaddr_in serv_addr;
  struct hostent *server = NULL;
  server = gethostbyname("fit.cvut.cz");
  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  FILE * fweb = fopen("odkazy.html","w+b");
  const char * request = "GET /student/odkazy HTTP/1.0\r\nLocation: www.fit.cvut.cz\r\n\r\n";

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
    (char *)&serv_addr.sin_addr.s_addr,
	  server->h_length);
  serv_addr.sin_port = htons(portno);

  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
    printf("ERROR connecting\n");
	  return 1;
  }
  printf("Connected\n");

/*
send(sockfd, (void*)request, strlen(request),0);
while((readsize = recv(sockfd,(void*)buffer,BUFFER_SIZE,0)) != 0){
	fwrite(buffer,1,readsize,fweb);
}
*/
  
  SSL_library_init();
  SSL_CTX * ctx = SSL_CTX_new(SSLv23_client_method());
  SSL_CTX_load_verify_locations(ctx,"cert.pem","/etc/ssl/certs/");
  SSL * ssl = SSL_new(ctx);
  SSL_set_fd(ssl,sockfd);
  SSL_connect(ssl);
  X509 *cert = SSL_get_peer_certificate(ssl);
  //1 for the CA that signed the certificate and so on.
  printf("Verify result %lu\n", SSL_get_verify_result(ssl));
  //Verify return code: 19 (self signed certificate in certificate chain)
  if(SSL_get_verify_result(ssl) == X509_V_OK || SSL_get_verify_result(ssl) == X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN){
      printf("Certificate valid\n");
  }
  else{
    printf("Certificate invalid\n");
    return 1;
  }
  /*printing ciphers*/
  const SSL_CIPHER *cipher = SSL_get_current_cipher(ssl);
  const char * cipher_name = NULL;
  int priority = 0;
  printf("Used cipher was %s\n",SSL_CIPHER_get_name(cipher));
  cipher_name = SSL_get_cipher_list(ssl,priority);
  while(cipher_name != NULL){
    printf("Priority: %d\t CIPHER: %s\n",priority,cipher_name);
    priority++;
    cipher_name = SSL_get_cipher_list(ssl,priority);
  }
  FILE * fcert = fopen("cert.pem","w+b");
  PEM_write_X509(fcert,cert);
  fclose(fcert);
  printf("Cert written\n");
  SSL_write(ssl,request,strlen(request));

  while((readsize = SSL_read(ssl,buffer,BUFFER_SIZE)) != 0){
	  fwrite(buffer,1,readsize,fweb);
  }

  SSL_shutdown(ssl);
  SSL_free(ssl);
  SSL_CTX_free(ctx);
  free(cert);
  fclose(fweb);
  return 0;
 }
