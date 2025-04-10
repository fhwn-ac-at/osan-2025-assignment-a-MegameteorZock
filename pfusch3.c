#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <threads.h>
#include <mqueue.h>
#include <fcntl.h>


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

int child_labour() {

    mqd_t command_queue = 
        mq_open("/mq_r47587", O_WRONLY | O_CREAT, S_IRWXU);
    
    
    if(command_queue == -1) 
    {
        fprintf(stderr, "Failed to open messages queue");
        return EXIT_FAILURE;
    }
    printf("[%d] Waiting for instruction...\n")
    struct work_messages instructions;
    int const received = mq_receive(command_queue, (void*) &instructions, sizeof(work_messages));
    if (received == 1)
    {
        fprintf(stderr, "failed to receive instructions, running away\n");
        return EXIT_FAILURE;
    }
    printf("[%d] Received message of size %d bytes: work to do %d\n")
    


    printf("[%d] Doing some work for (%d)\n", getpid(), getpid());
    srand(getpid());
    sleep(5);
    printf("[%d] Job's done\n", getpid());
    printf("[%d] Bringing coal to %d...\n", getpid(), getpid());

    mq_close
    
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[], char *envp[]) {
   // cli_args const args = parse_command_line(argc, argv);
  //printf("Parsed args: i: %d, s: %s, b: %d\n", args.i, args.s, args.b);




struct mq_attr queue_options(){
    .mq_maxmsg = 1;
    .mq_msgsize = sizeof(struct work_messages),
};

    mqd_t command_queue = 
        mq_open("/mq_r47587", O_WRONLY | O_CREAT, S_IRWXU);
    
    
    if(command_queue == -1) 
    {
        fprintf(stderr, "Failed to open messages queue");
        return EXIT_FAILURE;
    }





    pid_t forked = fork();
    if (forked == 0) {
        return child_labour();
    }


    struct work_messages instructions = { .works | 3 + rand() % 7 };
    int const send = mq_send(command_queue, (void*) &instructions, sizeof(work_messages));
    if(send == 1) {
        printf(stderr, "failed to send ")
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





    mq_close(command_queue);
    mq_unlink("/mq_r47587");

    return 0;
}