#ifndef SOCKETLIB_H
# define SOCKETLIB_H

# include <config.h>
# include <stdlib.h>

# include <arpa/inet.h>    //close

typedef struct			s_server_consumer
{
	int					socket;
	char				name[USERNAME_MAX];
	char				producer[USERNAME_MAX];
	char				product[PRODUCT_ID_MAX];
}						t_server_consumer;

typedef struct			s_server_producer
{
	int					socket;
	char				name[USERNAME_MAX];
	char				product[PRODUCT_ID_MAX];
}						t_server_producer;

typedef struct			s_server
{
	int					master_socket;
	int					pending_sockets[MAX_PENDINGS];
	int					producer_sockets[MAX_PRODUCERS];
	t_server_producer	producers[MAX_PRODUCERS];
	int					consumer_sockets[MAX_CONSUMERS];
	t_server_consumer	consumers[MAX_CONSUMERS];
	struct sockaddr_in	address;
}						t_server;

typedef struct			s_producer
{
	int					socket;
	struct sockaddr_in	address;
	char				buffer[SEND_MAX];
}						t_producer;

typedef struct			s_consumer
{
	int					socket;
	struct sockaddr_in	address;
	char				buffer[SEND_MAX];
}						t_consumer;

// typedef enum			s_login_type
// {
// 	PRODUCER = 0,
// 	CONSUMER
// }						e_login_type;

t_server			*start_server(int port);
void				socket_add(int *sockets, int new_socket, size_t max);
int					socket_add_child(int *sockets, struct _types_fd_set *readfds, int max_sd, size_t max);
void				incoming_connection(t_server *server, fd_set *readfds, int *addrlen);
void				socket_event(t_server *server, fd_set *readfds);
void				loop_server(t_server server);

ssize_t				login_producer(int sock,
						const char *user, const char *passwd,
						const char *product);
ssize_t				login_consumer(int sock,
						const char *user, const char *passwd,
						const char *product, const char *producer);

t_producer 			start_producer(const char *addr, int port);
void				stop_producer(t_producer producer);

t_consumer 			*start_consumer(const char *addr, int port);
void				stop_consumer(t_consumer *consumer);

ssize_t				produce_request(int sock, const char *type,
						unsigned char type_len, void *rawbytes, unsigned int bytes);
ssize_t				produce(int sock, const void *rawbytes, unsigned int bytes);

int 				new_socket(void);
void				delete_socket(int sock);
struct sockaddr_in	config_socket(const char *addr, int port);

void    			send_message(int sock, char *str, size_t len);
void				send_back_message(int sock, char *str, size_t len);
int					receive_production(t_server *server, int sock);
char    			*get_input(void);

int					empty_recv(int sock);

void				sockets_add(int *sockets, int new_socket, size_t max);
int					sockets_full(int *sockets, size_t max);
int              	next_socket(int *sockets, size_t max);
int             	next_producer(const t_server_producer *producers);
int             	next_consumer(const t_server_consumer *consumers);
int					authentication(t_server *server, int sock);

int			        sockets_add_child(t_server server, struct _types_fd_set *readfds);

# define TRUE   1
# define FALSE  0

#endif
