PROJECT = activity

CC = gcc
CFLAGS = -std=c99 -g -O1 -fno-inline -static

# We love deprecated functions
CFLAGS += -Wno-deprecated-declarations

# Disable stack canary
CFLAGS += -fno-stack-protector

# Disable non-executable stack
CFLAGS += -z execstack

# Disable glibc overflow check in gets()
CFLAGS += -U_FORTIFY_SOURCE

.PHONY: all
all: $(PROJECT)

activity: activity.o support.o support_asm.o

.PHONY: clean
clean:
	rm -rf $(PROJECT) *.o
