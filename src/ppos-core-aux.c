#include "ppos.h"
#include "ppos-core-globals.h"
#include "ppos-disk-manager.h"


// ****************************************************************************
// Adicione TUDO O QUE FOR NECESSARIO para realizar o seu trabalho
// Coloque as suas modificações aqui, 
// p.ex. includes, defines variáveis, 
// estruturas e funções
// ****************************************************************************

unsigned int _systemTime = 0; // Global system time in ticks

task_t* scheduler() {
    // put your customization here
    printf("scheduler\n");
    printf("taskExec->id: %d\n", taskExec->id);
    return readyQueue;
}

unsigned int systime () {
    // put your customization here
    printf("systime\n");
    return _systemTime;
}


void before_ppos_init () {
    // put your customization here
    printf("before_ppos_init\n");
#ifdef DEBUG
    printf("\ninit - BEFORE");
#endif
}

void after_ppos_init () {
    // put your customization here
    printf("after_ppos_init\n");
#ifdef DEBUG
    printf("\ninit - AFTER");
#endif
}

void before_task_create (task_t *task ) {
    // put your customization here
    printf("before_task_create\n");
    printf("task->id: %d\n", task->id);
#ifdef DEBUG
    printf("\ntask_create - BEFORE - [%d]", task->id);
#endif
}

// Função auxiliar para logar ponteiros de uma task
void log_task_ptrs(const char* label, task_t* t) {
    if (t == NULL) {
        printf("%s: ponteiro da task é NULL\n", label);
        return;
    }
    printf("%s: endereço=%p, id=%d, prev=%p, next=%p, context=%p, queue=%p, joinQueue=%p, custom_data=%p\n",
        label, (void*)t, t->id, (void*)t->prev, (void*)t->next, (void*)&t->context, (void*)t->queue, (void*)t->joinQueue, t->custom_data);
}

void after_task_create (task_t *task ) {
    printf("DEBUG: Entrou em after_task_create\n");
    log_task_ptrs("after_task_create - task", task);
    if (task != NULL) {
        // Inicializa os campos de métricas
        task->start_time = systime();
        task->processor_time = 0;
        task->activations = 0;
        task->last_proc_start = 0;
        task->end_time = 0;
        printf("DEBUG: task inicializado, id=%d\n", task->id);
    } else {
        printf("DEBUG: task é NULL em after_task_create\n");
    }
    printf("after_task_create\n");
    printf("task->id: %d\n", task ? task->id : -1);
#ifdef DEBUG
    printf("\ntask_create - AFTER - [%d]", task ? task->id : -1);
#endif
}

void before_task_exit () {
    printf("DEBUG: Entrou em before_task_exit\n");
    log_task_ptrs("before_task_exit - taskExec", taskExec);
    if (taskExec != NULL) {
        unsigned int now = systime();
        printf("DEBUG: taskExec não é NULL, id=%d\n", taskExec->id);
        printf("DEBUG: now=%u, last_proc_start=%u\n", now, taskExec->last_proc_start);
        taskExec->processor_time += (now - taskExec->last_proc_start);
        taskExec->end_time = now;
        printf("Task %d exit: execution time %u ms, processor time %u ms, %u activations\n",
            taskExec->id,
            taskExec->end_time - taskExec->start_time,
            taskExec->processor_time,
            taskExec->activations);
    } else {
        printf("DEBUG: taskExec é NULL em before_task_exit\n");
    }
    printf("before_task_exit\n");
    printf("taskExec->id: %d\n", taskExec ? taskExec->id : -1);
#ifdef DEBUG
    printf("\ntask_exit - BEFORE - [%d]", taskExec ? taskExec->id : -1);
#endif
}

void after_task_exit () {
    // put your customization here
    printf("DEBUG: Entrou em after_task_exit\n");
    log_task_ptrs("after_task_exit - taskExec", taskExec);
    printf("after_task_exit\n");
    printf("taskExec->id: %d\n", taskExec ? taskExec->id : -1);
#ifdef DEBUG
    printf("\ntask_exit - AFTER- [%d]", taskExec ? taskExec->id : -1);
#endif
}

void before_task_switch ( task_t *task ) {
    printf("DEBUG: Entrou em before_task_switch\n");
    log_task_ptrs("before_task_switch - taskExec", taskExec);
    log_task_ptrs("before_task_switch - task", task);
    if (taskExec != NULL) {
        printf("DEBUG: taskExec não é NULL, id=%d\n", taskExec->id);
        unsigned int now = systime();
        printf("DEBUG: now=%u, last_proc_start=%u\n", now, taskExec->last_proc_start);
        taskExec->processor_time += (now - taskExec->last_proc_start);
        printf("DEBUG: processor_time atualizado: %u\n", taskExec->processor_time);
    } else {
        printf("DEBUG: taskExec é NULL\n");
    }
    if (task != NULL) {
        printf("DEBUG: task não é NULL, id=%d\n", task->id);
        task->activations++;
        task->last_proc_start = systime();
        printf("DEBUG: ativations=%u, last_proc_start=%u\n", task->activations, task->last_proc_start);
    } else {
        printf("DEBUG: task é NULL\n");
    }
    printf("DEBUG: Saindo de before_task_switch\n");
    printf("before_task_switch\n");
    printf("taskExec->id: %d\n", taskExec ? taskExec->id : -1);
#ifdef DEBUG
    printf("\ntask_switch - BEFORE - [%d -> %d]", taskExec ? taskExec->id : -1, task ? task->id : -1);
#endif
}

void after_task_switch ( task_t *task ) {
    printf("DEBUG: Entrou em after_task_switch\n");
    log_task_ptrs("after_task_switch - taskExec", taskExec);
    log_task_ptrs("after_task_switch - task", task);
    if (taskExec != NULL)
        printf("DEBUG: taskExec não é NULL, id=%d\n", taskExec->id);
    else
        printf("DEBUG: taskExec é NULL\n");
    if (task != NULL)
        printf("DEBUG: task não é NULL, id=%d\n", task->id);
    else
        printf("DEBUG: task é NULL\n");
    printf("after_task_switch\n");
    printf("taskExec->id: %d\n", taskExec ? taskExec->id : -1);
#ifdef DEBUG
    printf("\ntask_switch - AFTER - [%d -> %d]", taskExec ? taskExec->id : -1, task ? task->id : -1);
#endif
}

void before_task_yield () {
    // put your customization here
    printf("before_task_yield\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_yield - BEFORE - [%d]", taskExec->id);
#endif
}
void after_task_yield () {
    // put your customization here
    printf("after_task_yield\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_yield - AFTER - [%d]", taskExec->id);
#endif
}


void before_task_suspend( task_t *task ) {
    // put your customization here
    printf("before_task_suspend\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_suspend - BEFORE - [%d]", task->id);
#endif
}

void after_task_suspend( task_t *task ) {
    // put your customization here
    printf("after_task_suspend\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_suspend - AFTER - [%d]", task->id);
#endif
}

void before_task_resume(task_t *task) {
    // put your customization here
    printf("before_task_resume\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_resume - BEFORE - [%d]", task->id);
#endif
}

void after_task_resume(task_t *task) {
    // put your customization here
    printf("after_task_resume\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_resume - AFTER - [%d]", task->id);
#endif
}

void before_task_sleep () {
    // put your customization here
    printf("before_task_sleep\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_sleep - BEFORE - [%d]", taskExec->id);
#endif
}

void after_task_sleep () {
    // put your customization here
    printf("after_task_sleep\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_sleep - AFTER - [%d]", taskExec->id);
#endif
}

int before_task_join (task_t *task) {
    // put your customization here
    printf("before_task_join\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_join - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_task_join (task_t *task) {
    // put your customization here
    printf("after_task_join\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\ntask_join - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}


int before_sem_create (semaphore_t *s, int value) {
    // put your customization here
    printf("before_sem_create\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nsem_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_create (semaphore_t *s, int value) {
    // put your customization here
    printf("after_sem_create\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nsem_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_down (semaphore_t *s) {
    // put your customization here
    printf("before_sem_down\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nsem_down - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_down (semaphore_t *s) {
    // put your customization here
    printf("after_sem_down\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nsem_down - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_up (semaphore_t *s) {
    // put your customization here
    printf("before_sem_up\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nsem_up - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_up (semaphore_t *s) {
    // put your customization here
    printf("after_sem_up\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nsem_up - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_sem_destroy (semaphore_t *s) {
    // put your customization here
    printf("before_sem_destroy\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nsem_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_sem_destroy (semaphore_t *s) {
    // put your customization here
    printf("after_sem_destroy\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nsem_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_create (mutex_t *m) {
    // put your customization here
    printf("before_mutex_create\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmutex_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_create (mutex_t *m) {
    // put your customization here
    printf("after_mutex_create\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmutex_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_lock (mutex_t *m) {
    // put your customization here
    printf("before_mutex_lock\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmutex_lock - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_lock (mutex_t *m) {
    // put your customization here
    printf("after_mutex_lock\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmutex_lock - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_unlock (mutex_t *m) {
    // put your customization here
    printf("before_mutex_unlock\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmutex_unlock - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_unlock (mutex_t *m) {
    // put your customization here
#ifdef DEBUG
    printf("\nmutex_unlock - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mutex_destroy (mutex_t *m) {
    // put your customization here
    printf("before_mutex_destroy\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmutex_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mutex_destroy (mutex_t *m) {
    // put your customization here
    printf("after_mutex_destroy\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmutex_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_create (barrier_t *b, int N) {
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_create (barrier_t *b, int N) {
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_join (barrier_t *b) {
    // put your customization here
    printf("before_barrier_join\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nbarrier_join - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_join (barrier_t *b) {
    // put your customization here
#ifdef DEBUG
    printf("\nbarrier_join - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_barrier_destroy (barrier_t *b) {
    // put your customization here
    printf("before_barrier_destroy\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nbarrier_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_barrier_destroy (barrier_t *b) {
    // put your customization here
    printf("after_barrier_destroy\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nbarrier_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_create (mqueue_t *queue, int max, int size) {
    // put your customization here
    printf("before_mqueue_create\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmqueue_create - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_create (mqueue_t *queue, int max, int size) {
    // put your customization here
    printf("after_mqueue_create\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmqueue_create - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_send (mqueue_t *queue, void *msg) {
    // put your customization here
    printf("before_mqueue_send\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmqueue_send - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_send (mqueue_t *queue, void *msg) {
    // put your customization here
    printf("after_mqueue_send\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmqueue_send - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_recv (mqueue_t *queue, void *msg) {
    // put your customization here
    printf("before_mqueue_recv\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmqueue_recv - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_recv (mqueue_t *queue, void *msg) {
    // put your customization here
#ifdef DEBUG
    printf("\nmqueue_recv - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_destroy (mqueue_t *queue) {
    // put your customization here
    printf("before_mqueue_destroy\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmqueue_destroy - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_destroy (mqueue_t *queue) {
    // put your customization here
    printf("after_mqueue_destroy\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmqueue_destroy - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}

int before_mqueue_msgs (mqueue_t *queue) {
    // put your customization here
    printf("before_mqueue_msgs\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmqueue_msgs - BEFORE - [%d]", taskExec->id);
#endif
    return 0;
}

int after_mqueue_msgs (mqueue_t *queue) {
    // put your customization here
    printf("after_mqueue_msgs\n");
    printf("taskExec->id: %d\n", taskExec->id);
#ifdef DEBUG
    printf("\nmqueue_msgs - AFTER - [%d]", taskExec->id);
#endif
    return 0;
}
