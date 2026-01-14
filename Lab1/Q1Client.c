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
    char buffer[MAX];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        int choice, n, key;

        printf("\n1.Search\n2.Sort Ascending\n3.Sort Descending\n4.Split Odd & Even\n5.Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        write(sock, &choice, sizeof(int));
        if (choice == 5)
            break;

        printf("Enter number of elements: ");
        scanf("%d", &n);

        int arr[n];
        printf("Enter elements:\n");
        for (int i = 0; i < n; i++)
            scanf("%d", &arr[i]);

        write(sock, &n, sizeof(int));
        write(sock, arr, sizeof(arr));

        if (choice == 1) {
            printf("Enter number to search: ");
            scanf("%d", &key);
            write(sock, &key, sizeof(int));
        }

        read(sock, buffer, MAX);
        printf("Server Response:\n%s\n", buffer);
    }

    close(sock);
    return 0;
}
