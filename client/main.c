#include <socketlib.h>
#include <config.h>

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

t_client *global_client;

#define CLIENT

// void	client_event(t_client *client, fd_set *readfds, int *addrlen)
// {
// 	char	buffer[1025];
// 	int		valread;
// 	int		sd;

//     sd = client->socket;
      
//     if (FD_ISSET(sd , readfds)) 
//     {
//         //Check if it was for closing , and also read the incoming message
//         if ((valread = read( sd , buffer, 1024)) == 0)
//         {
//             //Somebody disconnected , get his details and print
//             getpeername(sd , (struct sockaddr*)&client->address , (socklen_t*)&addrlen);
//             printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(client->address.sin_addr) , ntohs(client->address.sin_port));
//             //Close the socket and mark as 0 in list for reuse
//             close( sd );
//             exit(0);
//         }
//         //Echo back the message that came in
//         else
//         {
//             //set the string terminating NULL byte on the end of the data read
//             buffer[valread] = '\0';
//             printf("[%s]\n", buffer);
//             // send(sd , buffer , strlen(buffer) , 0 );
//         }
//     }
// }

char    *get_input(void)
{
    static char buffer[1024];

    buffer[read(1, buffer, sizeof(buffer) - 1) - 1] = '\0';
    return (buffer);
}

void    send_message(int sock, char *str, size_t len)
{
    const char    type[] = "print";
    char          c;

    (void)sock;
    c = sizeof(type) - 1;
    printf("%i %zu [%s] [%s]\n", c, len, type, str);
    send(sock, &c, sizeof(c), 0);
    send(sock, &len, sizeof(len), 0);
    send(sock, type, sizeof(type) - 1, 0);
    send(sock, str, len, 0);
}

void	loop(t_client client)
{
    fd_set        readfds; //set of socket descriptors

	while(TRUE)
    {
        //clear the socket set
        FD_ZERO(&readfds);
                    /* add STDIN_FILENO */
                    FD_SET(STDIN_FILENO, &readfds);
        // //add master socket to set
        FD_SET(client.socket, &readfds);
        // // max_sd = socket_add_child(&client, &readfds, client.socket);
        // //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        if ((select(1, &readfds , NULL , NULL , NULL) < 0) && (errno!=EINTR)) 
        	printf("select error");
        // //If something happened on the master socket , then its an incoming connection
        // // incoming_connection(&client, &readfds, &addrlen);
        // //else its some IO operation on some other socket :)
        // client_event(&client, &readfds, &addrlen);
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            char *input = get_input();
            send_message(client.socket, input, strlen(input) + 1);
        }
    }
}

int		main(void)
{
   t_client    client;

    setup_signals();
   client = start_client(IP, PORT);
   
   // global_client = &client;
   // signal(SIGTERM, &client_signal);

   get_message(client);
   loop(client);
   stop_client(client);
   return 0;
}