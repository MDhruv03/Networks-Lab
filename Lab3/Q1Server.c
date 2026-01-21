#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 1);
    printf("Server waiting for connection...\n");

    addr_len = sizeof(client_addr);
    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &addr_len);

    if (client_fd < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected.\n");

    pid_t pid = fork();

    if (pid > 0) {
        /* Parent: Send messages */
        printf("SERVER PARENT: PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, MAX);
            printf("Server: ");
            fgets(buffer, MAX, stdin);
            write(client_fd, buffer, strlen(buffer));
        }
    } else if (pid == 0) {
        /* Child: Receive messages */
        printf("SERVER CHILD: PID=%d PPID=%d\n", getpid(), getppid());
        while (1) {
            memset(buffer, 0, MAX);
            read(client_fd, buffer, MAX);
            printf("Client: %s", buffer);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
