// PI Network Attached Storage System - mbed code

#include "mbed.h"
#include "uLCD_4DGL.h"
#include "rtos.h"
#include <cstdio>

uLCD_4DGL uLCD(p9,p10,p11); // serial tx, serial rx, reset pin;
RawSerial  pi(USBTX, USBRX);    // For serial communication with PI.

// Metrics
volatile int memory_used;
volatile int upkeep_time;
volatile int problem_code;
volatile int ram_free;
volatile int cpu_usage;
volatile int hosts_connected;
volatile int temperature;

// LCD Lock
Mutex lcd_lock;

// Updates the memory free on /dev/sda1 for the PI (the first plugged in USB flashdrive/external drive) on LCD.
void lcd_memory_update() {
    while (1) {
        lcd_lock.lock();
        uLCD.locate(1, 4);
        uLCD.printf("Disk Free: %3.0d%%", (100 - memory_used));
        lcd_lock.unlock();

        Thread::wait(1000);
    }
}

// Updates the upkeep time of the Raspberry PI on LCD.
void lcd_upkeep_update() {
    while (1) {
        lcd_lock.lock();
        uLCD.locate(1, 7);
        uLCD.color(WHITE);
        uLCD.printf("Uptime: %3.0d Min", upkeep_time);
        lcd_lock.unlock();

        Thread::wait(1250);
    }
}

// Updates the ram usage on LCD.
void lcd_ram_usage() {
    while (1) {
        lcd_lock.lock();
        uLCD.locate(1, 10);
        uLCD.printf("RAM Free: %3.0d%%", ram_free);
        lcd_lock.unlock();

        Thread::wait(1500);
    }
}

// Updates the CPU usage on LCD.
void lcd_cpu_usage() {
    while (1) {
        lcd_lock.lock();
        uLCD.locate(1, 12);
        uLCD.printf("CPU Usage: %2.0d%%", cpu_usage);
        lcd_lock.unlock();

        Thread::wait(1750);
    }
}

// Updates the temperature of PI on LCD
void lcd_temp_update() {
    while (1) {
        lcd_lock.lock();
        uLCD.locate(15, 0);
        uLCD.printf("%2.0dC", temperature);
        lcd_lock.unlock();

        Thread::wait(2000);
    }
}

// Updates the status code on LCD if there is any issues.
void lcd_problem_update() {
    while (1) {
        lcd_lock.lock();
        uLCD.locate(1, 15);
        if (problem_code) {
            uLCD.printf("Status Code: %2.0d", problem_code);
        } else {
            uLCD.printf("Status Code: 00");
        }
        lcd_lock.unlock();

        Thread::wait(2250);
    }
}

int main()
{
    // Initialize metrics
    memory_used = 0;
    upkeep_time = 0;
    problem_code = 0;
    ram_free = 0;
    cpu_usage = 0;

    // Setup baudrate for pi.
    pi.baud(9600);

    // Init LCD title graphic.
    uLCD.cls();
    uLCD.locate(0,0);
    uLCD.background_color(BLACK);
    uLCD.color(WHITE);
    uLCD.printf("PI NAS SYSTEM|");

    // Startup threads for drawing to LCD
    Thread memoryThread;
    memoryThread.start(lcd_memory_update);

    Thread upkeepThread;
    upkeepThread.start(lcd_upkeep_update);

    Thread problemThread;
    problemThread.start(lcd_problem_update);
    
    Thread ramThread;
    ramThread.start(lcd_ram_usage);

    Thread cpuThread;
    cpuThread.start(lcd_cpu_usage);

    Thread tempThread;
    tempThread.start(lcd_temp_update);

    // Main loop, grab formatted data from the PI, and update the LCD based off of this data.
    while (1) {
        // Check if rebooting
        if (!memory_used && !upkeep_time && !ram_free && !cpu_usage) {
            problem_code = 1;
        } else {
            problem_code = 0;
        }

        // Check if any metrics are not being sent.
        if (!memory_used && upkeep_time && ram_free && cpu_usage) {
            problem_code = 2;
        } else if (memory_used && !upkeep_time && ram_free && cpu_usage) {
            problem_code = 3;
        } else if (memory_used && upkeep_time && !ram_free && cpu_usage) {
            problem_code = 4;
        } else if (memory_used && upkeep_time && ram_free && !cpu_usage) {
            problem_code = 4;
        } else {
            problem_code = 0;
        }

        char temp;
        char update_string[20];
        int update_val = 0;
        int i = 0;

        // Necessary incase pi.readable() is broken out of in the middle of data being sent from pi.
        int ready_post = 0;

        // Read in metric data from the PI.
        if (!pi.readable()) {
            Thread::yield();
        }
        while (pi.readable()) {
            temp = pi.getc();
            update_string[i] = temp;
            i++;
            if (temp == '/') {
                ready_post = 1;
            }
        }

        update_string[5] = '\0';
        pi.puts(update_string);

        // Switch on the initial character read from the PI.
        // Depending on this character, we choose to update a different metric.
        // I.E. Converts "u44/" into 44 to be stored into a volatile int
        switch(update_string[0]) {
            case 'u': {
                char num[20];
                int y = 1;
                if (ready_post) {
                    while(update_string[y] != '/') {
                        num[y - 1] = update_string[y];
                        y++;
                    }
                    num[y - 1] = '\0';
                    upkeep_time = atoi(num);
                    ready_post = 0;
                }
                break;
            }
            case 'm': {
                char num[20];
                int y = 1;
                if (ready_post) {
                    while(update_string[y] != '/') {
                        num[y - 1] = update_string[y];
                        y++;
                    }
                    num[y - 1] = '\0';
                    memory_used = atoi(num);
                    ready_post = 0;
                }
                break;
            }
            case 'r': {
                char num[20];
                int y = 1;
                if (ready_post) {
                    while(update_string[y] != '/') {
                        num[y - 1] = update_string[y];
                        y++;
                    }
                    num[y - 1] = '\0';
                    ram_free = atoi(num);
                    ready_post = 0;
                }
                break;
            }
            case 'c': {
                char num[20];
                int y = 1;
                if (ready_post) {
                    while(update_string[y] != '/') {
                        num[y - 1] = update_string[y];
                        y++;
                    }
                    num[y - 1] = '\0';
                    cpu_usage = atoi(num);
                    ready_post = 0;
                }
                break;
            }
            case 't': {
                char num[20];
                int y = 1;
                if (ready_post) {
                    while(update_string[y] != '/') {
                        num[y - 1] = update_string[y];
                        y++;
                    }
                    num[y - 1] = '\0';
                    temperature = atoi(num);
                    ready_post = 0;
                }
                break;
            }
        }
    }
}



