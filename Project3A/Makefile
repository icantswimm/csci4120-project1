# Makefile for bw_threshold.c

CC = gcc
CFLAGS = -Wall -O2
TARGET = mt_greyscal2blacknwhite
SRC = mt_greyscal2blacknwhite.c

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) -g -lpthread -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
