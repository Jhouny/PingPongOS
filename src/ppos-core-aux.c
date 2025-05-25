#include "ppos.h"
#include "ppos-core-globals.h"
#include "ppos-disk-manager.h"


// ****************************************************************************
// Adicione TUDO O QUE FOR NECESSARIO para realizar o seu trabalho
// Coloque as suas modificações aqui, 
// p.ex. includes, defines variáveis, 
// estruturas e funções
// ****************************************************************************
#include <signal.h>
#include <sys/time.h>

struct sigaction action ;
struct itimerval timer ;

const unsigned int max_quantum = 1;     // Quantum time in ticks
unsigned int quantum = max_quantum;      // Quantum time in milliseconds
unsigned int _systemTime = 0;   // Global system time in ticks
unsigned int prio_alpha = -1;   // Priority increment value

// Tick handler
void tick_handler (int signum) {
    _systemTime++; // Increment the system time
    quantum--;
    if (quantum == 0) {
        quantum = max_quantum; // Reset the quantum
        if (!(taskExec->systemTask)) {  // If the current task is not a system task
            task_yield(); // Yield the current task
        }
    }
}

// Function to get the sign of a number
// Returns 1 if positive, -1 if negative, and 0 if zero
int sign(int x) {
    return (x > 0) - (x < 0);
}

#define MAX_TASKS 1000

task_metrics_t metrics[MAX_TASKS];

task_t* scheduler() {
    // Iterate over the ready queue and find the task with the highest priority and increment all non-executing tasks' priorities
    task_t* highestPriorityTask = NULL;
    task_t* currTask = readyQueue;       // Task to be analyzed
    task_t* startTask = readyQueue;         // Start of the queue

    if (currTask == taskMain) {
        return taskMain; // If the main task is in the queue, return it
    }

    // Iterate over the ready queue and increase the dynamic priority of all tasks 
    // as well as find the task with the highest priority (smallest prioDyn)
    do {
        if (currTask->id >= 2) {
            currTask->prioDyn += prio_alpha;
            
            // Truncate the dynamic priority to the defined range
            if (abs(currTask->prioDyn) > 20) { currTask->prioDyn = sign(currTask->prioDyn) * 20; }
        }

        // Update the highest priority task
        if (highestPriorityTask == NULL || currTask->prioDyn <= highestPriorityTask->prioDyn) {
            highestPriorityTask = currTask;
        }

        currTask = currTask->next;
    } while (currTask != startTask);

    highestPriorityTask->prioDyn = highestPriorityTask->prio; // Reset the dynamic priority of the highest priority task

    return highestPriorityTask;
}

unsigned int systime () {
    return _systemTime;
}

void task_setprio (task_t *task, int prio) {
    /* 
        Set the STATIC priority of the task to the given value (truncates it between -20 and 20)
        If the task is NULL, set the priority of the currently executing task
    */ 
    if (task == NULL) {
        task = taskExec;
    }

    // Truncate the priority to defined range
    if (abs(prio) > 20) { prio = sign(prio) * 20; }
    
    task->prio = prio;
    task->prioDyn = task->prio; // Set the dynamic priority to the static one
}

int task_getprio (task_t *task) {
    /*
        Get the priority of the task. If the task is NULL, get the priority of the currently executing task
        The function returns the STATIC priority of the task
    */
    if (task == NULL) {
        task = taskExec;
    }

    return task->prio;
}


void before_ppos_init () {
#ifdef DEBUG
    printf("\ninit - BEFORE");
#endif
}

void after_ppos_init () {
#ifdef DEBUG
    printf("\ninit - AFTER");
#endif
    // Configure the timer and signal handler
    action.sa_handler = tick_handler;
    sigemptyset (&action.sa_mask);
    if (sigaction (SIGALRM, &action, NULL) < 0) {
        perror ("Sigaction error: ");
        exit (1);
    }

    // Set the timer to trigger every 1 millisecond
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 1000;

    // Set the timer to ITIMER_REAL
    if (setitimer (ITIMER_REAL, &timer, NULL) < 0) {
        perror ("Setitimer error: ");
        exit (1);
    }
}

void before_task_create (task_t *task ) {
#ifdef DEBUG
    printf("\ntask_create - BEFORE - [%d]", task->id);
#endif
}

void after_task_create (task_t *task ) {
#ifdef DEBUG
    printf("\ntask_create - AFTER - [%d]", task->id);
#endif
    if (task != NULL && task->id < MAX_TASKS) {
        unsigned int now = systime();
        metrics[task->id].start_time = now;
        metrics[task->id].processor_time = 0;
        metrics[task->id].activations = 0;
        metrics[task->id].last_proc_start = now;
        metrics[task->id].end_time = 0;
        printf("DEBUG: métricas inicializadas para id=%d\n", task->id);
    }
    printf("task->id: %d\n", task ? task->id : -1);

    // Initialize task properties
    task->prio = 0; // Set the default static priority to 0
    task->prioDyn = task->prio; // Set the dynamic priority to the static one
    task->systemTask = 0; // Set the task as a user task
}

void before_task_exit () {
#ifdef DEBUG
    printf("\ntask_exit - BEFORE - [%d]", taskExec->id);
#endif
    if (taskExec != NULL && taskExec->id < MAX_TASKS) {
        unsigned int now = systime();
        if (metrics[taskExec->id].last_proc_start != 0)
            metrics[taskExec->id].processor_time += (now - metrics[taskExec->id].last_proc_start);
        metrics[taskExec->id].end_time = now;
        printf("Task %d exit: execution time %u ms, processor time %u ms, %u activations\n",
            taskExec->id,
            metrics[taskExec->id].end_time - metrics[taskExec->id].start_time,
            metrics[taskExec->id].processor_time,
            metrics[taskExec->id].activations);
    }
}

void after_task_exit () {
#ifdef DEBUG
    printf("\ntask_exit - AFTER- [%d]", taskExec->id);
#endif
    
}

void before_task_switch ( task_t *task ) {
#ifdef DEBUG
    printf("\ntask_switch - BEFORE - [%d -> %d]", taskExec->id, task->id);
#endif
    if (taskExec != NULL && taskExec->id < MAX_TASKS) {
        unsigned int now = systime();
        if (metrics[taskExec->id].last_proc_start != 0)
            metrics[taskExec->id].processor_time += (now - metrics[taskExec->id].last_proc_start);
    }
    if (task != NULL && task->id < MAX_TASKS) {
        metrics[task->id].activations++;
        metrics[task->id].last_proc_start = systime();
    }
}

void after_task_switch ( task_t *task ) {
#ifdef DEBUG
    printf("\ntask_switch - AFTER - [%d -> %d]", taskExec->id, task->id);
#endif
}

void before_task_yield () {
#ifdef DEBUG
    printf("\ntask_yield - BEFORE - [%d]", taskExec->id);
#endif
}
void after_task_yield () {
#ifdef DEBUG
    printf("\ntask_yield - AFTER - [%d]", taskExec->id);
#endif
}


void before_task_suspend( task_t *task ) {
#ifdef DEBUG
    printf("\ntask_suspend - BEFORE - [%d]", task->id);
#endif
}

void after_task_suspend( task_t *task ) {
#ifdef DEBUG
    printf("\ntask_suspend - AFTER - [%d]", task->id);
#endif
}

void before_task_resume(task_t *task) {
#ifdef DEBUG
    printf("\ntask_resume - BEFORE - [%d]", task->id);
#endif
}

void after_task_resume(task_t *task) {
#ifdef DEBUG
    printf("\ntask_resume - AFTER - [%d]", task->id);
#endif
}

void before_task_sleep () {
#ifdef DEBUG
    printf("\ntask_sleep - BEFORE - [%d]", taskExec->id);
#endif
}

void after_task_sleep () {
#ifdef DEBUG
    printf("\ntask_sleep - AFTER - [%d]", taskExec->id);
#endif
}

int before_task_join (task_t *task) {
#ifdef DEBUG
    printf("\ntask_join - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_task_join (task_t *task) {
#ifdef DEBUG
    printf("\ntask_join - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}


int before_sem_create (semaphore_t *s, int value) {
#ifdef DEBUG
    printf("\nsem_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_create (semaphore_t *s, int value) {
#ifdef DEBUG
    printf("\nsem_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_down (semaphore_t *s) {
#ifdef DEBUG
    printf("\nsem_down - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_down (semaphore_t *s) {
#ifdef DEBUG
    printf("\nsem_down - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_up (semaphore_t *s) {
#ifdef DEBUG
    printf("\nsem_up - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_up (semaphore_t *s) {
#ifdef DEBUG
    printf("\nsem_up - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_destroy (semaphore_t *s) {
#ifdef DEBUG
    printf("\nsem_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_destroy (semaphore_t *s) {
#ifdef DEBUG
    printf("\nsem_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_create (mutex_t *m) {
#ifdef DEBUG
    printf("\nmutex_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_create (mutex_t *m) {
#ifdef DEBUG
    printf("\nmutex_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_lock (mutex_t *m) {
#ifdef DEBUG
    printf("\nmutex_lock - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_lock (mutex_t *m) {
#ifdef DEBUG
    printf("\nmutex_lock - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_unlock (mutex_t *m) {
#ifdef DEBUG
    printf("\nmutex_unlock - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_unlock (mutex_t *m) {
#ifdef DEBUG
    printf("\nmutex_unlock - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_destroy (mutex_t *m) {
#ifdef DEBUG
    printf("\nmutex_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_destroy (mutex_t *m) {
#ifdef DEBUG
    printf("\nmutex_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_create (barrier_t *b, int N) {
#ifdef DEBUG
    printf("\nbarrier_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_create (barrier_t *b, int N) {
#ifdef DEBUG
    printf("\nbarrier_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_join (barrier_t *b) {
#ifdef DEBUG
    printf("\nbarrier_join - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_join (barrier_t *b) {
#ifdef DEBUG
    printf("\nbarrier_join - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_destroy (barrier_t *b) {
#ifdef DEBUG
    printf("\nbarrier_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_destroy (barrier_t *b) {
#ifdef DEBUG
    printf("\nbarrier_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_create (mqueue_t *queue, int max, int size) {
#ifdef DEBUG
    printf("\nmqueue_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_create (mqueue_t *queue, int max, int size) {
#ifdef DEBUG
    printf("\nmqueue_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_send (mqueue_t *queue, void *msg) {
#ifdef DEBUG
    printf("\nmqueue_send - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_send (mqueue_t *queue, void *msg) {
#ifdef DEBUG
    printf("\nmqueue_send - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_recv (mqueue_t *queue, void *msg) {
#ifdef DEBUG
    printf("\nmqueue_recv - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_recv (mqueue_t *queue, void *msg) {
#ifdef DEBUG
    printf("\nmqueue_recv - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_destroy (mqueue_t *queue) {
#ifdef DEBUG
    printf("\nmqueue_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_destroy (mqueue_t *queue) {
#ifdef DEBUG
    printf("\nmqueue_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_msgs (mqueue_t *queue) {
#ifdef DEBUG
    printf("\nmqueue_msgs - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_msgs (mqueue_t *queue) {
#ifdef DEBUG
    printf("\nmqueue_msgs - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}
