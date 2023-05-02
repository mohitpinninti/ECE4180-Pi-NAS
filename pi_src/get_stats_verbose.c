#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	int fd;
	char buf[256];
	int n;

	fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		perror("open_port: Unable to open /dev/ttyACM0 - ");
		return (-1);
	}

	fcntl(fd, F_SETFL, 0);

	struct termios options;
	tcgetattr(fd, &options);
	cfsetspeed(&options, B9600);
	options.c_cflag &= ~CSTOPB;
	options.c_cflag != CLOCAL;
	options.c_cflag != CREAD;
	cfmakeraw(&options);
	tcsetattr(fd, TCSANOW, &options);
	sleep(1);

	while(1) {

		// READING UPTIME OF PI
		FILE *fp_uptime;
		char uptime[20];	

		fp_uptime = popen("/home/mohit/scripts/uptime.sh", "r");
		if (fp_uptime == NULL) {
		    return -1;
		}
		
		fgets(uptime, sizeof(uptime), fp_uptime);	
		printf("%s\n", uptime);
		
		//n = write(fd, "Hello mbed\r", 11);	
		n = write(fd, uptime, 5);
		if (n < 0) {
			perror("Write failed - ");
		}
		// Read the chars from the port if they are there
		sleep(2);
		n = read(fd, buf, 5);
		if (n < 0) {
			perror("Read failed - ");	
		} else if (n == 0) printf("No data on port\n");
		else {
			buf[n] = 0;
			printf("%i bytes read back: %s\n\r", n, buf);
		}
		sleep(1);
		pclose(fp_uptime);

		// READING MEMORY OF SINGLE DRIVE
		FILE *fp_memory;
		char memory[20];

		fp_memory = popen("/home/mohit/scripts/disk.sh", "r");
		if (fp_memory == NULL) {
			return -1;
		}

		fgets(memory, sizeof(memory), fp_memory);
		printf("%s\n", memory);

		n = write(fd, memory, 5);
		if (n < 0) {
			perror("Write failed -");
		}
		n = read(fd, buf, 5);
		if (n < 0) {
			perror("Read failed - ");	
		} else if (n == 0) printf("No data on port\n");
		else {
			buf[n] = 0;
			printf("%i bytes read back: %s\n\r", n, buf);
		}
		sleep(1);
		pclose(fp_memory);

		// READING RAM OF PI
		FILE *fp_ram;
		char ram[20];

		fp_ram = popen("/home/mohit/scripts/ram.sh", "r");
		if (fp_ram == NULL) {
			return -1;
		}

		fgets(ram, sizeof(ram), fp_ram);
		printf("%s\n", ram);

		n = write(fd, ram, 5);
		if (n < 0) {
			perror("Write failed -");
		}
		n = read(fd, buf, 5);
		if (n < 0) {
			perror("Read failed - ");	
		} else if (n == 0) printf("No data on port\n");
		else {
			buf[n] = 0;
			printf("%i bytes read back: %s\n\r", n, buf);
		}
		sleep(1);
		pclose(fp_ram);
		
		// READING CPU OF PI
		FILE *fp_cpu;
		char cpu[20];

		fp_cpu = popen("/home/mohit/scripts/cpu.sh", "r");
		if (fp_cpu == NULL) {
			return -1;
		}

		fgets(cpu, sizeof(cpu), fp_cpu);
		printf("%s\n", cpu);

		n = write(fd, cpu, 5);
		if (n < 0) {
			perror("Write failed -");
		}
		n = read(fd, buf, 5);
		if (n < 0) {
			perror("Read failed - ");	
		} else if (n == 0) printf("No data on port\n");
		else {
			buf[n] = 0;
			printf("%i bytes read back: %s\n\r", n, buf);
		}
		sleep(1);
		pclose(fp_cpu);

		// READING TEMP OF PI
		FILE *fp_temp;
		char temp[20];

		fp_temp = popen("/home/mohit/scripts/temp.sh", "r");
		if (fp_temp == NULL) {
			return -1;
		}

		fgets(temp, sizeof(temp), fp_temp);
		printf("%s\n", temp);

		n = write(fd, temp, 5);
		if (n < 0) {
			perror("Write failed -");
		}
		n = read(fd, buf, 5);
		if (n < 0) {
			perror("Read failed - ");	
		} else if (n == 0) printf("No data on port\n");
		else {
			buf[n] = 0;
			printf("%i bytes read back: %s\n\r", n, buf);
		}
		sleep(1);
		pclose(fp_temp);

		// End of sends
		sleep(5);
	}

	/*FILE *fp;
	char uptime[20];	

	fp = popen("../uptime.sh", "r");
	if (fp == NULL) {
	    return -1;
	}
	
	fgets(uptime, sizeof(uptime), fp);	
	printf("%s\n", uptime);
	pclose(fp);

	int y = 0;
	char str[2];
	while(uptimeFormatted[y] != '\0') {	
		str[0] = uptimeFormatted[y];
		str[1] = '\0';
		n = write(fd, str, 1);
		/**sleep(2);
		n = read(fd, buf, 1);
		printf("%c", buf[0]);
		***
		sleep(1);
		y++;
	}
	//n = write(fd, uptimeFormatted, 5);	
	if (n < 0) {
	    perror("Write failed - ");
	    return -1;
	}

	sleep(2);
	n = read(fd, buf, 4);
	sleep(1);
	buf[4] = '\0';
	printf("%s", buf);

	tcdrain(fd);
	close(fd);*/

	return 0;
}
