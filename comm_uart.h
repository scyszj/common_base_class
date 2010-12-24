#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#ifndef COMM_UART_H_
#define COMM_UART_H_

class comm_uart {
public:
	int open(const char *dev, int *fd);
	int close(int fd);

	int set_speed(int fd, speed_t speed);
	int set_parity(int fd, int databit, int stopbit, char parity);

	int write(int fd,const char *data,int size);
	int read(int fd,char *data,int size);
};

#endif /* COMM_UART_H_ */

