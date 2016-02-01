#include <socketlib.h>
#include <config.h>

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int         run_say(char *string)
{
    char    *argv[3];
    pid_t   child_pid;
    int     child_status;
    pid_t   tpid;

    argv[0] = "/bin/echo";
    argv[1] = string;
    argv[2] = NULL;
    child_pid = fork();
    if(child_pid == 0)
    {
        execvp(argv[0], argv);
        printf("Unknown command\n");
        exit(0);
    }
    else {
        do {
            tpid = wait(&child_status);
        } while(tpid != child_pid);

        return child_status;
    }
}

int					receive_string(t_consumer *consumer, int *len)
{
	if (recv(consumer->socket , len, sizeof(*len), 0) != sizeof(*len) ||
        *len == 0 || *len >= (int)sizeof(consumer->buffer) ||
        recv(consumer->socket , consumer->buffer, *len, 0) != *len)
		return (0);
	return (1);
}

int 			handle_say(t_consumer *consumer, fd_set *readfds)
{
	int			len;

    if (FD_ISSET(consumer->socket, readfds)) 
    {
        //Launch reception
        if (!receive_string(consumer, &len))
        {
            //somebody tried to authenticate himself but failed
            printf("Error receiving server data, disconnecting\n");
            close(consumer->socket);
            return (0);
        }
        else
            run_say(consumer->buffer);
    }
    return (1);
}

int			main(void)
{
	t_consumer		*consumer;
    fd_set			readfds; //set of socket descriptors
    int				max_sd;

	if (!(consumer = start_consumer(IP, PORT)))
        return (1);
	login_consumer(consumer->socket, "Lucca", "Sirignano", "say", "Arthur");
	while (TRUE)
	{
		//clear the socket set
        FD_ZERO(&readfds);
        //add consumer to set
        FD_SET(consumer->socket, &readfds);
        max_sd = consumer->socket;
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        if ((select(max_sd + 1 , &readfds , NULL , NULL , NULL) < 0) && (errno!=EINTR)) 
        	printf("select error");
        //If something happened on the consumer, then its a production
        if (!(handle_say(consumer, &readfds)))
        	break ;
	}
	printf("Not connected to server anymore\n");
	stop_consumer(consumer);
	return (0);
}
