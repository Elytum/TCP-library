#include <socketlib.h>
#include <config.h>

int			main(void)
{
	t_server	server;

	server = start_server(PORT);
	// set_hook(server, "Name", &function, sizeof(t_server));
	// trigger_hook(server, "Name", (t_server)something);
	loop_server(server);
	return (0);
}
