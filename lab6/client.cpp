#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

const int port = 8080;

int main()
{
    int sockfd = -1;

   
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        cout << ("socket client error\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len = sizeof(struct sockaddr_in);

    while (1)
    {
        char buff[1024] = {0};
        scanf("%s", buff);
        sendto(sockfd, buff, strlen(buff), 0,(struct sockaddr *)&serv_addr, len);
        

        struct sockaddr_in addr;
        recvfrom(sockfd, buff, 1024, 0,(struct sockaddr *)&addr, &len);
        printf("serv say:%s\n", buff);
    }
    close(sockfd);
    return 0;
}