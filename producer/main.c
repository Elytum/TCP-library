#include <socketlib.h>
#include <config.h>

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

void		send_hello(t_producer *producer)
{
	char	input[] = "Hello";

	if (!producer->socket)
	{
		if (DEBUG)
			printf("Couldn't \"send_hello\", socket is closed\n");
	}
	else if (produce(producer->socket, input, strlen(input) + 1) == -1)
		producer->socket = 0;
	else
		sleep(1);
}

int			main(void)
{
	t_producer	producer;

	producer = start_producer(IP, PORT);
	login_producer(producer.socket, "Arthur", "Chazal", "Detected");
	while (producer.socket)
		send_hello(&producer);
	printf("Not connected to server anymore\n");
	stop_producer(producer);
	return (0);
}
