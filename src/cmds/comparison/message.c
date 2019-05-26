/**
 * @file
 * @brief
 *
 * @date 26.05.19
 * @author Egor Anikin
 */

#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <net/inetdevice.h>
#include <net/l3/route.h>
#include "environment.h"

#define PORT 1
#define ITER 100

static int var[ITER];
static int current = 0;

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

static void *task1(void *arg) {
	int time;
  
	while (current < ITER) {
		time = clock();
		send(sock_send, &time, sizeof(time), 0);
	}

	return NULL;
}

static void *task2(void *arg) {
	int time, cur_time;
	int ret;

	while (current < ITER) {
		ret = recv(sock_recv, &time, sizeof(time), 0);
		cur_time = clock();

		if (ret <= 0) {
			error("Message fail");
		}

		var[current] = cur_time - time;
		current++;

		if (current == ITER) {
			output("Full message test", var, ITER);
		}
	}

	return NULL;
}

int main(int argc, char **argv) {
	setup();

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	pthread_create(&th1, &attr, task1, NULL);
	pthread_create(&th2, &attr, task2, NULL);

	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	return 0;
}
