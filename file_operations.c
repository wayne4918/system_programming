#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "file_operations.h"

// 파일 입/출력 함수 // example.txt에 "Hello, hufs" 라는 글 넣기
void file_io_operations() {
    printf("Starting file_io_operations\n");
    FILE *file = fopen("example.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "Hello, hufs!\n");
    fclose(file);
    printf("File I/O operations completed.\n");
}

// 시스템/파일 정보 함수
void system_info_operations() {
    printf("Starting system_info_operations\n");
    struct sysinfo info;
    if (sysinfo(&info) != 0) {
        perror("sysinfo");
        return;
    }

    printf("Uptime: %ld seconds\n", info.uptime);
    printf("Total RAM: %ld MB\n", info.totalram / (1024 * 1024));
    printf("System info operations completed.\n");
}
