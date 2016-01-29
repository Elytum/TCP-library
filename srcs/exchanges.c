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

size_t			recv_data(int sock, void *buffer, size_t size, unsigned char tries)
{
    ssize_t  i;
    ssize_t  ret;

    i = 0;
    while (tries && size)
    {
        if (!(ret = recv(sock, buffer + i, size, 0)))
        {
        	usleep(1000);
            --tries;
        }
        size -= ret;
        i += ret;
    }
    return (i);
}

int					empty_recv(int sock)
{
	char			buffer[1024];

	while (recv(sock, buffer, sizeof(buffer), 0) == sizeof(buffer))
		;
	return (0);
}

int					receive_order(int sock)
{
	unsigned char	type_size;
	unsigned int	size;
	char			type[TYPE_MAX];
	char			message[SEND_MAX];

	if (recv_data(sock , &type_size, sizeof(type_size), 1) != sizeof(type_size))
		return (-1);
	if (type_size)
	{
		if (type_size >= sizeof(type) || recv_data(sock , type, type_size, 10) != type_size)
			return (empty_recv(sock));
	}
	if (recv_data(sock , &size, sizeof(size), 10) != sizeof(size))
		return (empty_recv(sock));
	if (size)
	{
		if (size >= sizeof(message) || recv_data(sock , message, size, 10) != size)
			return (empty_recv(sock));
	}
	type[type_size] = '\0';
	if (!strcmp(type, "print"))
		printf("print: Typesize: %i [%s], size: %i [%s]\n", type_size, type, size, message);
	else if (!strcmp(type, "print tab"))
		printf("print tab: {%i %i}\n", ((int *)message)[0], ((int *)message)[1]);
	else
		printf("Unknown order %s\n", type);
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
