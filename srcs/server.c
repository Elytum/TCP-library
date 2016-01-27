#include <socketlib.h>
#include <sys/socket.h>
#include <macros.h>
#include <stdio.h>
#include <errno.h>

static int          create_master(void)
{
    int             master_socket;

    if ((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    return (master_socket);
}

static void         config_server(t_server *server, int port)
{
    unsigned int    i;

    //initialise all client_socket[] to 0 so not checked
    i = 0;
    while (i < TAB_SIZE(server->client_socket))
        server->client_socket[i++] = 0;
    //type of socket created
    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(port);
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
	//create a master socket
    server.master_socket = create_master();
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    config_master(&server);
    if (VERBOSE)
    	printf("Server started on port %d \n", port);
	return (server);
}