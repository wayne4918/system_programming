#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "file_operations.h"
#include "ipc_operations.h"

int main() {
    printf("Starting the program...\n");

    // 파일 입/출력 및 시스템/파일 정보 함수 호출
    file_io_operations();
    system_info_operations();

    // 프로세스 생성 및 exec 함수군 
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // 자식 프로세스에서 실행할 코드
        printf("Child process running execvp\n");
        char *args[] = {"/bin/ls", "-l", NULL};
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);  // execvp 실패 시 자식 프로세스 종료
    } else {
        // 부모 프로세스에서 실행할 코드
        printf("Parent process waiting for child\n");
        int status;
        waitpid(pid, &status, 0); // 자식 프로세스가 끝날 때까지 기다림

        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process did not exit successfully\n");
        }

        // 부모 프로세스만 IPC 및 시스템 V 통신 함수 호출
        printf("Parent process running IPC operations\n");
        ipc_operations();
        printf("Parent process running signal operations\n");
        signal_operations();
        printf("Parent process running mmap operations\n");
        mmap_operations();
        printf("Parent process running system V operations\n");
        system_v_operations();

        printf("Program finished.\n");
    }

    return EXIT_SUCCESS;
}
