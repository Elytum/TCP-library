#include <socketlib.h>

int		main(void)
{
   t_client    client;

   client = start_client("172.20.20.110", 4242);
   get_message(client);
   stop_client(client);
   return 0;
}