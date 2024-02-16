# Target library
lib := libuthread.a

ifneq ($(V),1)
Q=@
endif



all: $(lib)

objects := queue.o uthread.o context.o sem.o preempt.o

CC := gcc
CFlags := -Wall -Werror -Wextra -MMD


#dependency tracking
deps := $(patsubst %.o, %.d, $(objects))
-include $(deps)


libuthread.a: $(objects)
	@echo "CC $@"
	$(Q)ar rcs $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFlags) -O -c $@ $<

clean: 
	$(Q)rm -f $(lib) $(objects) $(deps)


