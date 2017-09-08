#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

/*

for connection_handler handler

char buffer[1024];
strcpy(buffer, "Message from proxy");
write(sockfd, buffer, strlen(buffer));
close(sockfd)
*/

#define port 8000

void sock_send(const char *text, int sockfd){
  char buffer[6000];
  strcpy(buffer, text);
  write(sockfd, buffer, strlen(buffer));
  close(sockfd);
}

void connection_handler(int sockfd){
  int read_size;
  char readbuffer[1024];
  char headers[4000];
  while (1){
    if ((read_size = recv(sockfd, readbuffer, 2000, 0)) > 0) {
      strcat(headers, readbuffer);
    }
    sock_send(headers, sockfd);
  }
}

int main(int argc, char const *argv[]) {
  int listenfd = 0, connfd = 0;

  struct sockaddr_in serv_addr;

  char sendBuff[1025];

  listenfd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  printf("%s\n", "Binded");

  if(listen(listenfd, 10) == -1){
      printf("Failed to listen\n");
      return -1;
  }

  while (1){
    connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
    connection_handler(connfd);
  }
  return 0;
}
