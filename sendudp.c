#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

void sendMessage(char *ip, char *msg, int port)
{
        if (!*msg)
                return;

        struct sockaddr_in sin;
        int s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (s < 0) {
                return;
        }

        memset( &sin, 0, sizeof( struct sockaddr_in ) );
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        if (ip)
                sin.sin_addr.s_addr = inet_addr(ip);
        else {
                uint32_t value =  1;
                if(setsockopt(s, SOL_SOCKET, SO_BROADCAST,(char*) &value, sizeof( uint32_t )) == -1) {
                        return;
                }
                sin.sin_addr.s_addr = 0xffffffff;
        }
        sendto(s,msg,strlen(msg),0,(struct sockaddr *) &sin, sizeof(sin));
        printf("Sending: '%s' to %s port %d\n",msg,ip,port);
        close(s);
}


int main(int argc, char *argv[]) {
        if ( argc == 3 )
                sendMessage(NULL,argv[2], atoi(argv[1]));
        else if ( argc == 4 )
                sendMessage(argv[2],argv[3], atoi(argv[1]));
	else {
		printf("Usage: %s port ip message \n",argv[0]);
	}

        return 0;
}

