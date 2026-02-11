// sender.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char data[MAX], sendData[MAX];

    printf("Enter binary data: ");
    scanf("%s", data);

    int choice;
    printf("Choose Parity Type:\n1. Even\n2. Odd\nChoice: ");
    scanf("%d", &choice);

    int ones = 0;
    for (int i = 0; data[i] != '\0'; i++) {
        if (data[i] == '1')
            ones++;
    }

    int parityBit;
    if (choice == 1) // Even
        parityBit = (ones % 2 == 0) ? 0 : 1;
    else // Odd
        parityBit = (ones % 2 == 0) ? 1 : 0;

    sprintf(sendData, "%s%d", data, parityBit);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock, sendData, strlen(sendData), 0);
    printf("Sent data with parity bit: %s\n", sendData);

    close(sock);
    return 0;
}
