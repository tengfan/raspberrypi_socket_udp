#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

int file;
int adapter_nr = 1;
char filename[20];
char buff[3];
char buff_rcv[3];

int main() 
{
	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	file = open(filename, O_RDWR);
	if (file < 0) {
			printf("file error");
			exit(1);
	}

	int addr = 0x08;

	if (ioctl(file, I2C_SLAVE, addr) < 0) {
			printf("error io");
			exit(1);
	}

	while(1) {
			// write
			getchar();
			buff[0] = 'r';
			buff[1] = 'a';
			buff[2] = 's';
			if (write(file, buff, 3) != 3) {
					printf("error write");
			}

			// Read
			getchar();
			if (read(file,buff_rcv, 2) != 2) {
					printf("error read");
			} else {
					printf("%c %c", buff_rcv[0], buff_rcv[1]);
			}
	}
	return 0;
}

