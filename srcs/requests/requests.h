#ifndef REQUEST_H
# define REQUEST_H

#define MAX_REQUESTS 10
#define TAB_SIZE(tab) (sizeof(tab) / sizeof(*tab))

#include <stdlib.h>

typedef enum		s_data_info
{
	POINTER,
	NOT_POINTER			
}					t_data_info;

typedef struct	s_request
{
	char		initialized;
	char		key[10];
	void		(*function)(long bytes, void *rawbytes);
	t_data_info	info;
	long		exact_size;
	int			id;
}				t_request;

int		add_request(t_request requests[][MAX_REQUESTS], const char *key, void *value, t_data_info info);
int		set_requests_size(t_request requests[][MAX_REQUESTS], const char *key, int exact_size);
int		set_request_size(t_request requests[][MAX_REQUESTS], int id, int exact_size);
int		remove_requests(t_request requests[][MAX_REQUESTS], const char *key);
int		remove_request(t_request requests[][MAX_REQUESTS], int id);
int		stream_requests(t_request requests[][MAX_REQUESTS], const char *key, long bytes, void *rawbytes);
int		stream_request(t_request requests[][MAX_REQUESTS], int id, long bytes, void *rawbytes);

#endif
