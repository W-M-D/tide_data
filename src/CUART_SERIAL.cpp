#include "../include/CUART_SERIAL.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
static const char* portName = "/dev/ttyAMA0";

CUART_SERIAL::CUART_SERIAL(int debug_level)
{
    DEBUG_LEVEL = debug_level;
    if(DEBUG_LEVEL >= 1)
    {

        printf("serial port %d \n\n",tty_fd);

    }
    while(!serial_init(9600))
    {
        sleep(60);
    }

}


bool CUART_SERIAL::serial_init(int baud)
{
    if(tty_fd == 3)
    {
        close(tty_fd);
        usleep( 1 * 1000 );

        if(DEBUG_LEVEL >= 3)
        {
            printf(" reset TTY_FD = %d",tty_fd);
        }
    }
    else
    {
        if(DEBUG_LEVEL >= 3)
        {
            printf("TTY_FD = %d",tty_fd);
        }
    }

    tcgetattr(tty_fd, &tio);
    tcflush(tty_fd, TCIOFLUSH);

    fcntl(STDIN_FILENO, F_SETFL, 0);       // make the reads non-blocking
    memset(&tio,0,sizeof(tio));

    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_cflag &= ~CSIZE;
    tio.c_cflag |= CS8;
    tio.c_cflag &= ~PARENB;// no parity
    tio.c_cflag &= ~CSTOPB;// one stop bit

    tio.c_lflag=0;

    tio.c_cc[VMIN]=40;
    tio.c_cc[VTIME]=10;



    tty_fd=open(portName, O_RDWR );

    if (baud == 9600)
    {
        cfsetospeed(&tio,B9600);            // 115200 baud
        cfsetispeed(&tio,B9600);            // 115200 baud
        usleep( 3 * 1000 );
    }

    if (baud == 38400)
    {
        cfsetospeed(&tio,B38400);            // 115200 baud
        cfsetispeed(&tio,B38400);            // 115200 baud

        usleep( 3 * 1000 );
    }

    tcsetattr(tty_fd,TCSANOW,&tio);
    tcflush(tty_fd, TCIOFLUSH);
    fcntl(tty_fd, F_SETFL, 0);

    usleep( 1 * 1000 );

    return true;
}


int CUART_SERIAL::data_read (std::deque <uint8_t> & data_list)
{
    struct pollfd fds[1];
    fds[0].fd = tty_fd;
    fds[0].events = POLLIN ;
    int pollrc = poll( fds, 1,10);
    if (pollrc < 0)
    {
        perror("poll");
    }
    else if( pollrc > 0)
    {
        if( fds[0].revents )
        {
            if( POLLIN)
            {
                int bytes_avail = 0;
                ioctl(tty_fd, FIONREAD, &bytes_avail);
                for(int i = 0; i < bytes_avail; i++)
                {
                    uint8_t read_byte = 0;
                    read(tty_fd, &read_byte,1);
                    data_list.emplace_back(read_byte);
                }
            }
        }
    }
    return -1;
}


int CUART_SERIAL::write_string(std::string data)
{
    lseek(tty_fd, 0, SEEK_SET);

    for(unsigned int i =0; i < data.length(); i++)
    {
        char c = data.at(i);

        if(POLLOUT)
        {

            write(tty_fd,&c,sizeof(char));

            if(DEBUG_LEVEL >= 3)
            {

                printf("0x%X ",data.at(i));

            }
        }
        else
        {

            i--;

        }
    }

}


CUART_SERIAL::~CUART_SERIAL()
{
    //dtor
}
