#include <socketlib.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <globals.h>

ssize_t				produce_request(int sock, const char *type,
						unsigned char type_len, void *rawbytes, unsigned int bytes)
{
	size_t			len;

	if (type_len > TYPE_MAX || bytes > SEND_MAX)
		return (0);
	memcpy(buffer, (void *)(int *)&type_len, sizeof(type_len));
	len = sizeof(type_len);
	memcpy(buffer + len, (void *)type, type_len);
	len += type_len;
	memcpy(buffer + len, (void *)&bytes, sizeof(bytes));
	len += sizeof(bytes);
	memcpy(buffer + len, (void *)rawbytes, bytes);
	len += bytes;
	return (send(sock, buffer, len, 0));
}

ssize_t				produce(int sock, const void *rawbytes, unsigned int bytes)
{
	size_t			len;

	if (sizeof(bytes) + bytes > SEND_MAX)
		return (0);
	memcpy(buffer, (void *)&bytes, sizeof(bytes));
	len = sizeof(bytes);
	memcpy(buffer + len, (void *)rawbytes, bytes);
	len += bytes;
	return (send(sock, buffer, len, 0));
}

t_producer			start_producer(const char *addr, int port)
{
	t_producer		producer;

	producer.socket = new_socket();
	producer.address = config_socket(addr, port);
	if (connect(producer.socket, (struct sockaddr*)&producer.address,
										sizeof(producer.address)) != 0)
	{
		perror("Connect ");
		exit(errno);
	}
	return (producer);
}

void				stop_producer(t_producer producer)
{
	delete_socket(producer.socket);
}

int             	next_producer(const t_server_producer *producers)
{
    unsigned int           i;

    i = 0;
    while (i < MAX_PRODUCERS)
    {
        //if position is empty
        if (producers[i].socket == 0)
            return (i);
        ++i;
    }
    return (-1);
}
