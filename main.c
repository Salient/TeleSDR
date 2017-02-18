#include "portaudio.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>


typedef struct {
	float left_phase;
	float right_phase;
} paTestData;



int main() {

	printf("Initializing TeleSDR...Please wait...\r\n");

        int sockfd; // socket descriptor
        char buffer[255];
        struct sockaddr_in sockconfig;
        memset(&sockconfig, 0, size_of(sockconfig));
        
	printf("Engaging network interface...\r\n");

        sockfd = socket(AF_INET,SOCK_STREAM,0);
        if (sockfd < 0 ) {
            printf("Error creating socket. Bailing \r\n");
            return 1;
        }


        sockconfig.sin_addr.s_addr = inet_addr("127.0.0.1"); // TODO: pass this address from command line
        sockconfig.sin_family = AF_INET;
        sockconfig.sin_port = htons(8321);

        if (connect(sockfd, (struct) sockconfig
}       


static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    /* Cast data passed through stream to our structure. */
    paTestData *data = (paTestData*)userData; 
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */
    
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->left_phase;  /* left */
        *out++ = data->right_phase;  /* right */
        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
        data->left_phase += 0.01f;
        /* When signal reaches top, drop back down. */
        if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
        /* higher pitch so we can distinguish left and right. */
        data->right_phase += 0.03f;
        if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
    }
    return 0;
}
