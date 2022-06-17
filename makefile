CC = c++
CFLAGS = -Wall -Wextra -Werror
SRCS = srcs/webserv.cpp srcs/Config.cpp srcs/Server.cpp srcs/exec_server.cpp srcs/get_html.cpp 

all:
	$(CC) $(CFLAGS) $(SRCS) -o server
	./server
