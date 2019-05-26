/**
 * @file
 * @brief
 *
 * @date 17.05.19
 * @author Egor Anikin
 */

#include <stdio.h>
#include <kernel/task.h>
#include <time.h>
#include "environment.h"

#define ITER 100

int t[3]= {0,0,0};

int curent[3] = {0,0,0};
int var[ITER][3];


static void * Task(void *nuber){
    int n = (int)nuber;
    int t1, t2, cur_time;

    while(curent[n] < ITER) {
        t1 = t[(n + 1) % 3];
        t2 = t[(n + 2) % 3];
        cur_time = clock();

        if ((t1 != 0) && (t2 != 0))
        {
            if (t1 > t2)
            {
                var[curent[n]][n] = cur_time - t2;
            }
            else
            {
                var[curent[n]][n] = cur_time - t1;
            }
            curent[n]++;
        }
        t[n] = clock();
    }
   
   return NULL; 
}

int main(int argc, char **argv) { 
    int tid1, tid2, tid3;
    int varFull[ITER * 3];

    tid1 = new_task("Task 1", Task, (void *) 0);
    tid2 = new_task("Task 2", Task, (void *) 1);
    tid3 = new_task("Task 3", Task, (void *) 2);

    task_waitpid(tid1);
    task_waitpid(tid2);
    task_waitpid(tid3);

    for (int i = 0; i < ITER; i++) {
        for (int j = 0; j < 3; j++) {
            varFull[i * 3 + j] = var[i][j];
        }
	}

    output("Process", varFull, ITER * 3);

    return 0;
}
