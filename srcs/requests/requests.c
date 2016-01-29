#include <requests.h>
#include <string.h>
#include <stdio.h>
#include <config.h>

int		set_requests_size(t_request requests[][MAX_REQUESTS], const char *key, int exact_size)
{
	unsigned int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (i < MAX_REQUESTS)
	{
		if ((*requests)[i].initialized == 1 && !strcmp(key, (*requests)[i].key))
		{
			(*requests)[i].exact_size = exact_size;
			++nb;
		}
		++i;
	}
	if (nb == 0 && DEBUG)
		printf("Couldn't set request %s size, no matching request\n", key);
	return (nb);
}

int		remove_requests(t_request requests[][MAX_REQUESTS], const char *key)
{
	unsigned int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (i < MAX_REQUESTS)
	{
		if ((*requests)[i].initialized == 1 && !strcmp(key, (*requests)[i].key))
		{
			(*requests)[i].initialized = 0;
			++nb;
		}
		++i;
	}
	if (nb == 0 && DEBUG)
			printf("Couldn't remove request %s, no matching request\n", key);
	return (nb);
}

int		stream_requests(t_request requests[][MAX_REQUESTS], const char *key, long bytes, void *rawbytes)
{
	unsigned int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (i < MAX_REQUESTS)
	{
		if ((*requests)[i].initialized == 1 && !strcmp(key, (*requests)[i].key))
		{
			if ((*requests)[i].exact_size != -1 && (*requests)[i].exact_size != bytes)
			{
				if (DEBUG)
					printf("Function launch of \"%s\" failed due to the number of received bytes\n", key);
			}
			else
			{
				(*requests)[i].function(bytes, rawbytes);
				++nb;
			}
		}
		++i;
	}
	if (nb == 0 && DEBUG)
		printf("Couldn't stream request %s, no matching request\n", key);
	return (nb);
}
