#include <stdlib.h>
#include <config.h>
#include <socketlib.h>

void		      sockets_add(int *sockets, int new_socket, size_t max)
{
	unsigned int		      i;

	i = 0;
    while (i < max) 
    {
        //if position is empty
        if (sockets[i] == 0)
        {
            sockets[i] = new_socket;
            break;
        }
        ++i;
    }
}

int              sockets_full(int *sockets, size_t max)
{
    unsigned int           i;

    i = 0;
    while (i < max)
    {
        //if position is empty
        if (sockets[i] == 0)
            return (0);
        ++i;
    }
    return (1);
}

int              next_socket(int *sockets, size_t max)
{
    unsigned int           i;

    i = 0;
    while (i < max)
    {
        //if position is empty
        if (sockets[i] == 0)
            return (i);
        ++i;
    }
    return (-1);
}
