#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sockfd;
    char buffer[MAX], result[MAX];
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("\nEnter a string (or 'Halt' to exit): ");
        scanf("%s", buffer);

        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&servaddr, len);

        if (strcmp(buffer, "Halt") == 0) {
            printf("Client exiting.\n");
            break;
        }

        recvfrom(sockfd, result, MAX, 0,
                 (struct sockaddr *)&servaddr, &len);

        printf("\n--- Server Response ---\n%s\n", result);
    }

    close(sockfd);
    return 0;
}
