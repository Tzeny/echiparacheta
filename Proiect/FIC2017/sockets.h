#ifndef SOCKETS_FILE
#define SOCKETS_FILE

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int connect(int portno, const char *ip);
void writeData(int socketfd, const char * buffer);
void processBuffer(int socketfd, const char * buffer);


#endif