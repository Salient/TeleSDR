/*
 * Copyright 2014 Jaime Peñalba <jpenalbae@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * hackrf_tcp uses a quick and dirty protocol.
 * Endianess is always network order (big endian).
 * 
 * 
 * All the packets have the following structure
 * ------------------------------------------
 * | 1 byte - TYPE | Variable length data   |
 * ------------------------------------------
 * 
 * 
 * Based on the type there are diferent packets:
 * 
 *   Hello packet (sent by the server once the client connects)
 *   ---------------------------------------------------------------
 *   | 0x00 | 1 byte - board id | variable length - version | 0x00 |
 *   ---------------------------------------------------------------
 * 
 *   Command request, basically calls to libkhackrf (sent by the client)
 *   ------------------------------------------------------------------
 *   | 0x01 | 1 byte - cmd type | 2 bytes - seq | 8 bytes - parameter |
 *   ------------------------------------------------------------------
 * 
 *   Command response (sent by the server)
 *   --------------------------------------------------------
 *   | 0x02 | 2 bytes - seq | 4 bytes (signed int) - result |
 *   --------------------------------------------------------
 * 
 *   Incomming I/Q data (sent by the server)
 *   -----------------------------------------
 *   | 0x03 | 4 byte - len |    I/Q data     |
 *   -----------------------------------------
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <inttypes.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <math.h>
#include <signal.h>

int main(int argc, char** argv)
{
    int opt, result, optval, server;
    struct sockaddr_in local, remote;
    socklen_t rlen;
    char buffer[255], version[255 + 1];
    
    char* addr = "127.0.0.1";
    int port = 1234;
   
    
    
    /* Setup server addr & port */
    memset(&local,0,sizeof(local));
	 local.sin_family = AF_INET;
	 local.sin_port = htons(port);
	 local.sin_addr.s_addr = inet_addr(addr);
    
    while (1) {
        /* Create the server socket */
        server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (server == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        optval = 1;
        setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
        setsockopt(server, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(int));
        
        /* Bind and listen */
        if (bind(server, (struct sockaddr *)&local, sizeof(local))) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        if (listen(server, 0)) {
                perror("listen");
                exit(EXIT_FAILURE);
        }
        
        printf("Waiting for a new client...\n");
        printf("Use the device argument 'hackrf_tcp=%s:%d' in OsmoSDR "
               "to connect\n", addr, port);
        
        int client = accept(server, (struct sockaddr *)&remote, &rlen);
        
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
        
       char board_id; 
        /* Send hello packet */
        memset(buffer, 0, sizeof(buffer));
        buffer[0] = 0x00;
        buffer[1] = board_id;
        strncpy(&buffer[2], version, sizeof(buffer)-3);
        write(client, buffer, strlen(&buffer[2])+3);

	sprintf(version,"O hai thar. You connected to mah server\r\n",39);
	memset(buffer,0,sizeof(buffer));
	strncpy(buffer, version,strlen(version));
	write(client,buffer, strlen(buffer));

	read(client,buffer, 5);
	printf(buffer);  
	printf("End of client connection.\n");
        close(client);
    }
    
    

	return EXIT_SUCCESS;
}

