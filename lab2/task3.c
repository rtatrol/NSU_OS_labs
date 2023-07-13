#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/reg.h>

int main() {
    pid_t child;            //value indentificator of process
    long orig_eax;

    child = fork();
    if (child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    } else {
        wait(NULL);
        while (1) {
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);      //next syscall
            wait(NULL);
            orig_eax = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_RAX, NULL);
            if (orig_eax == -1) {
                break;
            }
            printf("System call %ld\n", orig_eax);
        }
    }

    return 0;
}