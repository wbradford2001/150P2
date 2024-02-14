# Target library
lib := libuthread.a

all: $(lib)

objects := queue.o
## TODO: Phase 1
# libuthread.a: $(objs)
# 	$(CC) $(CFLAGS) -o $@ $^


libuthread.a: queue.o uthread.o context.o sem.o
	ar rcs libuthread.a queue.o uthread.o context.o preempt.o sem.o
	
sem.o: sem.c sem.c
	gcc -O -c sem.c sem.h

uthread.o: uthread.c uthread.h context.o private.h preempt.o queue.o queue.h
	gcc -O -c uthread.c context.o preempt.o queue.o

preempt.o: preempt.c private.h
	gcc -O -c preempt.c private.h

context.o: context.c private.h
	gcc -O -c context.c private.h

queue.o: queue.c queue.h
	gcc -O -c queue.c
# %.o: %.c
# 	$(CC) $(CFLAGS) -c -o $@ $<
