#include <stdio.h>

#include "includes.h"
#include "asm-arm/timer.h"
#include "asm-arm/irq.h"

#define  TASK_STK_SIZE 512
#define  N_TASKS 8

OS_STK TaskStk[N_TASKS][TASK_STK_SIZE];

OS_STK TaskStartStk[TASK_STK_SIZE];
char TaskData[N_TASKS];

void TaskStart(void *data);
void Task(void *data);

void Task(void *data)
{
	char task = *(char *)data;

    for (;;) {
		printf("Task %d\n", (int)task);
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

void TaskStart(void *data)
{
    int i;

    init_time();
    asm volatile ( "cpsie if" );
    OSStatInit(); /* Initialize uC/OS-II's statistics */


    for (i = 0; i < N_TASKS; i++) {
        TaskData[i] = i;
        OSTaskCreate(Task, (void *) &TaskData[i],
                &TaskStk[i][TASK_STK_SIZE - 1], 11 + i);
    }

    printf("OSStatInit\n");

    while (1) {
        OSCtxSwCtr = 0;
        OSTimeDlyHMSM(0, 0, 30, 0);
    }

}


int main(void)
{

    gic_init();
    OSInit();
    init_IRQ();
    printf("TaskStart\n");

    printf("\n Init Done\n");

    /*Initialize uC/OS-II*/
    OSTaskCreate(TaskStart, (void *) 0, &TaskStartStk[TASK_STK_SIZE - 1], 7);

    printf("\n Start multitasking \n");
    OSStart(); /* Start multitasking */

    return 0;
}

