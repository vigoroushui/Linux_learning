SOURCES:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SOURCES))
ELF:=main
CC:=gcc
CFLAGS:=-g -Wall
$(ELF):$(OBJS)
	gcc $^ -o $@
.PHONY:clean
clean:
	rm -rf $(ELF) $(OBJS)

