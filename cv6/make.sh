#!/bin/bash

g++ -Wall ssl.cpp -o ssl -lssl -lcrypto
./ssl
#openssl s_client -host edux.fit.cvut.cz -port 443 -showcerts
# openssl x509 -in cert.pem -text
#openssl s_client -CApath /etc/ssl/certs/ -host fit.cvut.cz -port 443 -showcerts
