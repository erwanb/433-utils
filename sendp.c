#include <wiringPi.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// A segment can contains up to 32 bits of data, and the siz of data in bits to be processed
typedef struct segment_s {
  uint8_t size;
  uint32_t data;
} segment_t;

// commands are to be converted to 0=code_zero & 1=code_one
const segment_t cmd_part1      = {22, 0x22ca1d}; // 10111000 01010011 010001
const segment_t cmd_part2      = {16, 0xffff};   // 11111111 11111111
const segment_t cmd_part3      = {7,  0x3f};     // 1111110
const segment_t cmd_on         = {1,  0x0};      // 0
const segment_t cmd_off        = {1,  0x1};      // 1
const segment_t cmd_channels[] = {{2, 0x3},      // 11
                                  {2, 0x1},      // 10
                                  {2, 0x2},      // 01
                                  {2, 0x0}};     // 00

// codes are sent through the wire as is
const segment_t msg_start = {22, 0x3ffffe}; // signal: _---------------------
const segment_t msg_end   = {13, 0xc};      // signal: __--______________
const segment_t code_zero = {8,  0xC0};     // signal: ______--
const segment_t code_one  = {8,  0xfc};     // signal: __------

// A message is composed of:
// [msg_start][cmd_part1][cmd_channel][cmd_part2][cmd_on/cmd_off][cmd_part3][msg_end]

void process_segment(uint8_t pin, segment_t segment, void (*callback)(uint8_t, uint32_t)) {
  for (uint8_t i = 0; i < segment.size; i++) {
    (*callback)(pin, segment.data);
    segment.data = segment.data >> 1;
  }
}

void send_code(uint8_t pin, uint32_t val) {
  digitalWrite(pin, val & 0x01);
  delayMicroseconds(100);
}

void send_cmd(uint8_t pin, uint32_t val) {
  process_segment(pin, val & 0x01 ? code_one : code_zero, &send_code);
}

void send_message(uint8_t pin, uint8_t channel, segment_t cmd) {
  for (uint8_t r = 0; r < 10; r++) {
    process_segment(pin, msg_start,             &send_code);
    process_segment(pin, cmd_part1,             &send_cmd);
    process_segment(pin, cmd_channels[channel], &send_cmd);
    process_segment(pin, cmd_part2,             &send_cmd);
    process_segment(pin, cmd,                   &send_cmd);
    process_segment(pin, cmd_part3,             &send_cmd);
    process_segment(pin, msg_end,               &send_code);
  }
}

int main(int argc, char const *argv[]) {
  const uint8_t pin = 0;
  uint8_t channel = atoi(argv[1]) - 1;
  uint8_t cmd = atoi(argv[2]);

  if (wiringPiSetup() == -1) {
    printf("Could not initialize wiringPi\n");
		return -1;
	}

  printf("Switching channel %u %s\n", channel + 1, cmd == 1 ? "on" : "off");
  pinMode(pin, OUTPUT);
  send_message(pin, channel, cmd == 1 ? cmd_on : cmd_off);
  return 0;
}
