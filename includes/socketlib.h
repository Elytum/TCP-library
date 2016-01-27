#ifndef SOCKETLIB_H
# define SOCKETLIB_H

# include <config.h>
# include <stdlib.h>

# include <arpa/inet.h>    //close

typedef struct			e_header
{
	size_t				size;
}						t_header;

typedef struct			s_server
{
	int					i;
	t_header			header;
	int					master_socket;
	int					client_socket[MAX_CLIENTS];
	struct sockaddr_in	address;
}						t_server;

typedef struct			s_client
{
	int					i;
	t_header			header;
}						t_client;

t_server	start_server(int port);
void		socket_add(t_server *server, int new_socket);
int			socket_add_child(t_server *server, struct _types_fd_set *readfds, int max_sd);
void		incoming_connection(t_server *server, fd_set *readfds, int *addrlen);
void		socket_event(t_server *server, fd_set *readfds, int *addrlen);
void		loop_server(t_server server);

# define TRUE   1
# define FALSE  0

#endif
