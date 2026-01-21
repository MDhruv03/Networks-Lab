#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9090
#define MAX 1024

/* Function to swap characters */
void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

/* Recursive function to generate permutations */
void permute(char *str, int l, int r, char *result) {
    int i;
    if (l == r) {
        strcat(result, str);
        strcat(result, "\n");
    } else {
        for (i = l; i <= r; i++) {
            swap(&str[l], &str[i]);
            permute(str, l +1 , r , result);
            swap(&str[l], &str[i]); // backtrack
        }
    }
}


int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    char buffer[MAX], result[MAX * 5];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("\n[SERVER] Waiting for client message...\n");

    len = sizeof(client_addr);
    recvfrom(sockfd, buffer, MAX, 0,
             (struct sockaddr *)&client_addr, &len);

    printf("[SERVER] String received: %s\n", buffer);

    result[0] = '\0';
    int n = strlen(buffer);
    buffer[n - 1] = '\0';   // remove newline

    permute(buffer, 0, strlen(buffer) - 1, result);

    sendto(sockfd, result, strlen(result), 0,
           (struct sockaddr *)&client_addr, len);

    printf("[SERVER] Permutations sent to client\n");

    close(sockfd);
    return 0;
}
