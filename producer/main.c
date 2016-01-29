#include <socketlib.h>
#include <config.h>

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

void		socket_event_producer(t_producer *producer, fd_set *readfds)
{
	printf("Try input\n");
	if (FD_ISSET(producer->socket, readfds)) 
		close(producer->socket);
}

ssize_t		produce_request(int sock, const char *type, unsigned char type_len, void *rawbytes, unsigned int bytes)
{
	static char		buffer[sizeof(type_len) + sizeof(bytes) + TYPE_MAX + SEND_MAX];
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

void		loop(t_producer producer)
{
	fd_set	readfds; //set of socket descriptors

	while(TRUE)
	{
		printf("Loop\n");
		/*			clear the socket set 			*/
		FD_ZERO(&readfds);
		/*			add STDIN_FILENO				*/
		FD_SET(STDIN_FILENO, &readfds);
		/*			add master socket to set		*/
		FD_SET(producer.socket, &readfds);
		/*			refresh the select				*/
		if ((select(1, &readfds , NULL , NULL , NULL) < 0) && (errno!=EINTR)) 
			printf("select error");
		socket_event_producer(&producer, &readfds);
		/*			if stdin contains \n			*/
		if (FD_ISSET(STDIN_FILENO, &readfds))
		{
			char *input = get_input();
			produce_request(producer.socket, "print", 5, input, strlen(input) + 1);
		}
	}
}

int			main(void)
{
	t_producer	producer;

	setup_signals();
	producer = start_producer(IP, PORT);
	loop(producer);
	stop_producer(producer);
	return 0;
}

