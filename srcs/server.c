#include <socketlib.h>
#include <sys/socket.h>
#include <macros.h>
#include <stdio.h>
#include <errno.h>

static void         config_server(t_server *server, int port)
{
    // unsigned int    i;

    //initialise all sockets[] to 0 so not checked
    bzero(server->pending_sockets, sizeof(server->pending_sockets));
    bzero(server->producer_sockets, sizeof(server->producer_sockets));
    bzero(server->consumer_sockets, sizeof(server->consumer_sockets));
    // i = 0;
    // while (i < TAB_SIZE(server->pending_sockets))
    //     server->pending_sockets[i++] = 0;
    // while (i < TAB_SIZE(server->producer_sockets))
    //     server->producer_sockets[i++] = 0;
    // while (i < TAB_SIZE(server->consumer_sockets))
    //     server->consumer_sockets[i++] = 0;
    //create a master socket
    server->master_socket = new_socket();
    server->address = config_socket(NULL, port);
}

static void         config_master(t_server *server)
{
    int             opt;

    opt = TRUE;
    if (setsockopt(server->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    //bind the socket to localhost port
    if (bind(server->master_socket, (struct sockaddr *)&server->address, sizeof(server->address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(server->master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

t_server	       start_server(int port)
{
	t_server       server;

    config_server(&server, port);
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    config_master(&server);
    if (VERBOSE)
    	printf("Server started on port %d\n", port);
	return (server);
}
