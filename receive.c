#include <wiringPi.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <strings.h>

uint8_t stop = 0;

void sigint_handler(int sig_no) {
	stop = 1;
	printf("CTRL-C pressed\n");
}

int main(int argc, char const *argv[]) {
  int pin = 2;
  FILE *fp;
  struct sigaction action;


  bzero(&action, sizeof(action));
  action.sa_handler = &sigint_handler;
  sigaction(SIGINT, &action, NULL);

  if (wiringPiSetup() == -1) {
    printf("Could not initialize wiringPi\n");
		return -1;
	}

  // Open file
  printf("Writing to %s\n", argv[1]);
	fp = fopen(argv[1], "w");
  if (fp == NULL) {
    perror("Error while opening the file");
    return -1;
  }

  // Receive the data
  pinMode(pin, INPUT);
  do {
    int pin_value = digitalRead(pin);
    fprintf(fp, "%u\n", pin_value);
    delayMicroseconds(100);
  } while (!stop);

  fclose(fp);
  return 0;
}
