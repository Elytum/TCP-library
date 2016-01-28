#include <socketlib.h>
#include <macros.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

int		sockets[MAX_CLIENTS];
int		max_socket = 0;

int					new_socket(void)
{
	int			sock;

	if (max_socket == TAB_SIZE(sockets))
		return (-1);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket");
		exit(errno);
	}
	sockets[max_socket++] = sock;
	return (sock);
}

int				delete_socket(int sock)
{
	int			i;

	i = 0;
	while (i < max_socket)
	{
		if (sockets[i] == sock)
		{
			close(sockets[i++]);
			while (i < max_socket)
			{
				sockets[i - 1] = sockets[i];
				++i;
			}
			--max_socket;
			return (1);
		}
		++i;
	}
	return (0);
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

void	    		closing_signal(int id)
{
	int		i;

	i = 0;
	while (i < max_socket)
    	close(sockets[i++]);
    printf("Stopped by signal: %i\n", id);
    exit(0);
}

void				setup_signals(void)
{
	signal(SIGTERM, &closing_signal);
}
