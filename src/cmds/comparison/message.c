/**
 * @file
 * @brief
 *
 * @date 26.05.19
 * @author Victor Y. Fadeev
 */

#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <net/inetdevice.h>
#include <net/l3/route.h>
#include "environment.h"

#define PORT 1

static int sock_recv, sock_send;


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

int main(int argc, char **argv) {
	setup();
	
	int buf[3];
	send(sock_send, "a", 1, 0);
	recv(sock_recv, buf, 2, 0);

	printf("buf[0] = %c\n", buf[0]);

	return 0;
}
