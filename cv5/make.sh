#!/bin/bash

g++ -Wall ssl.cpp -o ssl -lssl -lcrypto
./ssl
 openssl x509 -in cert.pem -text

