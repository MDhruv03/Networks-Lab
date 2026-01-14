#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 4096

int main() {
    int sock;
    struct sockaddr_in serv;
    char buffer[MAX], filename[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv, sizeof(serv));

    printf("Enter filename: ");
    scanf("%s", filename);
    write(sock, filename, sizeof(filename));

    read(sock, buffer, sizeof(buffer));
    if (strcmp(buffer, "File not present") == 0) {
        printf("File not present. Exiting.\n");
        return 0;
    }

    int choice;
    while (1) {
        printf("\n1.Search\n2.Replace\n3.Reorder\n4.Exit\nChoice: ");
        scanf("%d", &choice);
        write(sock, &choice, sizeof(int));

        if (choice == 4)
            break;

        if (choice == 1) {
            char word[50];
            printf("Enter string to search: ");
            scanf("%s", word);
            write(sock, word, sizeof(word));
        }

        else if (choice == 2) {
            char s1[50], s2[50];
            printf("Enter string to replace: ");
            scanf("%s", s1);
            printf("Enter new string: ");
            scanf("%s", s2);
            write(sock, s1, sizeof(s1));
            write(sock, s2, sizeof(s2));
        }

        read(sock, buffer, sizeof(buffer));
        printf("Server: %s\n", buffer);
    }

    close(sock);
    return 0;
}
