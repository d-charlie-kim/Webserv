NAME	= Webserv
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98

SRC 	= webserv.cpp Config.cpp

SRCDIR	= ./srcs/
SRCS	= $(addprefix $(SRCDIR), $(SRC))
OBJS	= $(SRCS:.cpp=.o)
INCDIR	= ./includes/

%.o:	%.cpp
			c++ $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

$(NAME): $(OBJS)
	c++ $(CXXFLAGS) -I$(INCDIR) -o $(NAME) $(OBJS)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
