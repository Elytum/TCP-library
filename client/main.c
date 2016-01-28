#include <socketlib.h>
#include <config.h>

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

/*
  while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
        //add master socket to set
        FD_SET(server.master_socket, &readfds);
        max_sd = socket_add_child(&server, &readfds, server.master_socket);
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        if ((select( max_sd + 1 , &readfds , NULL , NULL , NULL) < 0) && (errno!=EINTR)) 
          printf("select error");
        //If something happened on the master socket , then its an incoming connection
        incoming_connection(&server, &readfds, &addrlen);
        //else its some IO operation on some other socket :)
        socket_event(&server, &readfds, &addrlen);
    }
*/

void       socket_event_client(t_client *client, fd_set *readfds, int *addrlen)
{
  printf("Try input\n");
  if (FD_ISSET(client->socket , readfds)) 
  {
      //Check if it was for closing , and also read the incoming message
      printf("Input detected\n");
      if (receive_order(client->socket) == -1)
      {
          //Somebody disconnected , get his details and print
      // getpeername(client->socket , (struct sockaddr*)&client->address , (socklen_t*)&addrlen);
      // printf("Server disconnected , ip %s , port %d \n" , inet_ntoa(client->address.sin_addr) , ntohs(client->address.sin_port));
          printf("Server disconnected\n");
          (void)addrlen;
          //Close the socket and mark as 0 in list for reuse
          close( client->socket );
          client->socket = 0;
      }
  }
}

void	loop(t_client client)
{
    fd_set        readfds; //set of socket descriptors
    int addrlen = 0;

	while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);
                    /* add STDIN_FILENO */
                    FD_SET(STDIN_FILENO, &readfds);
        // //add master socket to set
        FD_SET(client.socket, &readfds);
        // // max_sd = socket_add_child(&client, &readfds, client.socket);
        // //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        if ((select(1, &readfds , NULL , NULL , NULL) < 0) && (errno!=EINTR)) 
          printf("select error");
        // //If something happened on the master socket , then its an incoming connection
        // // incoming_connection(&client, &readfds, &addrlen);
        // //else its some IO operation on some other socket :)
        // client_event(&client, &readfds, &addrlen);
        socket_event_client(&client, &readfds, &addrlen);
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            char *input = get_input();
            // send_message(client.socket, input, strlen(input) + 1);
            send_back_message(client.socket, input, strlen(input) + 1);
        }
    }
}

int		main(void)
{
   t_client    client;

    setup_signals();
   client = start_client(IP, PORT);
   get_message(client);
   loop(client);
   stop_client(client);
   return 0;
}