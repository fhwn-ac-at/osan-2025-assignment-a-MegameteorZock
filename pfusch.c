#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>


struct command_line_arguments {
    int i;
    char const *s;
    bool b;
} cli_args;


cli_args parse_command_line(int const argc, char * argv[])
{

cli_args args = {0, NULL, false};   
        
        
    int optgot;
    do
    {
        switch (optgot)
        {
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
        printf("Usage: %s -i <number> -s <string> -b\n", argv[0])
        exit(EXIT_FAILURE);
        }
    } while (optgot != -1);

    if(args.i <= 0 || strlen(args,s) <5)
    {
        printf("\nusage: i: %d, s: %s, b: %d\n", args.i, args.s, args.b);
        exit(EXIT_FAILURE);
    }

}



int do_work() {
    printf("[%d] Doing some work for (%d)\n", getpid());
    sleep(5)
    printf("[%d] Job's done\n", getpid());
    printf("[%d] Bringing coal to %d...\n", getpid(), getpid());




    return EXIT_SUCCESS;
}


int main(int argc, char * argv[], char *envp[]) {
    //cli_args const args = parse_command_line(argc, *argv);
   // printf("i: %d, s: %s, b: %d\n", args.i, args.s, args.b);

    printf("my PID is %d\n", getpid());

    printf("Sending a child into the mines...\n")

    pid_t forked = fork();
    if (forked == 0) {
        return child_labour();
    }

    printf("[%d] Enjoying some brandy.....\n", getpid())

    printf("[%d] Where the fudge is coal\n", getpid());

    //execlp("ls", "ls", "-l", NULL);


    return 0;
}
