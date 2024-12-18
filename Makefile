RED		= '\033[31m'
GREEN	= '\033[1;32m'
YELLOW	= '\033[33m'
BLUE	= '\033[34m'
PURPLE	= '\033[1;35m'
CIAN	= '\033[36m'
WHITE	= '\033[37m'
NONE	= '\033[0m'

NAME = webserv

SRCS_PATH = src/
INCS_PATH = inc/
BIN_PATH = bin/

SRCS =	main.cpp \
		Location.cpp \
		Server.cpp \
		Parser.cpp \
		Response.cpp \
		Request.cpp \
		SocketManager.cpp \
		CGI.cpp

OBJS = $(SRCS:%.c=bin/%.o)

CC = g++
CFLAGS = -std=c++98 -Wall -Werror -Wextra -g3 -O0 #-fsanitize=address
RM = rm -f

OBJS = $(SRCS:%.cpp=$(BIN_PATH)%.o)

###	--- RULES --- ###

all: $(NAME)

$(BIN_PATH)%.o: $(SRCS_PATH)%.cpp
	mkdir -p $(BIN_PATH)
	$(CC) $(CFLAGS) -c $< -o $@

	
$(NAME): $(OBJS)
	echo $(PURPLE)"[Creating $(NAME)]"$(NONE)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	echo $(GREEN)"$(NAME): ready to be executed"$(NONE)

clean:
	$(RM) $(OBJS)
	rm -rf bin/
	echo $(RED)"[Object Files Deleted]"$(NONE)

fclean: clean
	$(RM) $(NAME)
	echo $(RED)"[Executable File Deleted]"$(NONE)

re: fclean #$(NAME)
	clear
	$(MAKE)	
	
.PHONY: all clean fclean re $(NAME)

.SILENT: all clean fclean re $(NAME)