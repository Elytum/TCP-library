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
	t_header			header;
	int					master_socket;
	int					client_socket[MAX_CLIENTS];
	struct sockaddr_in	address;
}						t_server;

typedef struct			s_client
{
	t_header			header;
	int					server_socket;
	int					client_socket;
	struct sockaddr_in	address;
}						t_client;

t_server	start_server(int port);
void		socket_add(t_server *server, int new_socket);
int			socket_add_child(t_server *server, struct _types_fd_set *readfds, int max_sd);
void		incoming_connection(t_server *server, fd_set *readfds, int *addrlen);
void		socket_event(t_server *server, fd_set *readfds, int *addrlen);
void		loop_server(t_server server);

t_client 	start_client(const char *addr, int port);
void     	get_message(t_client client);
void     	stop_client(t_client client);


int 				new_socket(void);
struct sockaddr_in	config_socket(const char *addr, int port);

# define TRUE   1
# define FALSE  0

#endif
