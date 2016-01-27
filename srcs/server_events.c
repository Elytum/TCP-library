#include <socketlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

void		      socket_add(t_server *server, int new_socket)
{
	int		      i;

	i = 0;
    while (i < MAX_CLIENTS) 
    {
        //if position is empty
        if (server->client_socket[i] == 0)
        {
            server->client_socket[i] = new_socket;
            printf("Adding to list of sockets as %d\n" , i);
            break;
        }
        ++i;
    }
}

int			      socket_add_child(t_server *server, struct _types_fd_set *readfds, int max_sd)
{
	int		      i;
	int		      sd;

	i = 0;
	while (i < MAX_CLIENTS) 
    {
        //socket descriptor
        sd = server->client_socket[i];
        //if valid socket descriptor then add to read list
        if (sd > 0)
            FD_SET(sd, readfds);
        //highest file descriptor number, need it for the select function
        if (sd > max_sd)
            max_sd = sd;
        ++i;
    }
    return (max_sd);
}

void			 incoming_connection(t_server *server, fd_set *readfds, int *addrlen)
{
	int			 new_socket;
	const char	 message[] = "You're connected\n";

	if (FD_ISSET(server->master_socket, readfds)) 
    {
        if ((new_socket = accept(server->master_socket, (struct sockaddr *)&server->address, (socklen_t*)addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        //inform user of socket number - used in send and receive commands
        if (VERBOSE)
        	printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(server->address.sin_addr) , ntohs(server->address.sin_port));
        //send new connection greeting message
        if (send(new_socket, message, sizeof(message) - 1, 0) != (ssize_t)strlen(message))
            perror("send");
        //add new socket to array of sockets
        socket_add(server, new_socket);
    }
}

void			 socket_event(t_server *server, fd_set *readfds, int *addrlen)
{
	char	buffer[1025];
	int		i;
	int		valread;
	int		sd;

	i = 0;
	while (i < MAX_CLIENTS) 
    {
        sd = server->client_socket[i];
          
        if (FD_ISSET( sd , readfds)) 
        {
            //Check if it was for closing , and also read the incoming message
            if ((valread = read( sd , buffer, 1024)) == 0)
            {
                //Somebody disconnected , get his details and print
                getpeername(sd , (struct sockaddr*)&server->address , (socklen_t*)&addrlen);
                printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(server->address.sin_addr) , ntohs(server->address.sin_port));
                //Close the socket and mark as 0 in list for reuse
                close( sd );
                server->client_socket[i] = 0;
            }
            //Echo back the message that came in
            else
            {
                //set the string terminating NULL byte on the end of the data read
                buffer[valread] = '\0';
                send(sd , buffer , strlen(buffer) , 0 );
            }
        }
        ++i;
    }
}

void		      loop_server(t_server server)
{
	int		      addrlen;
    fd_set        readfds; //set of socket descriptors
    int		      max_sd, activity;

	while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
        //add master socket to set
        FD_SET(server.master_socket, &readfds);
        max_sd = socket_add_child(&server, &readfds, server.master_socket);
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
        if ((activity < 0) && (errno!=EINTR)) 
        	printf("select error");
        //If something happened on the master socket , then its an incoming connection
        incoming_connection(&server, &readfds, &addrlen);
        //else its some IO operation on some other socket :)
        socket_event(&server, &readfds, &addrlen);
    }
	(void)server;
}