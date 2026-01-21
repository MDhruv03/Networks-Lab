#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sock;
    struct sockaddr_in addr;
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&addr, sizeof(addr));

    printf("Input string: ");
    fgets(buffer, MAX, stdin);
    write(sock, buffer, strlen(buffer));

    printf("\nAt the server side:\n");

    /* Read exactly two messages */
    for (int i = 0; i < 2; i++) {
        memset(buffer, 0, MAX);
        read(sock, buffer, MAX);
        printf("%s", buffer);
    }

    close(sock);
    return 0;
}
