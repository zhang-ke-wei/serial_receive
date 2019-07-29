#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

int set_opt(int,int,int,char,int);

int main()
{
    char *serial_r = "/dev/ttySAC3";
    char *out = "please input��";
    char buffer[1024];
    int rd_sta;
    int fd;//�ļ����
    memset(buffer,0, sizeof(buffer));
    if((fd = open(serial_r,O_RDWR|O_NOCTTY)) < 0)
    {
        printf("open %s failed!\n",serial_r);
    }
    else
    {
        set_opt(fd,115200,8,'N',1);
        write(fd,out,strlen(out));
        /*while (1)
        {
            while (rd_sta = read(fd,buffer,1024)>0)
            {
                buffer[rd_sta+1] = '\0';
                printf("%s\n",buffer);
                write(fd,buffer,strlen(buffer));
                memset(buffer,0, sizeof(buffer));
                rd_sta = 0;
            }
        }*/
        while(1){
            while((rd_sta = read(fd, buffer, 512))>0){
                buffer[rd_sta+1] = '\0';
                write(fd,buffer,strlen(buffer));
                memset(buffer, 0, strlen(buffer));
                rd_sta = 0;
            }
        }
    }
    return 0;
}

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;
    if  ( tcgetattr( fd,&oldtio)  !=  0) {
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );
    newtio.c_cflag  |=  CLOCAL | CREAD;
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch( nEvent )
    {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        case 460800:
            cfsetispeed(&newtio, B460800);
            cfsetospeed(&newtio, B460800);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }
    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
        newtio.c_cflag |=  CSTOPB;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);
    if((tcsetattr(fd,TCSANOW,&newtio))!=0)
    {
        perror("com set error");
        return -1;
    }

    //	printf("set done!\n\r");
    return 0;
}