#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

// A simple shell implementation

//print shell prompt
void print_prompt() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        if (strncmp(cwd, "/home", 5) != 0) {
            printf("%s", cwd);
        } else if (strncmp(cwd, "/home", 5) == 0 && strlen(cwd) == 5) {
            printf("~");
        } else if (strncmp(cwd, "/home", 5) == 0 && strlen(cwd) > 5) {
            printf("~%s", cwd + strlen("/home"));
        }
        else {
            printf("%s", cwd);
        }
    } else {
        perror("getcwd");
    }
}
char *get_input() {
    char *input = malloc(1024);
    return fgets(input, 1024, stdin);

}


// Main function
int main() {
    while (1) {
        //prompt the user
        print_prompt();
        printf("$ ");
        //read the user's input
        char *input = get_input();
        input[strcspn(input, "\n")] = 0;
        if (strncmp(input, "exit", 4) == 0) {
            break;
        }
        //parse the input into tokens
        char *args [100];
        args[0] =strtok(input, " ");
        int i = 1;
        while (args[i - 1] != NULL) {
            args[i] = strtok(NULL, " ");
            i++;
        }
        /*
        // test: print the parsed arguments
        i = 0;
        while (args[i] != NULL) {
            printf("%s", args[i]);
            i++;
        }
        */
        //evaluate the command
        if (args[0] == NULL) {
            continue;
        } else if (strcmp(args[0], "exit") == 0) {
            break;
        } else if (strcmp(args[0], "cd") == 0) {
            chdir(args[1] ? args[1] : getenv("HOME"));
        } else if (strcmp(args[0], "pwd") == 0) {
            print_prompt();
            printf("\n");
        } else {
            int p = fork();
            if (p == 0) {
                execvp(args[0], args);
                printf("\n");
            }
            else if (p > 0){
                wait(NULL);
            }
            else {
                printf("Fork Error\n");
            }
        }
        free(input);
    }
    return 0;
}