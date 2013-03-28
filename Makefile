# Add new files as the pattern goes.

CC=gcc
CFLAGS = -Wall
OFLAGS = -c
LFLAGS = -lpthread
PROJ = sensor
OBJS = sensor.o temp_th.o admin_th.o data_th.o common.o node_list.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROJ) $(LFLAGS)

node_list.o: node_list.c node_list.c
	$(CC) $(CFLAGS) $(OFLAGS) node_list.c -o node_list.o

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

server: server.c
	$(CC) $(CFLAGS) -o server server.c

user: user.c
	$(CC) $(CFALGS) -o user user.c common.c
	
run: $(PROJ)
	./$(PROJ)

# Usage: make multi COUNT=<integer>
COUNT=5
multi: $(PROJ)
	./runMultiple.sh $(COUNT) $(PROJ)
clean: 
	rm $(PROJ) *.o

# Don't add the resulting html and latex folders to the repository
# They can be produced by this makefile on demand.
dox:
	doxygen distDoxy


#	gcc -pthread -o sensor sensor.c temp_th.c admin_th.c data_th.c common.c
