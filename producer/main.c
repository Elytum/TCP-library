#include <socketlib.h>
#include <config.h>

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

char		buffer[sizeof(unsigned char) + sizeof(unsigned int) + TYPE_MAX + SEND_MAX];

void		socket_event_producer(t_producer *producer, fd_set *readfds)
{
	if (FD_ISSET(producer->socket, readfds))
	{
		printf("Server disconnected\n");
		close(producer->socket);
	}
}

ssize_t		produce_request(int sock, const char *type, unsigned char type_len, void *rawbytes, unsigned int bytes)
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

ssize_t		login_producer(t_producer producer, const char *user, const char *passwd)
{
	const int			user_len = strlen(user) + 1;
	const int			passwd_len = strlen(passwd) + 1;
	const char			type[] = "producer";
	const unsigned char	type_len = sizeof(type);
	size_t				len;

	if (sizeof(type) > TYPE_MAX || user_len + passwd_len + sizeof(int) * 2 > SEND_MAX)
		return (0);
	memcpy(buffer, (void *)(int *)&type_len, sizeof(type_len));
	len = sizeof(type_len);
	memcpy(buffer + len, (void *)type, type_len);
	len += type_len;
	memcpy(buffer + len, (void *)&user_len, sizeof(user_len));
	len += sizeof(user_len);
	memcpy(buffer + len, (void *)user, user_len);
	len += user_len;
	memcpy(buffer + len, (void *)&passwd_len, sizeof(passwd_len));
	len += sizeof(passwd_len);
	memcpy(buffer + len, (void *)passwd, passwd_len);
	len += passwd_len;
	return (send(producer.socket, buffer, len, 0));
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
		printf("socket_event_producer\n");
		socket_event_producer(&producer, &readfds);
		/*			if stdin contains \n			*/
		printf("FD_ISSET\n");
		if (FD_ISSET(STDIN_FILENO, &readfds))
		{
		printf("get_input\n");
			char	*input = get_input();
		printf("produce_request\n");
			produce_request(producer.socket, "print", 5, input, strlen(input) + 1);
			// int		tab[2]; tab[0] = 42; tab[1] = 21;
			// produce_request(producer.socket, "print tab", 9, tab, sizeof(int) * 2);
		}
			printf("done\n");
	}
}

int			main(void)
{
	t_producer	producer;

	// setup_signals();
	producer = start_producer(IP, PORT);
	login_producer(producer, "arthur", "chazal");
	// produce_request(producer.socket, "print", 5, "", 0);
	loop(producer);
	stop_producer(producer);
	return 0;
}
