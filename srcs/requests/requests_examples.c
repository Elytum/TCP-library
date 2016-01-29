#include <requests_examples.h>
#include <stdio.h>

void	ask_putchar(long len, char *c)
{
	if (len != sizeof(char))
		return ;
	printf("Char: %c\n", (char)*c);
}

void	ask_putint(long len, int *t)
{
	if (len != sizeof(int))
		return ;
	printf("Int: %i\n", (int)*t);
}

void	ask_putlong(long len, long *l)
{
	if (len != sizeof(long))
		return ;
	printf("Long: %li\n", (long)*l);
}

void	ask_putstr(long len, char *s)
{
	write(1, s, len);
	write(1, "\n", 1);
}

void	ask_puttab(long len, int t[2][2])
{
	if (len != sizeof(int) * 4)
		return ;
	printf("{\n %i, %i,\n %i, %i\n}\n", t[0][0], t[0][1], t[1][0], t[1][1]);
}

void	ask_putuser(long len, t_user *user)
{
	if (len != sizeof(t_user))
		return ;
	printf("User:\n\t\tFirst name: %s\n\t\tLast name: %s\n\t\tAge: %i\n\t\tJob: %s\n", user->first_name, user->last_name, user->age, user->job);
}