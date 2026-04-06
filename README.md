# TCP Client/Server in C

A simple TCP client/server chat application built in C using POSIX socket APIs.

## How it works
- The server binds to port 8080, listens for connections, and echoes back any message it receives
- The client connects to the server, sends user input, and prints the server's response
- Type `quit` in the client to disconnect cleanly

## How to compile and run

### Server
gcc -Wall -Wextra -o server server.c
./server

### Client (in a second terminal)
gcc -Wall -Wextra -o client client.c
./client
