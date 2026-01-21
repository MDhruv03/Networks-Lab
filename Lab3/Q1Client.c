#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server.\n");

    pid_t pid = fork();

    if (pid > 0) {
        /* Parent: Send messages */
        printf("CLIENT PARENT: PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, MAX);
            printf("Client: ");
            fgets(buffer, MAX, stdin);
            write(sock, buffer, strlen(buffer));
        }
    } else if (pid == 0) {
        /* Child: Receive messages */
        printf("CLIENT CHILD: PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, MAX);
            read(sock, buffer, MAX);
            printf("Server: %s", buffer);
        }
    }

    close(sock);
    return 0;
}
