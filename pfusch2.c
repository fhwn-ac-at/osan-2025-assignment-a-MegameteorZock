#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

/*typedef struct {
    int i;
    char const *s;
    bool b;
} cli_args;


cli_args parse_command_line(int const argc, char *argv[]) {
    cli_args args = {0, NULL, false};   

    int optgot;
    while ((optgot = getopt(argc, argv, "i:s:b")) != -1) {
        switch (optgot) {
            case 'i':
                args.i = atoi(optarg);
                break;
            case 's':
                args.s = optarg;
                break;
            case 'b':
                args.b = true;
                break;
            case '?':
                printf("Usage: %s -i <number> -s <string> -b\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (args.i <= 0 || args.s == NULL || strlen(args.s) < 5) {
        printf("\nusage: i: %d, s: %s, b: %d\n", args.i, args.s ? args.s : "NULL", args.b);
        exit(EXIT_FAILURE);
    }

    return args;
}
*/

int do_work() {
    printf("[%d] Doing some work for (%d)\n", getpid(), getpid());
    srand(getpid());
    sleep(5);
    printf("[%d] Job's done\n", getpid());
    printf("[%d] Bringing coal to %d...\n", getpid(), getpid());

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[], char *envp[]) {
   // cli_args const args = parse_command_line(argc, argv);
  //printf("Parsed args: i: %d, s: %s, b: %d\n", args.i, args.s, args.b);


    printf("my PID is %d\n", getpid());


    printf("Sending a child into the mines...\n");


    pid_t forked = fork();
    if (forked == 0) {
        return do_work();
    }

    pid_t forked = fork();
    if (forked == 0) {
        return do_work();
    }

    for (int i = 0; i < 10; i++) {
        int wstatus = 0;
        pid_t waited = wait(&wstatus);
        
        if (WIFEXITED(wstatus)) {
            printf("[%d] child %d exited normally with return code %d\n",
                   getpid(), waited, WEXITSTATUS(wstatus));
        } 
        else if (WIFSIGNALED(wstatus)) {
            printf("[%d] child %d terminated with signal %d\n",
                   getpid(), waited, WTERMSIG(wstatus));
        } 
        else {
            printf("[%d] child %d ended unexpectedly, status = %d\n",
                   getpid(), waited, wstatus);
        }

        printf("[%d] wait returned %d, status is %d\n", getpid(), waited, wstatus);
    }

    printf("All children have returned\n");

    return 0;
}
