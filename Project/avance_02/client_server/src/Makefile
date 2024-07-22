# Linux Makefile
# Socket class with SSL
#

CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c 
LDFLAGS := -lssl -lcrypto -pthread -lstdc++fs
DEPS := Socket.h
TARGETS := client.out server.out jsonTest.out

ALL := $(TARGETS)
$(info $$TARGETS = $(TARGETS))

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $<

%.out: %.o Socket.o
	$(info $$^ = $^)
	$(CXX) $^ Socket.o $(LDFLAGS) -o $^

client.out: client.o VSocket.o Socket.o
	$(CXX) client.o VSocket.o Socket.o $(LDFLAGS) -o $@

server.out: server.o VSocket.o Socket.o
	$(CXX) server.o VSocket.o Socket.o $(LDFLAGS) -o $@

jsonTest.out: jsonTest.o
	$(CXX) jsonTest.o $(LDFLAGS) -o $@

clean:
	rm -f *.o *.out