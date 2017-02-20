#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#include "../receiver/resender.h"

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

    alt_t a;
    char buffer[sizeof(double)];
    while(1){
        a.d_rcv = 99.9;
        memcpy(&buffer, &a.rcv, sizeof(long));

        usleep(10000);
        if(-1 == sendto(sock, &buffer, sizeof(buffer), 0, (struct sockaddr *)&dst, sizeof(struct sockaddr))){
          std::cerr << "Failed to send UDP datagram" << std::endl;
          continue;
        }
    }
}

int main(){
  send();
  return 0;
}
