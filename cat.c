#include <stdio.h>
#include <stdlib.h>

void print_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char buffer[BUFSIZ];
    while (fgets(buffer, sizeof(buffer), file)) {
        fputs(buffer, stdout);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++) {
        print_file(argv[i]);
    }

    return EXIT_SUCCESS;
}
