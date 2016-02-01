#include <socketlib.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include <globals.h>

ssize_t					login_producer(int sock, const char *user,
                            const char *passwd, const char *product)
{
	const unsigned int	user_len = strlen(user) + 1;
	const unsigned int	passwd_len = strlen(passwd) + 1;
    const unsigned char producer_len = sizeof(PRODUCER_INTRO) - 1;
    const unsigned char product_len = strlen(product);
	size_t				len;

	if (9 + user_len + passwd_len + sizeof(int) * 2 > SEND_MAX)
		return (0);
    memcpy(buffer, &producer_len, sizeof(unsigned char));
	len = 9;
	memcpy(buffer + sizeof(producer_len), (void *)&"PRODUCER", len);
	memcpy(buffer + len, (void *)&user_len, sizeof(user_len));
	len += sizeof(user_len);
	memcpy(buffer + len, (void *)user, user_len);
	len += user_len;
	memcpy(buffer + len, (void *)&passwd_len, sizeof(passwd_len));
	len += sizeof(passwd_len);
	memcpy(buffer + len, (void *)passwd, passwd_len);
	len += passwd_len;
    memcpy(buffer + len, (void *)&product_len, sizeof(product_len));
    len += sizeof(product_len);
    memcpy(buffer + len, (void *)product, product_len);
    len += product_len;
	return (send(sock, buffer, len, 0));
}

ssize_t                 login_consumer(int sock, const char *user,
                            const char *passwd, const char *product,
                            const char *producer)
{
    const unsigned int  user_len = strlen(user) + 1;
    const unsigned int  passwd_len = strlen(passwd) + 1;
    const unsigned char consumer_len = sizeof(CONSUMER_INTRO) - 1;
    const unsigned char product_len = strlen(product);
    const unsigned char producer_len = strlen(producer);
    size_t              len;

    len = 9;
    if (len + user_len + passwd_len + sizeof(int) * 2 > SEND_MAX)
        return (0);
    memcpy(buffer, &consumer_len, sizeof(unsigned char));
    memcpy(buffer + sizeof(consumer_len), (void *)&"CONSUMER", len);
    memcpy(buffer + len, (void *)&user_len, sizeof(user_len));
    len += sizeof(user_len);
    memcpy(buffer + len, (void *)user, user_len);
    len += user_len;
    memcpy(buffer + len, (void *)&passwd_len, sizeof(passwd_len));
    len += sizeof(passwd_len);
    memcpy(buffer + len, (void *)passwd, passwd_len);
    len += passwd_len;
    memcpy(buffer + len, (void *)&product_len, sizeof(product_len));
    len += sizeof(product_len);
    memcpy(buffer + len, (void *)product, product_len);
    len += product_len;
    memcpy(buffer + len, (void *)&producer_len, sizeof(producer_len));
    len += sizeof(producer_len);
    memcpy(buffer + len, (void *)producer, producer_len);
    len += producer_len;
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

static int          add_producer(t_server *server, int sock, const char *user, int id)
{
    unsigned char   product_len;
    
    if (recv(sock , &product_len, sizeof(product_len), 0) != sizeof(product_len) ||
        !product_len || product_len >= sizeof(server->producers[id].product) ||
            recv(sock , &server->producers[id].product, product_len, 0) != product_len)
        return (0);
    printf("[PRODUCER %i] \"%s\" produce \"%s\"\n", id, user, server->producers[id].product);
    server->producers[id].socket = sock;
    strcpy(server->producers[id].name, user);
    return (1);
}

static int          add_consumer(t_server *server, int sock, const char *user, int id)
{
    unsigned char   product_len;
    unsigned char   producer_len;

    if (recv(sock , &product_len, sizeof(product_len), 0) != sizeof(product_len) ||
        !product_len || product_len >= sizeof(server->consumers[id].product) ||
        recv(sock , &(server->consumers[id].product), product_len, 0) != product_len ||
        recv(sock , &producer_len, sizeof(producer_len), 0) != sizeof(producer_len) ||
        !producer_len || producer_len >= sizeof(server->consumers[id].producer) ||
        recv(sock , &(server->consumers[id].producer), producer_len, 0) != producer_len)
        return (0);
    server->consumers[id].socket = sock;
    strcpy(server->consumers[id].name, user);
    printf("[CONSUMER %i] \"%s\" consume \"%s\" from \"%s\"\n", id, user, server->consumers[id].product, server->consumers[id].producer);
    return (1);
}

static int      check_authentication(t_server *server, int sock, const char *type, const char *user, const char *passwd)
{
    int         id;

    if (!strcmp(type, "PRODUCER"))
    {
        if ((id = next_producer(server->producers)) == -1 ||
            !correct_logins(user, passwd) || !add_producer(server, sock, user, id))
            return (0);
    }
    else if (!strcmp(type, "CONSUMER"))
    {
        if ((id = next_consumer(server->consumers)) == -1 ||
            !correct_logins(user, passwd) || !add_consumer(server, sock, user, id))
            return (0);
    }
    else
    {
        printf("Unknown type %s, disconnecting\n", type);
        return (0);
    }
    return (1);
}

int                 authentication(t_server *server, int sock)
{
    unsigned char   type_size;
    char            type[TYPE_MAX];
    int             sizes[2];
    char            logs[SEND_MAX];

    if (recv(sock , &type_size, sizeof(type_size), 0) != sizeof(type_size))
        return (0);
    if (type_size)
    {
        if (type_size >= sizeof(type) || recv(sock , type, type_size, 0) != type_size)
            return (0);
    }
    if (recv(sock , &sizes[0], sizeof(sizes[0]), 0) != sizeof(sizes[0]))
        return (0);
    if (sizes[0])
    {
        if (sizes[0] >= (int)sizeof(logs) || recv(sock , logs, sizes[0], 0) != sizes[0])
            return (0);
    }
    if (recv(sock , &sizes[1], sizeof(sizes[1]), 0) != sizeof(sizes[1]))
        return (0);
    if (sizes[1])
    {
        if (sizes[1] >= (int)sizeof(logs) || recv(sock , logs + sizes[0], sizes[1], 0) != sizes[1])
            return (0);
    }
    return (check_authentication(server, sock, type, logs, logs + sizes[0]));
}
