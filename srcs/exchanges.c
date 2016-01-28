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
    size_t  i;
    size_t  ret;

    i = 0;
    while (tries && size)
    {
        if (!(ret = recv(sock, buffer + i, size, 0)))
            --tries;
        size -= ret;
        i += ret;
    }
    return (i);
}

int					receive_order(int sock)
{
	unsigned char	type_size;
	size_t			size;
	char			type[TYPE_MAX];
	char			message[SEND_MAX];

	printf("Receive order\n");
	if (recv_data(sock , &type_size, sizeof(type_size), 1) != sizeof(type_size))
		return (-1);
	if (recv_data(sock , &size, sizeof(size), 2) != sizeof(size))
		return (0) ;
	if (recv_data(sock , type, type_size, 2) != type_size)
		return (0) ;
	if (recv_data(sock , message, size, 10) != size)
		return (0) ;
	if (!strncmp(type, "print", 5))
		printf("print: Typesize: %i [%s], size: %i [%s]\n", type_size, type, size, message);
	else if (!strcmp(type, "send back"))
	{
		printf("send back: Typesize: %i [%s], size: %i [%s]\n", type_size, type, size, message);
		send_message(sock, message, size);
	}
	return (1);
}

char			*get_input(void)
{
	static char	buffer[1024];

	buffer[read(1, buffer, sizeof(buffer) - 1) - 1] = '\0';
	return (buffer);
}
