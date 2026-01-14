#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define MAX 1024

int isPalindrome(char str[]) {
    int i = 0, j = strlen(str) - 1;
    while (i < j) {
        if (str[i] != str[j])
            return 0;
        i++;
        j--;
    }
    return 1;
}

int main() {
    int sockfd;
    char buffer[MAX], result[MAX];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("UDP Server running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, MAX);
        recvfrom(sockfd, buffer, MAX, 0,
                 (struct sockaddr *)&cliaddr, &len);

        if (strcmp(buffer, "Halt") == 0) {
            printf("Termination signal received. Server exiting.\n");
            break;
        }

        int len_str = strlen(buffer);
        int a = 0, e = 0, i = 0, o = 0, u = 0;

        for (int k = 0; k < len_str; k++) {
            char ch = tolower(buffer[k]);
            if (ch == 'a') a++;
            else if (ch == 'e') e++;
            else if (ch == 'i') i++;
            else if (ch == 'o') o++;
            else if (ch == 'u') u++;
        }

        sprintf(result,
                "String: %s\n"
                "Length: %d\n"
                "Palindrome: %s\n"
                "Vowel Count:\n"
                "a = %d, e = %d, i = %d, o = %d, u = %d",
                buffer, len_str,
                isPalindrome(buffer) ? "YES" : "NO",
                a, e, i, o, u);

        sendto(sockfd, result, strlen(result) + 1, 0,
               (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}
