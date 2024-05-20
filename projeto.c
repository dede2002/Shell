#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define MAX_PATHS 64

void lsh_loop(void);
char *lsh_read_line(void);
char **lsh_split_line(char *);
int lsh_cd(char **);
int lsh_execute(char **args);
int lsh_path(char **args);
int lsh_launch(char **args);

char *paths[MAX_PATHS];
int num_paths = 0;

int main() {
    // Inicia com zero caminhos definidos
    num_paths = 0;

    // Run command loop.
    lsh_loop();

    return EXIT_SUCCESS;
}

void lsh_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("shell> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);
    } while (status);
}

char *lsh_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0; // have getline allocate a buffer for us

    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            exit(EXIT_SUCCESS);  // We received an EOF
        } else  {
            perror("readline");
            exit(EXIT_FAILURE);
        }
    }

    return line;
}

char **lsh_split_line(char *line) {
    int bufsize = MAX_ARGS;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, " erro de alocacao\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n");
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, " erro de alocacao\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n");
    }
    tokens[position] = NULL;
    return tokens;
}

int lsh_execute(char **args) {
    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }

    if (strcmp(args[0], "cd") == 0) {
        return lsh_cd(args);
    }

    if (strcmp(args[0], "path") == 0) {
        return lsh_path(args);
    }

    return lsh_launch(args);
}

int lsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, " Argumento esperado para \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

int lsh_path(char **args) {
    if (args[1] == NULL) {
        // Clear the list of paths if no paths are provided
        num_paths = 0;
        printf(" Path redefinido.\n");
    } else {
        // Clear existing paths and add new ones
        num_paths = 0;
        for (int i = 1; args[i] != NULL; i++) {
            if (num_paths >= MAX_PATHS) {
                fprintf(stderr, " Path muito longo.\n");
                return 1;
            }
            paths[num_paths] = strdup(args[i]);  // Use strdup to avoid issues with string lifespan
            num_paths++;
        }
        printf(" Novo path definido. \n");
        
    }
    return 1;
}

int lsh_launch(char **args) {
    if (num_paths == 0) {
        fprintf(stderr, " Nenhum path definido.\n");
        return 1;
    }

    for (int i = 0; i < num_paths; i++) {
        char cmd[MAX_LINE];
        snprintf(cmd, sizeof(cmd), "%s/%s", paths[i], args[0]);
        if (access(cmd, X_OK) == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                // Child process
                execv(cmd, args);
                perror("lsh");
                exit(EXIT_FAILURE);
            } else if (pid < 0) {
                // Error forking
                perror("lsh");
            } else {
                // Parent process
                int status;
                waitpid(pid, &status, 0);
            }
            return 1;
        }
    }
    printf(" Comando nao encontrado.\n");
    return 1;
}
