#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

void print_file_info(const char *path, const char *filename) {
    struct stat file_stat;
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, filename);

    if (stat(full_path, &file_stat) == -1) {
        perror("stat");
        return;
    }

    // File type and permissions
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");

    // Number of hard links
    printf("%ld ", (long)file_stat.st_nlink);

    // Owner name
    struct passwd *pw = getpwuid(file_stat.st_uid);
    printf("%s ", pw ? pw->pw_name : "UNKNOWN");

    // Group name
    struct group *gr = getgrgid(file_stat.st_gid);
    printf("%s ", gr ? gr->gr_name : "UNKNOWN");

    // File size
    printf("%ld ", (long)file_stat.st_size);

    // Last modification time
    char time_buf[80];
    struct tm *timeinfo;
    timeinfo = localtime(&file_stat.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", timeinfo);
    printf("%s ", time_buf);

    // File name
    printf("%s\n", filename);
}

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;
    char *path = ".";
    int long_listing = 0;
    int show_all = 0;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            long_listing = 1;
        } else if (strcmp(argv[i], "-a") == 0) {
            show_all = 1;
        } else {
            path = argv[i];
        }
    }

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (!show_all && dir->d_name[0] == '.') {
                continue;
            }
            if (long_listing) {
                print_file_info(path, dir->d_name);
            } else {
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);
    } else {
        perror("my_ls");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
