#include <socketlib.h>
#include <sys/socket.h>

static unsigned int               check_pendings_childs(int *sockets, struct _types_fd_set *readfds, int max_sd)
{
    unsigned int         i;
    int                  sd;

    i = 0;
    while (i < MAX_PENDINGS) 
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

static unsigned int               check_producers_childs(t_server_producer *producers, struct _types_fd_set *readfds, int max_sd)
{
    unsigned int         i;
    int                  sd;

    i = 0;
    while (i < MAX_PRODUCERS) 
    {
        //socket descriptor
        sd = producers[i].socket;
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

static unsigned int               check_consumers_childs(t_server_consumer *consumers, struct _types_fd_set *readfds, int max_sd)
{
    unsigned int         i;
    int                  sd;

    i = 0;
    while (i < MAX_CONSUMERS) 
    {
        //socket descriptor
        sd = consumers[i].socket;
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

int			               sockets_add_child(t_server server, struct _types_fd_set *readfds)
{
	int		      max_sd;

    max_sd = server.master_socket;
    max_sd = check_pendings_childs(server.pending_sockets, readfds, max_sd);
    max_sd = check_producers_childs(server.producers, readfds, max_sd);
    max_sd = check_consumers_childs(server.consumers, readfds, max_sd);
    return (max_sd);
}