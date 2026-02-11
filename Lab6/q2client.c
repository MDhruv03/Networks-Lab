// sender_crc.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 2048

void xor(char *a, char *b, int len) {
    for (int i = 1; i < len; i++)
        a[i] = (a[i] == b[i]) ? '0' : '1';
}

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

void stringToBinary(char *input, char *binary) {
    while (*input) {
        for (int i = 7; i >= 0; i--)
            *binary++ = ((*input >> i) & 1) + '0';
        input++;
    }
    *binary = '\0';
}

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char text[200], binary[MAX], generator[25], data[MAX], remainder[25];

    printf("Enter message: ");
    scanf(" %[^\n]", text);

    stringToBinary(text, binary);

    int choice;
    printf("Select CRC type (1=CRC12, 2=CRC16, 3=CRC-CCITT): ");
    scanf("%d", &choice);

    if (choice == 1)
        strcpy(generator, "1100000001111");
    else if (choice == 2)
        strcpy(generator, "11000000000000101");
    else
        strcpy(generator, "10001000000100001");

    int genLen = strlen(generator);

    strcpy(data, binary);
    for (int i = 0; i < genLen - 1; i++)
        strcat(data, "0");

    crcDivision(data, generator, remainder);

    strncat(binary, remainder, genLen - 1);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock, binary, strlen(binary), 0);
    printf("Sent codeword: %s\n", binary);

    close(sock);
    return 0;
}
