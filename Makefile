CC=g++
CFLAGS=-Wall -pedantic-errors

LIBS=-pthread -lncurses -lcrypto 

server: chat_server.cpp fatal_error.cpp sockets.cpp encryption.cpp
	$(CC) -o server $(CFLAGS) $(LIBS)chat_server.cpp fatal_error.cpp sockets.cpp encryption.cpp

client: chat_client.cpp fatal_error.cpp sockets.cpp encryption.cpp
	$(CC) -o client $(CFLAGS) $(LIBS)chat_client.cpp fatal_error.cpp sockets.cpp encryption.cpp
