#ifndef REQUESTS_EXAMPLES_H
# define REQUESTS_EXAMPLES_H

typedef struct	s_user
{
	char		first_name[12];
	int			age;
	char		last_name[12];
	char		job[12];
}				t_user;

void	ask_putchar(long len, char *c);
void	ask_putint(long len, int *t);
void	ask_putlong(long len, long *l);
void	ask_putstr(long len, char *s);
void	ask_puttab(long len, int t[2][2]);
void	ask_putuser(long len, t_user *user);

#endif
