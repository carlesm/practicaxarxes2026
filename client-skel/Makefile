CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11

# TODO: quan hagueu creat protocol.c i udp.c, afegiu-los a OBJS:
#   OBJS = main.o tap.o protocol.o udp.o
OBJS    = main.o tap.o

TARGET  = vpnclient

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
