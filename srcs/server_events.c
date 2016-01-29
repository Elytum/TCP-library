#include <socketlib.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

void		      sockets_add(int *sockets, int new_socket, size_t max)
{
	unsigned int		      i;

	i = 0;
    while (i < max) 
    {
        //if position is empty
        if (sockets[i] == 0)
        {
            sockets[i] = new_socket;
            break;
        }
        ++i;
    }
}

int              sockets_full(int *sockets, size_t max)
{
    unsigned int           i;

    i = 0;
    while (i < max)
    {
        //if position is empty
        if (sockets[i] == 0)
            return (0);
        ++i;
    }
    return (1);
}

static unsigned int               sub_sockets_add_child(int *sockets, struct _types_fd_set *readfds, int max_sd, size_t max)
{
    unsigned int         i;
    int                  sd;

    i = 0;
    while (i < max) 
    {
        //socket descriptor
        sd = sockets[i];
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

int			      sockets_add_child(t_server server, struct _types_fd_set *readfds)
{
	int		      max_sd;

    max_sd = server.master_socket;
    max_sd = sub_sockets_add_child(server.pending_sockets, readfds, max_sd, MAX_PENDINGS);
    max_sd = sub_sockets_add_child(server.producer_sockets, readfds, max_sd, MAX_PRODUCERS);
    max_sd = sub_sockets_add_child(server.consumer_sockets, readfds, max_sd, MAX_CONSUMERS);
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
        sockets_add(server->pending_sockets, new_socket, MAX_PENDINGS);
    }
}

char                *authentication(int sock)
{
    unsigned char   type_size;
    char            type[TYPE_MAX];
    unsigned int    sizes[2];
    char            logs[SEND_MAX];

    if (recv_data(sock , &type_size, sizeof(type_size), 1) != sizeof(type_size))
        return (NULL);
    if (type_size)
    {
        if (type_size >= sizeof(type) || recv_data(sock , type, type_size, 10) != type_size)
            return (NULL);
    }
    if (recv_data(sock , &sizes[0], sizeof(sizes[0]), 10) != sizeof(sizes[0]))
        return (NULL);
    printf("Type: %s\n", type);
    if (sizes[0])
    {
        if (sizes[0] >= sizeof(logs) || recv_data(sock , logs, sizes[0], 10) != sizes[0])
            return (NULL);
    }
    if (recv_data(sock , &sizes[1], sizeof(sizes[1]), 10) != sizeof(sizes[1]))
        return (NULL);
    if (sizes[1])
    {
        if (sizes[1] >= sizeof(logs) || recv_data(sock , logs + sizes[0], sizes[1], 10) != sizes[1])
            return (NULL);
    }
    printf("Login: %s, Password: %s\n", logs, logs + sizes[0]);
    if (!strcmp(logs, "arthur") && !strcmp(logs + sizes[0], "chazal"))
        return (strdup(logs));
    else
    {
        printf("Failed login as %s\n", logs);
        return (NULL);
    }
}

void             update_pendings(t_server *server, fd_set *readfds)
{
    unsigned int     i;
    int     sd;
    char    *user;

    i = 0;
    while (i < MAX_PENDINGS) 
    {
        sd = server->pending_sockets[i];

        if (FD_ISSET(sd , readfds)) 
        {
            //Launch authentication test
            if (sockets_full(server->pending_sockets, MAX_PENDINGS) || !(user = authentication(sd)))
            {
                //somebody tried to authenticate himself but failed
                close(sd);
                server->pending_sockets[i] = 0;
            }
            else
            {
                printf("Logged in user: %s\n", user);
                free(user);
                sockets_add(server->producer_sockets, server->pending_sockets[i], MAX_PRODUCERS);
                server->pending_sockets[i] = 0;
            }
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
        sd = server->producer_sockets[i];

        if (FD_ISSET(sd , readfds)) 
        {
            //Launch authentication test
            if (receive_order(server->producer_sockets[i]) == -1)
            {
                //somebody tried to authenticate himself but failed
                close(sd);
                server->producer_sockets[i] = 0;
            }
        }
        ++i;
    }
}

void			 socket_event(t_server *server, fd_set *readfds)
{
    update_pendings(server, readfds);
    update_producers(server, readfds);
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
        max_sd = sockets_add_child(server, &readfds);
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        if ((select( max_sd + 1 , &readfds , NULL , NULL , NULL) < 0) && (errno!=EINTR)) 
        	printf("select error"); 
        //If something happened on the master socket , then its an incoming connection
        incoming_connection(&server, &readfds, &addrlen);
        //else its some IO operation on some other socket :)
        socket_event(&server, &readfds);
    }
}