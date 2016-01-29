#include <socketlib.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include <globals.h>

ssize_t					login(int sock, e_login_type type,
							const char *user, const char *passwd)
{
	const unsigned int	user_len = strlen(user) + 1;
	const unsigned int	passwd_len = strlen(passwd) + 1;
	size_t				len;

	if (9 + user_len + passwd_len + sizeof(int) * 2 > SEND_MAX)
		return (0);
	if (type == PRODUCER)
	{
		len = 9;
		memcpy(buffer, (void *)&"\x08PRODUCER", len);
	}
	else if (type == CONSUMER)
	{
		len = 9;
		memcpy(buffer, (void *)&"\x08CONSUMER", len);
	}
	memcpy(buffer + len, (void *)&user_len, sizeof(user_len));
	len += sizeof(user_len);
	memcpy(buffer + len, (void *)user, user_len);
	len += user_len;
	memcpy(buffer + len, (void *)&passwd_len, sizeof(passwd_len));
	len += sizeof(passwd_len);
	memcpy(buffer + len, (void *)passwd, passwd_len);
	len += passwd_len;
	return (send(sock, buffer, len, 0));
}


static int             correct_logins(const char *user, const char *passwd)
{
    if (!strcmp(user, "Lucca"))
    {
        return (strcmp(passwd, "Sirignano") == 0);
    }
    else if (!strcmp(user, "Marie-Ange"))
    {
        return (strcmp(passwd, "Boyomo") == 0);
    }
    else if (!strcmp(user, "Arthur"))
    {
        return (strcmp(passwd, "Chazal") == 0);
    }
    return (0);
}

static int      check_authentication(t_server *server, int sock, const char *type, const char *user, const char *passwd)
{
    if (!strcmp(type, "PRODUCER"))
    {
        if (sockets_full(server->producer_sockets, MAX_PRODUCERS) ||
            !correct_logins(user, passwd))
            return (0);
        sockets_add(server->producer_sockets, sock, MAX_PRODUCERS);
    }
    else if (!strcmp(type, "CONSUMER"))
    {
        if (sockets_full(server->consumer_sockets, MAX_PRODUCERS) ||
            !correct_logins(user, passwd))
            return (0);
        sockets_add(server->consumer_sockets, sock, MAX_PRODUCERS);
    }
    else
    {
        printf("Unknown type %s, disconnecting\n", type);
        return (0);
    }
    printf("Logged in %s as %s\n", user, type);
    return (1);
}

int                 authentication(t_server *server, int sock)
{
    unsigned char   type_size;
    char            type[TYPE_MAX];
    unsigned int    sizes[2];
    char            logs[SEND_MAX];

    if (recv_data(sock , &type_size, sizeof(type_size), 1) != sizeof(type_size))
        return (0);
    if (type_size)
    {
        if (type_size >= sizeof(type) || recv_data(sock , type, type_size, 10) != type_size)
            return (0);
    }
    if (recv_data(sock , &sizes[0], sizeof(sizes[0]), 10) != sizeof(sizes[0]))
        return (0);
    printf("Type: %s\n", type);
    if (sizes[0])
    {
        if (sizes[0] >= sizeof(logs) || recv_data(sock , logs, sizes[0], 10) != sizes[0])
            return (0);
    }
    if (recv_data(sock , &sizes[1], sizeof(sizes[1]), 10) != sizeof(sizes[1]))
        return (0);
    if (sizes[1])
    {
        if (sizes[1] >= sizeof(logs) || recv_data(sock , logs + sizes[0], sizes[1], 10) != sizes[1])
            return (0);
    }
    return (check_authentication(server, sock, type, logs, logs + sizes[0]));
}
