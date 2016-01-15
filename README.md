# 433-utils

433-utils provide simple tools to receive and send commands to the remote controlled switch PRF-100 made by "Home Confort" ([link]("www.home-confort.net/fr/content/prf-100…")).

It has been tested on Raspbian 8.0 (Jessie) and depends on wiringPi to communicate with the gpio.

Note: the gpio pins are hard coded. the receiver should be on pin 2 and the transmitter on pin 0.

## receive

Usage: `./receive output.txt`

receive writes to the output file raw data received by the receiver.

To discover the message sent by a remote control button, run the receive program, push the button, and then ctrl-c to stop the program.

You can then plot the output file, which contains zeros and ones, to find a pattern in the data.

## send

Usage: `./send output.txt`

send the content of a file written by receive through the transmitter.

## sendp

Usage `./sendp 1 0`

The PRF-100 is controlled by the remote TRF-80, which can control up to 4 switches. sendp emulate the remote control, by sending a on/off command to one of the 4 channels.

sendp takes two arguments, the channel number and an on/off command (1=on, 0=off).
