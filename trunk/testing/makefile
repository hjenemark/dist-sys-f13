# Add new files as the pattern goes.

CC=gcc
CFLAGS = -Wall -g -O0
OFLAGS = -c
LFLAGS = -lm
PROJ = test
BIN = test
OBJS = test.o msg_list.o common.o sens_list.o

all: $(BIN)

test: $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $(PROJ) $(LFLAGS)

test.o: test.c
	$(CC) $(CFLAGS) $(LFLAGS) $(OFLAGS) test.c -o test.o

msg_list.o: ../msg_list.c
	$(CC) $(CFLAGS) $(OFLAGS) ../msg_list.c -o msg_list.o

common.o: ../common.c
	$(CC) $(CFLAGS) $(OFLAGS) ../common.c -o common.o

sens_list.o: ../sens_list.c
	$(CC) $(CFLAGS) $(OFLAGS) ../sens_list.c -o sens_list.o

run: $(PROJ)
	./$(PROJ)

memcheck: $(PROJ)
	valgrind --leak-check=yes --track-origins=yes ./$(PROJ)
clean: 
	rm $(BIN) *.o

