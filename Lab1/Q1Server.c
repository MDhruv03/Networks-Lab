#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

void sort(int arr[], int n, int asc) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if ((asc && arr[i] > arr[j]) || (!asc && arr[i] < arr[j])) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server listening on port %d...\n", PORT);
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    while (1) {
        int choice, n;
        read(client_fd, &choice, sizeof(int));

        if (choice == 5) {
            printf("Client exited.\n");
            break;
        }

        read(client_fd, &n, sizeof(int));
        int arr[n];
        read(client_fd, arr, sizeof(arr));

        char result[MAX] = "";
        char temp[50];

        if (choice == 1) { // Search
            int key, found = 0;
            read(client_fd, &key, sizeof(int));
            for (int i = 0; i < n; i++)
                if (arr[i] == key)
                    found = 1;

            if (found)
                sprintf(result, "Number %d found in the list.", key);
            else
                sprintf(result, "Number %d NOT found in the list.", key);
        }

        else if (choice == 2 || choice == 3) { // Sort
            sort(arr, n, choice == 2);
            strcat(result, "Sorted List: ");
            for (int i = 0; i < n; i++) {
                sprintf(temp, "%d ", arr[i]);
                strcat(result, temp);
            }
        }

        else if (choice == 4) { // Odd & Even
            strcat(result, "Even Numbers: ");
            for (int i = 0; i < n; i++)
                if (arr[i] % 2 == 0) {
                    sprintf(temp, "%d ", arr[i]);
                    strcat(result, temp);
                }

            strcat(result, "\nOdd Numbers: ");
            for (int i = 0; i < n; i++)
                if (arr[i] % 2 != 0) {
                    sprintf(temp, "%d ", arr[i]);
                    strcat(result, temp);
                }
        }

        write(client_fd, result, strlen(result) + 1);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
