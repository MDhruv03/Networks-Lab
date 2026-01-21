#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

#define PORT 8080
#define MAX 1024

void sort_desc(char *a, int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = i+1; j < n; j++)
            if (a[i] < a[j]) {
                char t = a[i]; a[i] = a[j]; a[j] = t;
            }
}

void sort_asc(char *a, int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = i+1; j < n; j++)
            if (a[i] > a[j]) {
                char t = a[i]; a[i] = a[j]; a[j] = t;
            }
}

int main() {
    int sfd, cfd;
    struct sockaddr_in addr;
    char input[MAX];

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sfd, 1);

    printf("[SERVER] Waiting for client...\n");
    cfd = accept(sfd, NULL, NULL);

    read(cfd, input, MAX);
    input[strcspn(input, "\n")] = '\0';

    pid_t pid = fork();

    if (pid == 0) {
        /* CHILD → digits ascending */
        char digits[MAX];
        int k = 0;

        for (int i = 0; input[i]; i++)
            if (input[i] >= '0' && input[i] <= '9')
                digits[k++] = input[i];

        digits[k] = '\0';
        sort_asc(digits, k);

        char msg[MAX];
        snprintf(msg, MAX,
                 "Output at the child process (PID=%d): %s\n",
                 getpid(), digits);

        write(cfd, msg, strlen(msg));
        exit(0);
    } 
    else {
        wait(NULL);   // ✅ wait for child

        /* PARENT → characters descending */
        char chars[MAX];
        int k = 0;

        for (int i = 0; input[i]; i++)
            if ((input[i] >= 'a' && input[i] <= 'z') ||
                (input[i] >= 'A' && input[i] <= 'Z'))
                chars[k++] = input[i];

        chars[k] = '\0';
        sort_desc(chars, k);

        char msg[MAX];
        snprintf(msg, MAX,
                 "Output at the parent process (PID=%d): %s\n",
                 getpid(), chars);

        write(cfd, msg, strlen(msg));
    }

    close(cfd);
    close(sfd);
    return 0;
}
