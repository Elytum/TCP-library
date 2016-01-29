#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <requests.h>

#include <requests_examples.h>

void	setup_requests(t_request requests[][MAX_REQUESTS])
{
	int putchar_reaquest;
	int putint_reaquest;
	int putlong_reaquest;
	int puttab_reaquest;
	int putuser_reaquest;

	bzero(*requests, sizeof(*requests));
	putchar_reaquest = add_request(requests, "putchar", &ask_putchar, NOT_POINTER);
	set_request_size(requests, putchar_reaquest, sizeof(char));

	putint_reaquest = add_request(requests, "putint", &ask_putint, NOT_POINTER);
	set_request_size(requests, putint_reaquest, sizeof(int));

	putlong_reaquest = add_request(requests, "putlong", &ask_putlong, NOT_POINTER);
	set_request_size(requests, putlong_reaquest, sizeof(long));

	add_request(requests, "putstr", &ask_putstr, POINTER);
	puttab_reaquest = add_request(requests, "puttab", &ask_puttab, POINTER);
	set_request_size(requests, puttab_reaquest, sizeof(int) * 4);
	putuser_reaquest = add_request(requests, "putuser", &ask_putuser, POINTER);
	set_request_size(requests, putuser_reaquest, sizeof(t_user));
}

int main(void)
{
	t_request	requests[MAX_REQUESTS];

	setup_requests(&requests);

	char	c = 'a';
	int		i = 32;
	long	l = 64;
	char	s[1024]; strcpy(s, "Test string\n");
	int		t[2][2]; t[0][0] = 0; t[0][1] = 1; t[1][0] = 2; t[1][1] = 3;
	t_user	u; strcpy(u.first_name, "Arthur"); strcpy(u.last_name, "Chazal"); u.age=19; strcpy(u.job, "Programmer");

	stream_requests(&requests, "putchar", sizeof(c), &c);
	stream_requests(&requests, "putint", sizeof(i), &i);
	stream_requests(&requests, "putlong", sizeof(l), &l);
	stream_requests(&requests, "putstr", strlen(s), &s);
	stream_requests(&requests, "puttab", sizeof(t), &t);
	stream_requests(&requests, "putuser", sizeof(u), &u);
	remove_requests(&requests, "putuser");
	stream_requests(&requests, "putuser", sizeof(u), &u);

	return (0);
}