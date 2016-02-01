#include <socketlib.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <globals.h>

// ssize_t				produce_request(int sock, const char *type,
// 						unsigned char type_len, void *rawbytes, unsigned int bytes)
// {
// 	size_t			len;

// 	if (type_len > TYPE_MAX || bytes > SEND_MAX)
// 		return (0);
// 	memcpy(buffer, (void *)(int *)&type_len, sizeof(type_len));
// 	len = sizeof(type_len);
// 	memcpy(buffer + len, (void *)type, type_len);
// 	len += type_len;
// 	memcpy(buffer + len, (void *)&bytes, sizeof(bytes));
// 	len += sizeof(bytes);
// 	memcpy(buffer + len, (void *)rawbytes, bytes);
// 	len += bytes;
// 	return (send(sock, buffer, len, 0));
// }

t_consumer			*start_consumer(const char *addr, int port)
{
	t_consumer		*consumer;

	if (!(consumer = (t_consumer *)malloc(sizeof(t_consumer))))
		return (NULL);
	consumer->socket = new_socket();
	consumer->address = config_socket(addr, port);
	if (connect(consumer->socket, (struct sockaddr*)&consumer->address,
										sizeof(consumer->address)) != 0)
	{
		perror("Connect ");
		exit(errno);
	}
	return (consumer);
}

void				stop_consumer(t_consumer *consumer)
{
	delete_socket(consumer->socket);
	free(consumer);
}

int             	next_consumer(const t_server_consumer *consumers)
{
    unsigned int           i;

    i = 0;
    while (i < MAX_CONSUMERS)
    {
        //if position is empty
        if (consumers[i].socket == 0)
            return (i);
        ++i;
    }
    return (-1);
}
