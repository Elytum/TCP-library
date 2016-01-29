#include <socketlib.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define MAXBUF          1024

t_producer          start_producer(const char *addr, int port)
{
   t_producer       producer;

   producer.socket = new_socket();
   producer.address = config_socket(addr, port);
   if (connect(producer.socket, (struct sockaddr*)&producer.address, sizeof(producer.address)) != 0)
   {
      perror("Connect ");
      exit(errno);
   }
   return (producer);
}

void              stop_producer(t_producer producer)
{
   delete_socket(producer.socket);
}
