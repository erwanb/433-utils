CC=gcc
CFLAGS= -std=gnu99
LIBS= -lwiringPi

SEND_SRC= send.c
SEND_OBJ= $(SEND_SRC:.c=.o)

SENDP_SRC= sendp.c
SENDP_OBJ= $(SENDP_SRC:.c=.o)

RECEIVE_SRC= receive.c
RECEIVE_OBJ= $(RECEIVE_SRC:.c=.o)

all: clean send receive sendp

send: $(SEND_OBJ)
	$(CC) $(LIBS) -o $@ $^

receive: $(RECEIVE_OBJ)
	$(CC) $(LIBS) -o $@ $^

sendp: $(SENDP_OBJ)
	$(CC) $(LIBS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -Rf send sendp receive *.o
