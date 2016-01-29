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
	int					producer_socket[MAX_PRODUCERS];
	struct sockaddr_in	address;
}						t_server;

typedef struct			s_producer
{
	t_header			header;
	int					socket;
	struct sockaddr_in	address;
}						t_producer;

t_server			start_server(int port);
void				socket_add(t_server *server, int new_socket);
int					socket_add_child(t_server *server, struct _types_fd_set *readfds, int max_sd);
void				incoming_connection(t_server *server, fd_set *readfds, int *addrlen);
void				socket_event(t_server *server, fd_set *readfds, int *addrlen);
void				loop_server(t_server server);

t_producer 			start_producer(const char *addr, int port);
void     			stop_producer(t_producer producer);


int 				new_socket(void);
void				delete_socket(int sock);
struct sockaddr_in	config_socket(const char *addr, int port);

void				setup_signals(void);

void    			send_message(int sock, char *str, size_t len);
void				send_back_message(int sock, char *str, size_t len);
int					receive_order(int sock);
size_t				recv_data(int sock, void *buffer, size_t size, unsigned char tries);
char    			*get_input(void);
# define TRUE   1
# define FALSE  0

#endif
