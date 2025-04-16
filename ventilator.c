#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define MAX_MSG_SIZE 32
#define TASK_QUEUE "/task_queue"
#define RESULT_QUEUE "/result_queue"


// structur for a task message
struct task_msg {
    int effort;
};

// structur for a result message
struct result_msg {
    int worker_id;
    pid_t pid;
    int task_count;
    int total_time;
};

// function that a worker process executes
void worker(int worker_id) {
    mqd_t task_q = mq_open(TASK_QUEUE, O_RDONLY);
    mqd_t result_q = mq_open(RESULT_QUEUE, O_WRONLY);

    struct task_msg task;
    struct result_msg result = {worker_id, getpid(), 0, 0};

    // as long as they're is a task to do
    while (1) {
        if (mq_receive(task_q, (char*)&task, MAX_MSG_SIZE, NULL) == -1) continue;
        if (task.effort == 0) break;

        printf("%02ld:%02ld:%02ld | Worker #%02d | Received task with effort %d\n",
            (long)(time(NULL) / 3600 % 24),
            (long)(time(NULL) / 60 % 60),
            (long)(time(NULL) % 60),
            worker_id, task.effort);

        sleep(task.effort);
        result.task_count++;
        result.total_time += task.effort;
    }

    // termination task
    printf("%02ld:%02ld:%02ld | Worker #%02d | Received termination task\n",
        (long)(time(NULL) / 3600 % 24),
        (long)(time(NULL) / 60 % 60),
        (long)(time(NULL) % 60), worker_id);

    mq_send(result_q, (const char*)&result, sizeof(result), 0);
    mq_close(task_q);
    mq_close(result_q);
    exit(0);
}


int main(int argc, char *argv[]) {
    int workers = 0, tasks = 0, queue_size = 0;
    int opt;

    // parse command line arguments
    while ((opt = getopt(argc, argv, "w:t:s:")) != -1) {
        switch (opt) {
            case 'w': workers = atoi(optarg); break;
            case 't': tasks = atoi(optarg); break;
            case 's': queue_size = atoi(optarg); break;
            default:
                fprintf(stderr, "Usage: %s -w <workers> -t <tasks> -s <queue_size>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    //initialize
    srand(time(NULL));
    printf("%02ld:%02ld:%02ld | Ventilator | Starting %d workers for %d tasks and a queue size of %d\n",
        (long)(time(NULL) / 3600 % 24),
        (long)(time(NULL) / 60 % 60),
        (long)(time(NULL) % 60),
        workers, tasks, queue_size);

    // create message queues
    struct mq_attr attr = {.mq_flags = 0, .mq_maxmsg = queue_size, .mq_msgsize = MAX_MSG_SIZE, .mq_curmsgs = 0};
    mq_unlink(TASK_QUEUE);
    mq_unlink(RESULT_QUEUE);
    mqd_t task_q = mq_open(TASK_QUEUE, O_CREAT | O_WRONLY, 0666, &attr);
    mqd_t result_q = mq_open(RESULT_QUEUE, O_CREAT | O_RDONLY, 0666, &attr);

    //creates workers
    pid_t pids[MAX_WORKERS];

    for (int i = 0; i < workers; i++) {
        pid_t pid = fork();
        if (pid == 0) worker(i + 1);
        pids[i] = pid;
        printf("%02ld:%02ld:%02ld | Worker #%02d | Started worker PID %d\n",
            (long)(time(NULL) / 3600 % 24),
            (long)(time(NULL) / 60 % 60),
            (long)(time(NULL) % 60), i + 1, pid);
    }

    // task distribution
    printf("%02ld:%02ld:%02ld | Ventilator | Distributing tasks\n",
        (long)(time(NULL) / 3600 % 24),
        (long)(time(NULL) / 60 % 60),
        (long)(time(NULL) % 60));

    for (int i = 0; i < tasks; i++) {
        struct task_msg task = {.effort = (rand() % 10) + 1};
        printf("%02ld:%02ld:%02ld | Ventilator | Queuing task #%d with effort %d\n",
            (long)(time(NULL) / 3600 % 24),
            (long)(time(NULL) / 60 % 60),
            (long)(time(NULL) % 60), i + 1, task.effort);
        mq_send(task_q, (const char*)&task, sizeof(task), 0);
    }

    // termination tasks
    printf("%02ld:%02ld:%02ld | Ventilator | Sending termination tasks\n",
        (long)(time(NULL) / 3600 % 24),
        (long)(time(NULL) / 60 % 60),
        (long)(time(NULL) % 60));

    struct task_msg term = {.effort = 0};
    for (int i = 0; i < workers; i++) mq_send(task_q, (const char*)&term, sizeof(term), 0);

    // Waiting for results
    printf("%02ld:%02ld:%02ld | Ventilator | Waiting for workers to terminate\n",
        (long)(time(NULL) / 3600 % 24),
        (long)(time(NULL) / 60 % 60),
        (long)(time(NULL) % 60));

    for (int i = 0; i < workers; i++) {
        struct result_msg result;
        mq_receive(result_q, (char*)&result, sizeof(result), NULL);
        printf("%02ld:%02ld:%02ld | Ventilator | Worker %d processed %d tasks in %d seconds\n",
            (long)(time(NULL) / 3600 % 24),
            (long)(time(NULL) / 60 % 60),
            (long)(time(NULL) % 60),
            result.worker_id, result.task_count, result.total_time);
    }

    // clear: end processes and queues
    for (int i = 0; i < workers; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        printf("%02ld:%02ld:%02ld | Ventilator | Worker %d with PID %d exited with status %d\n",
            (long)(time(NULL) / 3600 % 24),
            (long)(time(NULL) / 60 % 60),
            (long)(time(NULL) % 60),
            i + 1, pids[i], WEXITSTATUS(status));
    }

    mq_close(task_q);
    mq_close(result_q);
    mq_unlink(TASK_QUEUE);
    mq_unlink(RESULT_QUEUE);
    return 0;
}
