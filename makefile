CC = c++
CFLAGS = -Wall -Wextra -Werror
SRCS = srcs/webserv.cpp srcs/Config.cpp srcs/Server.cpp

all:
	$(CC) $(CFLAGS) $(SRCS) -o server
	./server
