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

// ssize_t					login_producer(t_producer producer,
// 							const char *user, const char *passwd)
// {
// 	const unsigned int	user_len = strlen(user) + 1;
// 	const unsigned int	passwd_len = strlen(passwd) + 1;
// 	const char			type[] = "producer";
// 	const unsigned char	type_len = sizeof(type);
// 	size_t				len;

// 	if (sizeof(type) > TYPE_MAX ||
// 		user_len + passwd_len + sizeof(int) * 2 > SEND_MAX)
// 		return (0);
// 	memcpy(buffer, (void *)(int *)&type_len, sizeof(type_len));
// 	len = sizeof(type_len);
// 	memcpy(buffer + len, (void *)type, type_len);
// 	len += type_len;
// 	memcpy(buffer + len, (void *)&user_len, sizeof(user_len));
// 	len += sizeof(user_len);
// 	memcpy(buffer + len, (void *)user, user_len);
// 	len += user_len;
// 	memcpy(buffer + len, (void *)&passwd_len, sizeof(passwd_len));
// 	len += sizeof(passwd_len);
// 	memcpy(buffer + len, (void *)passwd, passwd_len);
// 	len += passwd_len;
// 	return (send(producer.socket, buffer, len, 0));
// }

t_producer          start_producer(const char *addr, int port)
{
   t_producer       producer;

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

void              stop_producer(t_producer producer)
{
   delete_socket(producer.socket);
}
