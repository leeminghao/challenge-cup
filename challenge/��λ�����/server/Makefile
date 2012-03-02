#CC = gcc
CC = /usr/local/arm/3.4.1/bin/arm-linux-gcc
CFLAGS = -Wall
OBJECTS = lmh_server.o lmh_socket.o lmh_v4l.o \
	lmh_err.o lmh_temphum.o

server : $(OBJECTS)
	$(CC) $(CFLAGS) -o server $(OBJECTS) -static -lm

lmh_server.o : lmh_server.c lmh_socket.c lmh_socket.h \
	lmh_v4l.c lmh_v4l.h lmh_err.c lmh_err.h \
	lmh_temphum.c lmh_temphum.h

	$(CC) $(CFLAGS) -c lmh_server.c lmh_socket.c lmh_v4l.c lmh_err.c lmh_temphum.c

lmh_socket.o : lmh_socket.c lmh_socket.h lmh_err.c lmh_err.h

	$(CC) $(CFLAGS) -c lmh_socket.c lmh_err.c

lmh_v4l.o : lmh_v4l.c lmh_v4l.h lmh_err.c lmh_err.h

	$(CC) $(CFLAGS) -c lmh_v4l.c

lmh_err.o : lmh_err.c lmh_err.h
	$(CC) $(CFLAGS) -c lmh_err.c

lmh_temphum.o : lmh_temphum.c lmh_temphum.h \
	lmh_err.c lmh_err.h
	$(CC) $(CFLAGS) -c lmh_temphum.c lmh_err.c

.PHONY: cleanall cleanobj cleanexe
cleanall: cleanobj cleanexe

cleanobj:
	$(RM) *.o

cleanexe:
	$(RM) server