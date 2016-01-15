#include <wiringPi.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  int pin = 0;
  FILE *fp;
  int buffer[512];

  if (wiringPiSetup() == -1) {
    printf("Could not initialize wiringPi\n");
		return -1;
	}

  // Open file
  printf("Sending %s\n", argv[1]);
	fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror("Error while opening the file");
    return -1;
  }

  // Read the data from file
  int pin_value;
  int c = 0;
  while (fscanf(fp, "%u\n", &pin_value) == 1) {
    buffer[c++] = pin_value;
  }
  fclose(fp);

  // Send the data
  pinMode(pin, OUTPUT);
  for (int r = 0; r < 10; r++) {
    for (int i = 0; i < c; i++) {
      digitalWrite(pin, buffer[i]);
      delayMicroseconds(100);
    }
  }

  return 0;
}
