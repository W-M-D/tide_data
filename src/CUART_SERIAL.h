#ifndef CUART_SERIAL_H
#define CUART_SERIAL_H
#include <deque>
#include <sys/poll.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
class CUART_SERIAL
{
    public:
        CUART_SERIAL(int);

        bool  serial_init(int);
        void serial_setup(int);
        int write_string(std::string data); // writes a string to uart port returns number of bytes written.
        int data_read (std::deque <uint8_t> & data_list); // reads data into a deque

        virtual ~CUART_SERIAL();
    protected:
    private:
    int v_time;
    int DEBUG_LEVEL;
    int tty_fd;
    struct termios tio;
    uint8_t data_in;
};

#endif // CUART_SERIAL_H
