#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <string.h>
#include "ipc_operations.h"

// IPC 함수 (파이프 사용)
void ipc_operations() {
    printf("Starting ipc_operations\n");
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) { // 자식 프로세스
        close(pipefd[1]); // 쓰기 끝 닫기
        char buffer[128];
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);
        close(pipefd[0]);
        printf("Child process finished ipc_operations\n");
        exit(EXIT_SUCCESS);  // 자식 프로세스 종료
    } else { // 부모 프로세스
        close(pipefd[0]); // 읽기 끝 닫기
        write(pipefd[1], "Hello from parent", 18);
        close(pipefd[1]);
        wait(NULL); // 자식 프로세스가 끝날 때까지 기다림
        printf("Parent process finished ipc_operations\n");
    }

    printf("IPC operations completed.\n");
}

// 시그널 사용 
void handle_signal(int sig) {
    printf("Received signal %d\n", sig);
}

void signal_operations() {
    printf("Starting signal_operations\n");
    signal(SIGUSR1, handle_signal);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) { // 자식 프로세스
        printf("Child process sending signal to parent\n");
        kill(getppid(), SIGUSR1);
        exit(EXIT_SUCCESS);  // 자식 프로세스 종료
    } else { // 부모 프로세스
        wait(NULL); // 자식 프로세스가 끝날 때까지 기다림
    }

    printf("Signal operations completed.\n");
}

// 메모리 매핑 
void mmap_operations() {
    printf("Starting mmap_operations\n");
    const size_t SIZE = 4096;
    const char *message = "Hello from shared memory";

    void *shared_memory = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        return;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    }

    if (pid == 0) { // 자식 프로세스
        memcpy(shared_memory, message, strlen(message) + 1);
        exit(EXIT_SUCCESS);  // 자식 프로세스 종료
    } else { // 부모 프로세스
        wait(NULL); // 자식 프로세스가 끝날 때까지 기다림
        printf("Parent process read from shared memory: %s\n", (char *)shared_memory);
        munmap(shared_memory, SIZE);
    }

    printf("Memory mapping operations completed.\n");
}

// 시스템 V 메시지 큐 
void system_v_operations() {
    printf("Starting system_v_operations\n");
    key_t key;
    int msgid;

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);

    struct msg_buffer {
        long msg_type;
        char msg_text[100];
    } message;

    message.msg_type = 1;
    snprintf(message.msg_text, sizeof(message.msg_text), "Hello from system V message queue");

    msgsnd(msgid, &message, sizeof(message), 0);
    printf("Data send is: %s\n", message.msg_text);

    msgrcv(msgid, &message, sizeof(message), 1, 0);
    printf("Data received is: %s\n", message.msg_text);

    msgctl(msgid, IPC_RMID, NULL);

    printf("System V operations completed.\n");
}
