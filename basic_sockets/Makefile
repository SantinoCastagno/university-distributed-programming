CC=gcc
CFLAGS=

SRCS=$(wildcard *.c)
EXECS=$(SRCS:.c=)

.PHONY: all clean

all: $(EXECS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXECS)
