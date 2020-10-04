TARGET=all.out
OBJS=main.o
TOPDIR=${PWD}

CFLAGS=-I${TOPDIR}/inc

all:$(TARGET)

$(TARGET):$(OBJS)
	g++ $^ -o $@

%.o:src/%.c

test.o:test.c

clean:
	rm -rf ${OBJS} ${TARGETS}
