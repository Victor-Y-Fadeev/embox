/**
 * @file
 * @brief
 *
 * @date 17.05.19
 * @author Egor Anikin
 */

#include <stdio.h>
#include <kernel/task.h>
#include <unistd.h>
#include <pthread.h>
#include <util/err.h>
#include <time.h>
#include "environment.h"

#define ITER 100

int t[3]= {0,0,0};

int curent[3] = {0,0,0};
int var[ITER][3];

pthread_t th1, th2, th3;


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
   
   thread_exit(NULL); 
}

int main(int argc, char **argv) { 
    int varFull[ITER * 3];

    pthread_create(&th1, NULL, Task, (void *) 0);
    pthread_create(&th2, NULL, Task, (void *) 1);
    pthread_create(&th3, NULL, Task, (void *) 2);

    for (int i = 0; i < ITER; i++) {
        for (int j = 0; j < 3; j++) {
            varFull[i * 3 + j] = var[i][j];
        }
	}

    output("Process", varFull, ITER * 3);

    return 0;
}
