#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int s, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    s = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(s, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(s, 5);

    printf("Concurrent Server running...\n");

    // Clear previous content file
    FILE *fp = fopen("parts.txt", "w");
    fclose(fp);

    int client_count = 0;

    while (client_count < 2) {
        connfd = accept(s, (struct sockaddr*)&cliaddr, &len);
        client_count++;

        if (fork() == 0) {
            close(s);
            char buf[MAXLINE];
            int n = recv(connfd, buf, MAXLINE, 0);
            buf[n] = '\0';

            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &cliaddr.sin_addr, ip, INET_ADDRSTRLEN);

            printf("Received from %s:%d -> %s\n",
                   ip, ntohs(cliaddr.sin_port), buf);

            FILE *fp = fopen("parts.txt", "a");
            fprintf(fp, "%s\n", buf);
            fclose(fp);

            close(connfd);
            exit(0);
        }
        close(connfd);
    }

    // Wait for both child processes
    while (wait(NULL) > 0);

    // Now parent combines strings
    FILE *fp_base = fopen("college.txt", "r");
    char base[100];
    fgets(base, sizeof(base), fp_base);
    fclose(fp_base);
    base[strcspn(base, "\n")] = 0;

    FILE *fp_parts = fopen("parts.txt", "r");
    char p1[100], p2[100];
    fgets(p1, sizeof(p1), fp_parts);
    fgets(p2, sizeof(p2), fp_parts);
    fclose(fp_parts);

    p1[strcspn(p1, "\n")] = 0;
    p2[strcspn(p2, "\n")] = 0;

    printf("\nFINAL OUTPUT:\n");
    printf("%s %s %s\n", base, p1, p2);

    close(s);
    return 0;
}
