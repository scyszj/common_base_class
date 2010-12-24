#include "comm_uart.h"

int comm_uart::open(const char *dev, int *fd)
{
    *fd = open(dev, O_RDWR | O_NOCTTY | O_NDELAY, S_IRUSR | S_IWUSR);
    if (*fd == -1)
        return 0;
    else
        return -1;
}

int comm_uart::close(int fd)
{
    return (close(fd));
}

int comm_uart::set_speed(int fd, speed_t speed)
{
    int status;
    struct termios Opt; //定义了这样一个结构

    tcgetattr(fd, &Opt); //用来得到机器原端口的默认设置

    if (fd != -1) {//判断传进来是否相等
        tcflush(fd, TCIOFLUSH); //刷新输入输出缓冲
        cfsetispeed(&Opt, speed); //这里分别设置
        cfsetospeed(&Opt, speed);
        status = tcsetattr(fd, TCSANOW, &Opt); //这是立刻把bote rates设置真正写到串口中去

        if (status != 0)
            return -1;

        tcflush(fd, TCIOFLUSH); //同上
        return 0x0;
    }

    return -1;
}

int comm_uart::set_parity(int fd, int databits, int stopbits, char parity)
{
    struct termios options; //定义一个结构

    if (fd == -1)
        return fd;

    if (tcgetattr(fd, &options) != 0) //首先读取系统默认设置options中,必须
        return -1;

    options.c_cflag &= ~CSIZE; //这是设置c_cflag选项不按位数据位掩码
    switch (databits) { /*设置数据位数*/
    case 7:
        options.c_cflag |= CS7; //设置c_cflag选项数据位为7位
        break;
    case 8:
        options.c_cflag |= CS8; //设置c_cflag选项数据位为8位
        break;
    default:
        return -1;
    }

    //设置奇偶校验，c_cflag和c_iflag有效
    switch (parity) {
    case 'n':
    case 'N': //无校验 当然都不选
        options.c_cflag &= ~PARENB; /* Clear parity enable */
        options.c_iflag &= ~INPCK; /* Enable parity checking */
        break;
    case 'o':
    case 'O': //奇校验 其中PARENB校验位有效;PARODD奇校验;INPCK检查校验
        options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
        options.c_iflag |= INPCK; /* Disnable parity checking */
        break;
    case 'e':
    case 'E': //偶校验，奇校验不选就是偶校验了
        options.c_cflag |= PARENB; /* Enable parity */
        options.c_cflag &= ~PARODD; /* 转换为偶效验*/
        options.c_iflag |= INPCK; /* Disnable parity checking */
        break;
    default:
        return -1;
    }

    /* 设置停止位*/
    switch (stopbits) {
        //这是设置停止位数，影响的标志是c_cflag
    case 1:
        options.c_cflag &= ~CSTOPB; //不指明表示一位停止位
        break;
    case 2:
        options.c_cflag |= CSTOPB; //指明CSTOPB表示两位，只有两种可能
        break;
    default:
        return -1;
    }

    //这是设置输入是否进行校验
    if (parity != 'n' && parity != 'N')
        options.c_iflag |= INPCK;
    //这个地方是用来设置控制字符和超时参数的，一般默认即可。
    //稍微要注意的是c_cc数组的VSTART 和 VSTOP 元素被设定成DC1 和 DC3，代表ASCII 标准的XON和XOFF字符。
    options.c_cc[VTIME] = 10; // 1 seconds
    options.c_cc[VMIN] = 0;

    //所以如果在传输这两个字符的时候就传不过去，这时需要把软件流控制屏蔽 options.c_iflag &= ~(IXON | IXOFF | IXANY)
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    //不经处理直接发送
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    //屏蔽对回车换行处理
    options.c_oflag &= ~(INLCR | IGNCR | ICRNL);
    options.c_oflag &= ~(ONLCR | OCRNL);

    tcflush(fd, TCIFLUSH); //刷新和立刻写进去
    if (tcsetattr(fd, TCSANOW, &options) != 0)
        return -1;

    return 0;
}

int comm_uart::write(int fd, const char *data, int size)
{
    return (write(fd, data, size));
}
int comm_uart::read(int fd, char *data, int size)
{
    return (read(fd, data, size));
}
