#include <socketlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

void			 incoming_connection(t_server *server, fd_set *readfds, int *addrlen)
{
	int			 new_socket;
	// const char	 message[] = "You're connected\n";

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
        // if (send(new_socket, message, sizeof(message) - 1, 0) != (ssize_t)strlen(message))
        //     perror("send");
        //add new socket to array of sockets
        sockets_add(server->pending_sockets, new_socket, MAX_PENDINGS);
    }
}

void             update_pendings(t_server *server, fd_set *readfds)
{
    unsigned int     i;
    int              sd;

    i = 0;
    while (i < MAX_PENDINGS) 
    {
        sd = server->pending_sockets[i];

        if (FD_ISSET(sd , readfds)) 
        {
            //Launch authentication test
            if (!authentication(server, sd))
                close(sd);
            server->pending_sockets[i] = 0;
        }
        ++i;
    }
}

void             update_producers(t_server *server, fd_set *readfds)
{
    unsigned int     i;
    int     sd;

    i = 0;
    while (i < MAX_PRODUCERS) 
    {
        sd = server->producers[i].socket;

        if (FD_ISSET(sd , readfds)) 
        {
            //Launch reception
            if (receive_production(server, sd) == -1)
            {
                //somebody tried to authenticate himself but failed
                printf("Producer \"%s\" disconnected\n", server->producers[i].name);
                close(sd);
                server->producers[i].socket = 0;
            }
        }
        ++i;
    }
}

void             update_consumers(t_server *server, fd_set *readfds)
{
    unsigned int     i;
    int     sd;

    i = 0;
    while (i < MAX_CONSUMERS) 
    {
        sd = server->consumers[i].socket;

        if (FD_ISSET(sd, readfds)) 
        {
            printf("Consumer \"%s\" disconnected\n", server->consumers[i].name);
            close(sd);
            server->consumers[i].socket = 0;
        }
        ++i;
    }
}

void		      loop_server(t_server server)
{
	int		      addrlen;
    fd_set        readfds; //set of socket descriptors
    int		      max_sd;

	while(TRUE) 
    {
        //clear the socket set
        FD_ZERO(&readfds);
        //add master socket to set
        FD_SET(server.master_socket, &readfds);
        //add every other sockets to set
        max_sd = sockets_add_child(server, &readfds);
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        if ((select( max_sd + 1 , &readfds , NULL , NULL , NULL) < 0) && (errno!=EINTR)) 
        	printf("select error"); 
        //If something happened on the master socket , then its an incoming connection
        incoming_connection(&server, &readfds, &addrlen);
        //else its some IO operation on some other socket :)
        update_producers(&server, &readfds);
        update_consumers(&server, &readfds);
        update_pendings(&server, &readfds);
    }
}