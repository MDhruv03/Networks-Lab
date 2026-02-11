// receiver.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Receiver waiting for data...\n");

    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    read(new_socket, buffer, MAX);

    printf("Received data: %s\n", buffer);

    // Parity check
    int ones = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '1')
            ones++;
    }

    int parityType;
    printf("Enter parity type used (1=Even, 2=Odd): ");
    scanf("%d", &parityType);

    int error = 0;

    if (parityType == 1) { // Even parity
        if (ones % 2 != 0)
            error = 1;
    } else { // Odd parity
        if (ones % 2 == 0)
            error = 1;
    }

    if (error)
        printf("❌ Data is CORRUPTED!\n");
    else
        printf("✅ Data received correctly.\n");

    close(new_socket);
    close(server_fd);
    return 0;
}
