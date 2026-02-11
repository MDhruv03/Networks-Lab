// receiver_crc.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 2048

// XOR operation
void xor(char *a, char *b, int len) {
    for (int i = 1; i < len; i++)
        a[i] = (a[i] == b[i]) ? '0' : '1';
}

// Perform CRC division
void crcDivision(char *data, char *gen, char *remainder) {
    int genLen = strlen(gen);
    char temp[MAX];
    strncpy(temp, data, genLen);
    temp[genLen] = '\0';

    int dataLen = strlen(data);

    for (int i = genLen; i < dataLen; i++) {
        if (temp[0] == '1')
            xor(temp, gen, genLen);
        memmove(temp, temp + 1, genLen - 1);
        temp[genLen - 1] = data[i];
    }

    if (temp[0] == '1')
        xor(temp, gen, genLen);

    strcpy(remainder, temp + 1);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX], generator[25], remainder[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Receiver waiting...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    read(new_socket, buffer, MAX);
    printf("Received codeword: %s\n", buffer);

    int choice;
    printf("Select CRC type (1=CRC12, 2=CRC16, 3=CRC-CCITT): ");
    scanf("%d", &choice);

    if (choice == 1)
        strcpy(generator, "1100000001111");
    else if (choice == 2)
        strcpy(generator, "11000000000000101");
    else
        strcpy(generator, "10001000000100001");

    crcDivision(buffer, generator, remainder);

    int error = 0;
    for (int i = 0; i < strlen(remainder); i++) {
        if (remainder[i] == '1') {
            error = 1;
            break;
        }
    }

    if (error)
        printf("❌ Data is CORRUPTED\n");
    else
        printf("✅ Data is NOT corrupted\n");

    close(new_socket);
    close(server_fd);
    return 0;
}
    