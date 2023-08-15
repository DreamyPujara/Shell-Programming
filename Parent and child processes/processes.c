#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <arpa/inet.h>

void print_destination_ips() {
    struct in_addr addr;

    // Sample destination IP addresses
    const char *dest_ips[] = {
        "8.8.8.8",
        "1.1.1.1",
        "208.67.222.222"
    };

    printf("Parent Process: Destination IP Addresses:\n");
    for (int i = 0; i < sizeof(dest_ips) / sizeof(dest_ips[0]); ++i) {
        inet_aton(dest_ips[i], &addr);
        printf("%s\n", inet_ntoa(addr));
    }
}

void list_files_and_directories() {
    DIR *dir;
    struct dirent *ent;

    printf("Child Process: Files and Directories in Current Directory:\n");

    if ((dir = opendir(".")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            printf("%s\n", ent->d_name);
        }
        closedir(dir);
    } else {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }
}

int main() {
    pid_t child_pid = fork();

    if (child_pid < 0) {
        perror("Fork failed");
        return EXIT_FAILURE;
    } else if (child_pid == 0) {
        // Child process
        list_files_and_directories();
    } else {
        // Parent process
        print_destination_ips();

        int status;
        waitpid(child_pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process exited with status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process terminated abnormally\n");
        }
    }

    return EXIT_SUCCESS;
}
