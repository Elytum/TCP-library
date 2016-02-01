#include <socketlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <config.h>
#include <stdio.h>
#include <string.h>

void			send_message(int sock, char *str, size_t len)
{
	const char	type[] = "print";
	char		c;

	c = sizeof(type) - 1;
	send(sock, &c, sizeof(c), 0);
	send(sock, &len, sizeof(len), 0);
	send(sock, type, sizeof(type) - 1, 0);
	send(sock, str, len, 0);
}

void			send_back_message(int sock, char *str, size_t len)
{
	const char	type[] = "send back";
	char		c;

	c = sizeof(type) - 1;
	send(sock, &c, sizeof(c), 0);
	send(sock, &len, sizeof(len), 0);
	send(sock, type, sizeof(type) - 1, 0);
	send(sock, str, len, 0);
}

int					empty_recv(int sock)
{
	char			buffer[1024];

	while (recv(sock, buffer, sizeof(buffer), 0) == sizeof(buffer))
		;
	return (0);
}

int					interested_consumer(t_server_consumer *consumers, const char *type, const char *producer)
{
	unsigned int    id;

    id = 0;
    while (id < MAX_CONSUMERS)
    {
    	if (!consumers[id].socket)
    	{
    		++id;
    		continue ;
    	}
    	if (!strcmp(producer, consumers[id].producer) &&
        	!strcmp(type, consumers[id].product))
	        return (id);
        ++id;
    }
    return (id);
}

void				distribute_production(t_server_consumer *consumers, const char *producer, const char *type, const char *data, int size)
{
    unsigned int    id;
    char			buffer[100];
    int				len;

    if ((id = interested_consumer(consumers, type, producer)) == MAX_CONSUMERS)
    	return ;
    memcpy(buffer, &size, sizeof(size));
    memcpy(buffer + sizeof(size), data, size);
    len = sizeof(size) + size;
    while (id < MAX_CONSUMERS)
    {
    	if (!consumers[id].socket)
    	{
    		++id;
    		continue ;
    	}
    	 if (!strcmp(producer, consumers[id].producer) &&
        	!strcmp(type, consumers[id].product))
        {
        	if (send(consumers[id].socket, buffer, len, 0) == -1)
        		consumers[id].socket = 0;
        }
        ++id;
    }
}

void				handle_production(t_server *server, int producer, const char *data, int size)
{
	const char		*type = server->producers[producer].product;

	if (!strcmp(type, "Detected"))
		distribute_production(server->consumers, server->producers[producer].name, "say", data, size);
	else
		printf("Unknown production %s\n", type); // UNSAFE BEHAVIOR
	(void)server;
}

int					find_producer(t_server *server, int sock)
{
	unsigned int	id;

	id = 0;
	while (id < MAX_PRODUCERS)
	{
		if (sock == server->producers[id].socket)
			return (id);
		++id;
	}
	return (-1);
}

int					receive_production(t_server *server, int sock)
{
	int				size;
	char			data[SEND_MAX];
	int				producer = find_producer(server, sock);

	if (producer == -1 || recv(sock , &size, sizeof(size), 0) != sizeof(size))
		return (-1);
	if (size)
	{
		if (size >= (int)sizeof(data) || recv(sock , data, size, 0) != size)
			return (empty_recv(sock));
	}
	handle_production(server, producer, data, size);
	return (1);
}

char			*get_input(void)
{
	static char	buffer[1024];

	buffer[read(1, buffer, sizeof(buffer) - 1) - 1] = '\0';
	return (buffer);
}

void			send_request(int sock, char *str, size_t len)
{
	const char	type[] = "print";
	char		c;

	c = sizeof(type) - 1;
	send(sock, &c, sizeof(c), 0);
	send(sock, &len, sizeof(len), 0);
	send(sock, type, sizeof(type) - 1, 0);
	send(sock, str, len, 0);
}
