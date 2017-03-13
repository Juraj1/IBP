#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <sys/time.h>

#include "../receiver/typedefs.h"

/* 
 * sender testing program for data suply
 * author: Jiri Zahradnik
 * date: spring 2017
 */

void send(){
    struct sockaddr_in dst;
    /* init to zeros */
    memset(&dst, 0, sizeof(struct sockaddr_in));
    /* ipv4 */
    dst.sin_family = AF_INET;
    /* ipv4: 127.0.0.1 */
    dst.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    /* connect to port defined in config file */
    dst.sin_port = htons(1337);

    struct sockaddr_in src;
    /* init to zeros */
    memset(&src, 0, sizeof(struct sockaddr_in));
    /* ipv4 */
    src.sin_family = AF_INET;
    /* ipv4: 127.0.0.1 */
    src.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    /* connect to port defined in config file */
    src.sin_port = htons(1338);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cerr << "failed to create socket" << std::endl;
        return;
    }

    if(0 > bind(sock, (struct sockaddr *)&src, sizeof(struct sockaddr_in))){
        std::cerr << "Failed to connect socket" << std::endl;
        return;
    }

    struct timeval tv;
   
    packet_t packet;
    memset(&packet, 0x0, sizeof(packet_t));
    packet.altitude = 50;


    while(1){
        if(packet.altitude > 20){
            usleep(10000);
            --(packet.altitude);
        }
        else{
            usleep(100000);
            packet.altitude -= 0.05;
        }
        gettimeofday(&tv, NULL);
        packet.timestamp = 1000000 * tv.tv_sec + tv.tv_usec;

        if(0 >= packet.altitude){
            packet.altitude = 100;
        }
        if(-1 == sendto(sock, &packet, sizeof(packet_t), 0, (struct sockaddr *)&dst, sizeof(struct sockaddr))){
          std::cerr << "Failed to send UDP datagram" << std::endl;
          continue;
        }
    }
}

int main(){
  send();
  return 0;
}
