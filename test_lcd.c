/*
 * test_lcd.c
 *	A test suit to test functionality of Nokia 5110 LCD.
 */
#include "lcd.h"
#include <sys/time.h>
#include <time.h>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>


int fd; /* port file descriptor */
char port[20] = "/dev/ttyO4"; /* port to connect to */
speed_t baud = B9600; /* baud rate */


int main()
{
    int ret;
    
    lcd_init();
    init_all_segments();
    sleep(3);
    
    //init();
    clrscr();
    //while(1) {
    lcd_printf("start");
    //}
    fd = open(port, O_RDWR); /* connect to port */
    
    /* set the other settings (in this case, 9600 8N1) */
    struct termios settings;
    tcgetattr(fd, &settings);
    
    cfsetospeed(&settings, baud); /* baud rate */
    cfsetispeed(&settings, baud);
    settings.c_cflag &= ~PARENB; /* no parity */
    settings.c_cflag &= ~CSTOPB; /* 1 stop bit */
    settings.c_cflag &= ~CSIZE;
    settings.c_cflag |= CS8 | CLOCAL; /* 8 bits */
    settings.c_lflag = ICANON; /* canonical mode */
    settings.c_oflag &= ~OPOST; /* raw output */
    
    tcsetattr(fd, TCSANOW, &settings); /* apply the settings */
    tcflush(fd, TCOFLUSH);
    
    /* — code to use the port here — */
    int i = 0;
    char buff[8];
    while(i< 10){
        write(fd,"KD\n",3);
        read(fd,&buff,8);
        printf("message : %s \n", &buff);
        lcd_printf("%s", &buff);
        i++;
    }
    
    close(fd); /* cleanup */
    
    lcd_deinit();
    return 0;
    //sendXbee("Test"); //The transmitting (TX) pin is connected to TTY04
}