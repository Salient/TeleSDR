/*
 * =====================================================================================
 *
 *       Filename:  client.c
 *
 *    Description:  TeleSDR Prototype Client
 *
 *        Version:  1.0
 *        Created:  02/18/2017 13:29:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>


#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <math.h>
#include <signal.h>

int main(int argc){


    memset(&local,0,sizeof(local));
	 local.sin_family = AF_INET;
	 local.sin_port = htons(port);
	 local.sin_addr.s_addr = inet_addr(addr);
    
    while (1) {
        /* Create the server socket */
        server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (server == -1) {
            perror("socket");
            close_and_exit(EXIT_FAILURE);
        }

        optval = 1;
        setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
        setsockopt(server, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(int));
        
        /* Bind and listen */
        if (bind(server, (struct sockaddr *)&local, sizeof(local))) {
            perror("bind");
            close_and_exit(EXIT_FAILURE);
        }

        if (listen(server, 0)) {
                perror("listen");
                close_and_exit(EXIT_FAILURE);
        }
        
        printf("Waiting for a new client...\n");
        printf("Use the device argument 'hackrf_tcp=%s:%d' in OsmoSDR "
               "to connect\n", addr, port);
        
        client = accept(server, (struct sockaddr *)&remote, &rlen);
        
        /* 
         * Note to this non sense open and close of the server socket.
         * 
         * Always close the server socket after we get a client as
         * HackRF only supports one user/listener, so we wont be accepting
         * any more connections till the current client disconnects.
         */
        close(server);
        
        if (client == -1) {
            perror("accept");
            continue;
        }
        
        printf("New connection from: %s\n", inet_ntoa(remote.sin_addr));
        


}
