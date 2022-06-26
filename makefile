NAME		=	webserv
CXX			=	c++
CXXFLAGS	=	-I$(INCDIR) -Wall -Wextra -Werror -std=c++98
INCDIR		=	includes
SRCDIR		=	srcs/

SRC		=	Config.cpp\
			exec_server.cpp\
			Request_parser.cpp\
			Server.cpp\
			webserv.cpp\
			Response.cpp\
			set_html_file.cpp\
			utils.cpp\

SRCS	=	$(addprefix $(SRCDIR), $(SRC))

OBJS 	=	$(SRCS:.cpp=.o)

$(NAME)	:	$(OBJS)
		$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

RM 		=	rm -rf

all	:	$(NAME)

clean	:
		$(RM) $(OBJS)

fclean	:	clean
		$(RM) $(NAME)

re		:
		make fclean
		make all

.PHONY	: all clean fclean re