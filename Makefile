TARGET=gitwrap

GIT_BINARY_PATH=/usr/bin/git

CFLAGS=-std=c99 -pedantic -Wall -Werror
CPPFLAGS=-D_XOPEN_SOURCE=600 -DGIT_BINARY_PATH=\"$(GIT_BINARY_PATH)\"
LDFLAGS=

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFALAGS) $(OBJ) -o $(TARGET)

clean:
	rm -rf  $(TARGET) $(OBJ)

.PHONY: all clean
