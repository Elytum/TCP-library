#include <socketlib.h>
#include <macros.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

int					new_socket(void)
{
	int			sock;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket");
		exit(errno);
	}
	return (sock);
}

void				delete_socket(int sock)
{
	close(sock);
}

struct sockaddr_in	config_socket(const char *addr, int port)
{
	struct sockaddr_in	config;

	/*---Initialize server address/port struct---*/
	config.sin_family = AF_INET;
	config.sin_port = htons(port);
	if (addr)
	{
		if (inet_aton(addr, (struct in_addr*)&config.sin_addr.s_addr) == 0)
		{
			perror(addr);
			exit(errno);
		}
	}
	else
		config.sin_addr.s_addr = INADDR_ANY;
	return (config);
}
