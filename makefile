EXE = sensor_gateway
OBJS = main.o Storagemgr.o Datamgr.o Logmgr.o tcpsocket.o myqueue.o liblist.so
CC =  gcc -std=c99
CCL = gcc -std=c99 -L. -Wl,-rpath=.
CFLAGS = -Wall -c
LFLAGS = -Wall 
THREAD = -pthread
MYSQL = $$(mysql_config --cflags --libs)

INCLUDE=/usr/include/mysql
LIBS=/usr/lib/mysql/plugin


$(EXE):$(OBJS)
	@echo "\n***** LINKING  $(OBJS)*****"
	$(CCL) $(LFLAGS) $(THREAD) $(OBJS) -o $(EXE) $(MYSQL)

main.o:main.c
	$(CC) $(CFLAGS) main.c $(MYSQL)
	
Storagemgr.o:Storagemgr.c
	$(CC) $(CFLAGS) Storagemgr.c $(MYSQL)
	
Datamgr.o:Datamgr.c 
	$(CC) $(CFLAGS) Datamgr.c
	
Logmgr.o:Logmgr.c 
	$(CC) $(CFLAGS) Logmgr.c
	
tcpsocket.o:tcpsocket.c 
	$(CC) $(CFLAGS) tcpsocket.c
	
myqueue.o:myqueue.c 
	$(CC) $(CFLAGS) myqueue.c
	
sensor_node:sensor_node.c
	$(CC) sensor_node.c tcpsocket.c -o sensor_node
	
clean:
	rm *.o $(EXE)
	
	
ts1:
	./sensor_node 1 1 127.0.0.1 1234
ts2:
	./sensor_node 2 2 127.0.0.1 1234
ts3:
	./sensor_node 3 1 127.0.0.1 1234
ts4:
	./sensor_node 4 2 127.0.0.1 1234
ts5:
	./sensor_node 5 1 127.0.0.1 1234
ts6:
	./sensor_node 6 2 127.0.0.1 1234
ts7:
	./sensor_node 7 1 127.0.0.1 1234
ts8:
	./sensor_node 8 2 127.0.0.1 1234