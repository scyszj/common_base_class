#include <comm_net.h>
#include <comm_base.h>

using namespace std;
int comm_net_link::setup_tcp_server(in_port_t port)
{
    int listensock; //定义监听socket，一个用于监听
    listensock = socket(AF_INET, SOCK_STREAM, 0); //创建一个套接字，用于监听
    if (listensock != -1) {
        printf("创建sock成功!");
    } else {
        printf("创建sock失败!");
        return 0x1;
    }

    struct sockaddr_in serveraddr; //定义网络套接字地址结构
    bzero(&serveraddr, sizeof(sockaddr_in)); //地址结构清零

    serveraddr.sin_family = AF_INET; //指定使用的通讯协议族
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); //指定接受任何连接，因为服务器不知道谁会要求连接
    serveraddr.sin_port = htons(port); //指定监听的端口
    int on = 1;
    setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (bind(listensock, (sockaddr *) &serveraddr, sizeof(sockaddr_in)) == 0) { //给套接口邦定地址
        printf("bind成功!");
    } else {
        printf("bind失败!");
        return 0x2;
    }

    if (listen(listensock, 32) == 0) { //开始监听
        printf("listen成功!");
    } else {
        printf("listen失败!");
        return 0x3;
    }

    //通讯sockaddr
    struct sockaddr clientaddr;
    socklen_t addrlen;
    int connsock;
    clientaddr.sa_family = AF_INET;
    addrlen = sizeof(sockaddr);
    connsock = accept(listensock, (sockaddr *) &clientaddr, &addrlen); //建立通讯的套接字，accept函数，等待客户端程序使用connect函数的连接
    if (connsock == -1) {
        printf("accept连接错误!");
        close(listensock);
        return 0x4;
    } else {
        printf("accept连接成功");

        //设置Sock超时
        struct timeval timeout;
        timeout.tv_sec = 60; //1分钟收不到数据就断开
        timeout.tv_usec = 0;
        setsockopt(connsock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        setsockopt(connsock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

        while (1) {
            send(connsock, "连接到服务器成功！", 18, 0);//向客户端发送数据
            comm_base cb;
            cb.msleep(10);

            char recvbuff[512];
            int rcnt = recv(connsock, recvbuff, sizeof(recvbuff), 0);
            recvbuff[rcnt] = '\0';
            if ((strstr(recvbuff, "USRCMDENDNET") != 0 && rcnt == 12) || rcnt <= 0) { //正常断开或命令断开或意外断开
                break;
            }
        }

        close(connsock); //关闭通讯套接字
        close(listensock); //关闭监听套接字
    }

    return 0x0;
}

int comm_net_link::connect_tcp_server(struct sockaddr_in serveraddr)
{
    int sockfd;
    char recvbuff[128];

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //创建套接字

    bzero(&serveraddr, sizeof(sockaddr_in));
    serveraddr.sin_family = AF_INET; //指定使用IP讯协议族
    serveraddr.sin_port = htons(5000);//指定要连接的服务器的端口

    connect(sockfd, (sockaddr *) &serveraddr, sizeof(serveraddr));//连接服务器

    bzero(recvbuff, 128);
    recv(sockfd, recvbuff, sizeof(recvbuff), 0); //接收服务器的数据

    printf("%s", recvbuff); //打印接收到的数据
    close(sockfd); //关闭套接字

    return 0x0;
}

int init_hotplug_sock(void)
{
	const int buffersize = UEVENT_BUFFER_SIZE;
	int ret;

	struct sockaddr_nl snl;
	bzero(&snl, sizeof(struct sockaddr_nl));
	snl.nl_family = AF_NETLINK;
	snl.nl_pid = getpid();
	snl.nl_groups = 1;

	int sockfd = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (sockfd == -1) {
		perror("socket");
		return -1;
	}
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &buffersize, sizeof(buffersize));

	ret = bind(sockfd, (struct sockaddr *) &snl, sizeof(struct sockaddr_nl));
	if (ret < 0) {
		perror("bind");
		close(sockfd);
		return -1;
	}

	return sockfd;
}
