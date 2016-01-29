#include <requests.h>
#include <string.h>
#include <stdio.h>
#include <config.h>

int		add_request(t_request requests[][MAX_REQUESTS], const char *key, void *value, t_data_info info)
{
	unsigned int	i;

	i = 0;
	while (i < MAX_REQUESTS)
	{
		if ((*requests)[i].initialized == 0)
		{
			(*requests)[i].initialized = 1;
			memcpy((*requests)[i].key, key, sizeof((*requests)[i].key));
			(*requests)[i].function = value;
			(*requests)[i].info = info;
			(*requests)[i].exact_size = -1;
			return (i);
		}
		++i;
	}
	if (DEBUG)
		printf("Couldn't add request %s, too many requests\n", key);
	return (-1);
}

int		set_request_size(t_request requests[][MAX_REQUESTS], int id, int exact_size)
{
	if ((*requests)[id].initialized == 0)
	{
		if (DEBUG)
			printf("Couldn't set request %i size, request doesn't exist\n", id);
		return (0);
	}
	(*requests)[id].exact_size = exact_size;
	return (1);
}

int		remove_request(t_request requests[][MAX_REQUESTS], int id)
{
	if ((*requests)[id].initialized == 1)
	{
		if (DEBUG)
			printf("Couldn't remove request %i, request doesn't exist\n", id);
		(*requests)[id].initialized = 0;
		return (1);
	}
	return (1);
}

int		stream_request(t_request requests[][MAX_REQUESTS], int id, long bytes, void *rawbytes)
{
	if ((*requests)[id].initialized == 1)
	{
		(*requests)[id].function(bytes, rawbytes);
		return (1);
	}
	if (DEBUG)
		printf("Couldn't stream request %i, request doesn't exist\n", id);
	return (0);
}
