#include <socketlib.h>

int			main(void)
{
	t_server	server;

	server = start_server(4242);
	// set_hook(server, "Name", &function, sizeof(t_server));
	// trigger_hook(server, "Name", (t_server)something);
	loop_server(server);
	return (0);
}
