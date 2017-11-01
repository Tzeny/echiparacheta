#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <unistd.h>

#include <string.h>

int connect(int portno, const char *ip)
{
  int sockfd;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];

  //portno = 20232;

  /* Create a socket point */
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
     perror("ERROR opening socket");
     exit(1);
  }

  server = gethostbyname(ip);

  if (server == NULL) {
     fprintf(stderr,"ERROR, no such host\n");
     exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);

  /* Now connect to the server */
  if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
     perror("ERROR connecting");
     exit(1);
  }

  return sockfd;
}

void writeData(int socketfd, const char * buffer)
{
  int n;
  n = send(socketfd, buffer, strlen(buffer),0);

  if (n < 0) {
     perror("ERROR writing to socket");
     exit(1);
  }
}

/*void readData(int socketfd)
{
  int n;
  char *buffer;
  bzero(buffer,256);
  n = read(socketfd, buffer, 255);

  if (n < 0) {
     perror("ERROR reading from socket");
     exit(1);
  }
}*/

void processBuffer(int socketfd, const char * buffer)
{
  int index=0;
  int len = strlen(buffer);
  while(index<len)
  {
    if(!strchr("fbrls",buffer[index]))
      continue;

    char msg[20];
    sprintf(msg,"%c",buffer[index]);

    writeData(socketfd,msg);
    sleep(1);
    index++;
  }

  writeData(socketfd,"s");
}

int main(int argc, char *argv[]) {
  int socketfd = connect(20232,"193.226.12.217");

  processBuffer(socketfd, "ff");

  shutdown(socketfd,2);
  
  return 0;
}
