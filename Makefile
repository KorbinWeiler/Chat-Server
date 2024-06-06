EXTRA_CFLAGS += -g -Wall
#APP_EXTRA_FLAGS:= -O2 -ansi -pedantic
LDFLAGS = -lpthread
G++:=g++
RM:=rm

all: server client

server.o: server.cpp server.h
	$(G++) $(EXTRA_CFLAGS) -c server.cpp

server: server.o
	$(G++) $(EXTRA_CFLAGS) server.o -o server $(LDFLAGS)

client.o: client.cpp client.h
	$(G++) $(EXTRA_CFLAGS) -c client.cpp

client: client.o
	$(G++) $(EXTRA_CFLAGS) client.o -o client $(LDFLAGS)

clean: server.o client.o
	$(RM) -f *~ *.o server client core *.tar *.zip *.gzip *.bzip *.gz