#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

const int port = 8080;

int main()
{
    int sockfd=-1;

    struct sockaddr_in servaddr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cout << "socket server error\n";
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        std::cout << "bind failed";
        exit(EXIT_FAILURE);
    }

    socklen_t len = sizeof(struct sockaddr_in);

    while (1)
    {
        char buff[1024] = {0};
        struct sockaddr_in cli_addr;

        recvfrom(sockfd, buff, 1024, 0, (struct sockaddr *)&cli_addr, &len);
        printf("client say:%s\n", buff);
        
        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&cli_addr, len);
    }
    close(sockfd);
    return 0;
}