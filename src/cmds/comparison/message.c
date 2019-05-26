/**
 * @file
 * @brief
 *
 * @date 26.05.19
 * @author Egor Anikin
 */

#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <net/inetdevice.h>
#include <net/l3/route.h>
#include <unistd.h>
#include <pthread.h>
#include <util/err.h>
#include <kernel/task.h>
#include "environment.h"

#define PORT 1
#define ITER 500

static int var[ITER];
static int current = 0;

static int curTime = 0;

static int sock_recv, sock_send;
pthread_t th1, th2;


static void setup(void) {
	// Setup device suite
	struct in_device *in_dev;
	in_dev = inetdev_get_loopback_dev();

	inetdev_set_addr(in_dev, htonl(INADDR_LOOPBACK));
	netdev_flag_up(in_dev->dev, IFF_UP);
	rt_add_route(in_dev->dev, ntohl(INADDR_LOOPBACK & ~1)
				, htonl(0xFF000000), 0, RTF_UP);

	// Setup receive socket
	struct sockaddr_in addr;
	sock_recv = socket(AF_INET, SOCK_DGRAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	bind(sock_recv, (struct sockaddr *) &addr, sizeof(addr));
	fcntl(sock_recv, F_SETFD, O_NONBLOCK);

	// Setup send socket
	sock_send = socket(AF_INET, SOCK_DGRAM, 0);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	connect(sock_send, (struct sockaddr *) &addr, sizeof(addr));
}

static inline void *Task1(void *arg) {
    int message = 0;
  
    while (current < ITER) 
    {
        message = clock();
        send(sock_send, &message, sizeof(message), 0);
    }

    thread_exit(NULL); //vTaskDelete(NULL);
}

static inline void *Task2(void *arg) {
    int message = 0;
    int ret = 0;

    while (current <= ITER) 
    {
        ret = recv(sock_recv, &message, sizeof(message), 0);
        curTime = clock();

        if (ret > 0 )
        {
			printf(" message = %d\n", message);
            var[current] = curTime - message;
            current++;

            if (current == ITER)
            {
                output("Full message test", var, ITER);
                thread_exit(NULL);
            }
		
            
        }
    }
    thread_exit(NULL);
}

int main(int argc, char **argv) {
	setup();
	
	pthread_create(&th1, NULL, Task1, NULL);
    pthread_create(&th2, NULL, Task2, NULL);

	return 0;
}
