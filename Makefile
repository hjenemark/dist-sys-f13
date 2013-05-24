# Add new files as the pattern goes.

CC=gcc
CFLAGS = -Wall
OFLAGS = -c
LFLAGS = -lpthread
PROJ = sensor
BIN = sensor user 
SENOBJS = sensor.o temp_th.o admin_th.o data_th.o common.o sense_db.o sens_list.o msg_list.o
UIOBJS = user.o common.o msg_list.o

all: user sensor

# Target for user interface
user: $(UIOBJS)
	$(CC) $(CFLAGS) $(UIOBJS) -o user $(LFLAGS)

user.o: user.c
	$(CC) $(CFLAGS) $(OFLAGS) user.c -o user.o

## Targets for sensor node
sensor: $(SENOBJS)
	$(CC) $(CFLAGS) $(SENOBJS) -o $(PROJ) $(LFLAGS)

node_list.o: node_list.c node_list.h
	$(CC) $(CFLAGS) $(OFLAGS) node_list.c -o node_list.o

sense_db.o: sense_db.c
	$(CC) $(CFLAGS) $(OFLAGS) sense_db.c -o sense_db.o

sens_list.o: sens_list.c
	$(CC) $(CFLAGS) $(OFLAGS) sens_list.c -o sens_list.o

msg_list.o: msg_list.c
	$(CC) $(CFLAGS) $(OFLAGS) msg_list.c -o msg_list.o

sensor.o: sensor.c sensor.h
	$(CC) $(CFLAGS) $(OFLAGS) sensor.c -o sensor.o
	
temp_th.o: temp_th.c temp_th.c
	$(CC) $(CFLAGS) $(OFLAGS) temp_th.c -o temp_th.o

admin_th.o: admin_th.c admin_th.h
	$(CC) $(CFLAGS) $(OFLAGS) admin_th.c -o admin_th.o

data_th.o: data_th.c data_th.h
	$(CC) $(CFLAGS) $(OFLAGS) data_th.c -o data_th.o

common.o: common.c common.h
	$(CC) $(CFLAGS) $(OFLAGS) common.c -o common.o

memcheck: $(PROJ)
	valgrind --leak-check=yes --track-origins=yes ./server
	
run: $(PROJ)
	./$(PROJ)

clean: 
	rm $(BIN) *.o

dox:
	doxygen distDoxy
