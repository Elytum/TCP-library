#include <socketlib.h>

int			main(void)
{
	t_server	server;

	server = start_server(4242);
	loop_server(server);
	return (0);
}