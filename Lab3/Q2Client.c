#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9090
#define MAX 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("\n[CLIENT] Enter string: ");
    fgets(buffer, MAX, stdin);

    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("\n[CLIENT] Waiting for server response...\n\n");

    recvfrom(sockfd, buffer, MAX, 0, NULL, NULL);

    printf("[CLIENT] Permutations received:\n%s\n", buffer);

    close(sockfd);
    return 0;
}
