#include "SERIAL.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

char read_buf[128];

int abrePorta(char* port) {
	int p = open(port, O_RDWR);
	return(p);
}

int configuraPorta(int port) {
	struct termios tty;
	
	if(tcgetattr(port, &tty) != 0) {
	  printf("\nErro %i de tcgetattr: %s\n", errno, strerror(errno));
	  return 1;
	}
	
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~CRTSCTS;
	tty.c_cflag |= CREAD | CLOCAL;
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO;
	tty.c_lflag &= ~ECHOE;
	tty.c_lflag &= ~ECHONL;
	tty.c_lflag &= ~ISIG;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
	tty.c_oflag &= ~OPOST;
	tty.c_oflag &= ~ONLCR;
	tty.c_cc[VTIME] = 1;
	tty.c_cc[VMIN] = 1;
	
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	
	if (tcsetattr(port, TCSANOW, &tty) != 0) {
	  printf("Erro %i de tcsetattr: %s\n", errno, strerror(errno));
	  return 1;
	}
	
	return 0;
	
}

void escrevePorta(int port, char* write_buf, int n) {
  write(port, write_buf, n);	
}

char* lePorta(int port) {
	memset(&read_buf, '\0', sizeof(read_buf));  
	int num_bytes = 0;
	char read_char = '\0';
	int pos = 0;
	do {
	  num_bytes = read(port, &read_char, 1 );
	  sprintf(&read_buf[pos], "%c", read_char);
	  pos += num_bytes;
	} while(read_char != '\r' && num_bytes > 0);
	if (num_bytes < 0) {
	  printf("\nErro de leitura: %s\n", strerror(errno));
	  return '\0';
	}
	return read_buf;	
}

void fechaPorta(int port) {
	close(port);
}
