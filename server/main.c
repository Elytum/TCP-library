#include <socketlib.h>
#include <config.h>
#include <stdio.h>

int			main(void)
{
	t_server	*server;

	if (!(server = start_server(PORT)))
	{
		printf("Not enough memory to create a server\n");
		return (0);
	}
	// set_hook(server, "Name", &function, sizeof(t_server));
	// trigger_hook(server, "Name", (t_server)something);
	loop_server(*server);
	return (0);
}
