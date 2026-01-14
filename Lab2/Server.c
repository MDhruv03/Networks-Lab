#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 4096

void reorder_file(char *fname) {
    FILE *fp = fopen(fname, "r+");
    char data[MAX];
    int n = fread(data, 1, MAX, fp);

    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (data[i] > data[j]) {
                char t = data[i];
                data[i] = data[j];
                data[j] = t;
            }

    rewind(fp);
    fwrite(data, 1, n, fp);
    fclose(fp);
}

int main() {
    int sfd, cfd;
    struct sockaddr_in addr;
    char filename[100], buffer[MAX];

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(sfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(sfd, 1);

    printf("TCP Server started...\n");
    cfd = accept(sfd, NULL, NULL);

    /* Receive filename */
    read(cfd, filename, sizeof(filename));
    FILE *fp = fopen(filename, "r+");

    if (!fp) {
        write(cfd, "File not present", 16);
        close(cfd);
        close(sfd);
        return 0;
    }

    write(cfd, "File present", 12);

    int choice;
    while (1) {
        read(cfd, &choice, sizeof(int));

        if (choice == 4) {
            printf("Client exited.\n");
            break;
        }

        /* SEARCH */
        if (choice == 1) {
            char word[50];
            read(cfd, word, sizeof(word));

            rewind(fp);
            int count = 0;
            char line[MAX];

            while (fgets(line, MAX, fp))
                if (strstr(line, word))
                    count++;

            if (count > 0)
                sprintf(buffer, "String found %d times", count);
            else
                strcpy(buffer, "String not found");

            write(cfd, buffer, sizeof(buffer));
        }

        /* REPLACE */
        else if (choice == 2) {
            char s1[50], s2[50], line[MAX];
            read(cfd, s1, sizeof(s1));
            read(cfd, s2, sizeof(s2));

            FILE *temp = fopen("temp.txt", "w");
            rewind(fp);
            int found = 0;

            while (fgets(line, MAX, fp)) {
                char *pos = strstr(line, s1);
                if (pos) {
                    found = 1;
                    fprintf(temp, "%.*s%s%s",
                            (int)(pos - line), line, s2,
                            pos + strlen(s1));
                } else
                    fputs(line, temp);
            }

            fclose(fp);
            fclose(temp);
            remove(filename);
            rename("temp.txt", filename);
            fp = fopen(filename, "r+");

            if (found)
                strcpy(buffer, "String replaced");
            else
                strcpy(buffer, "String not found");

            write(cfd, buffer, sizeof(buffer));
        }

        /* REORDER */
        else if (choice == 3) {
            fclose(fp);
            reorder_file(filename);
            fp = fopen(filename, "r+");
            strcpy(buffer, "File reordered");
            write(cfd, buffer, sizeof(buffer));
        }
    }

    fclose(fp);
    close(cfd);
    close(sfd);
    return 0;
}
