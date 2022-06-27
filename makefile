NAME	= webserv
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98

SRC 	= Config.cpp exec_server.cpp Request_parser.cpp Response.cpp Request.cpp\
		  Server.cpp set_html_file.cpp utils.cpp webserv.cpp Cgi.cpp Client.cpp\

SRCDIR	= ./srcs/
SRCS	= $(addprefix $(SRCDIR), $(SRC))
OBJS	= $(SRCS:.cpp=.o)
INCDIR	= ./includes/

%.o:	%.cpp
			c++ $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

all: $(NAME)
	mkdir -p ./www
	mkdir -p ./cgi-bin/uploads

$(NAME): $(OBJS)
	c++ $(CXXFLAGS) -I$(INCDIR) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)
	$(RM) -r www/
	$(RM) -r cgi-bin/uploads/

re: fclean all
