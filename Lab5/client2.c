#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<string.h>
#define PORT 8080

int main() {
    int s;
    struct sockaddr_in serv;
    char msg[] = "Technology";

    s = socket(AF_INET, SOCK_STREAM, 0);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);

    connect(s, (struct sockaddr*)&serv, sizeof(serv));
    send(s, msg, strlen(msg), 0);
    close(s);
    return 0;
}
