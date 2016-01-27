#include <socketlib.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXBUF          1024

t_client          start_client(const char *addr, int port)
{
   t_client       client;

   client.client_socket = new_socket();
   client.address = config_socket(addr, port);
   if (connect(client.client_socket, (struct sockaddr*)&client.address, sizeof(client.address)) != 0)
   {
      perror("Connect ");
      exit(errno);
   }
   return (client);
}

void              stop_client(t_client client)
{
   close(client.client_socket);
}

void              get_message(t_client client)
{
    char buffer[MAXBUF];

    bzero(buffer, MAXBUF);
    recv(client.client_socket, buffer, sizeof(buffer), 0);
    printf("%s", buffer);
}