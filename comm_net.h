#ifndef _COMM_NET_H_
#define _COMM_NET_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <arpa/inet.h>


/////////////////////////////////////////////////////////

#define UEVENT_BUFFER_SIZE 2048

//////////////////////////////////////////////////////////

class comm_net_link
{
private:
    comm_net_link() {};
    ~comm_net_link() {};

public:
    int setup_tcp_server(in_port_t port);
    int connect_tcp_server(struct sockaddr_in serveraddr);
    int init_hotplug_sock(void);
};

#endif /* _COMM_NET_H_ */
