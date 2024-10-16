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

BIN_PATH = bin/

SRCS =	main.cpp \
				AConfig.cpp \
				WebServer.cpp \
				Server.cpp \
				Location.cpp \
				SLocaSearch.cpp \

OBJS = $(SRCS:%.cpp=bin/%.o)

CC = g++
CFLAGS = -std=c++98 -Wall -Werror -Wextra -g3
RM = rm -f

###	--- RULES --- ###

all: $(NAME)

$(BIN_PATH)%.o: $(SRCS_PATH)%.cpp
	mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

	
$(NAME): $(OBJS)
	echo $(PURPLE)"[Creating $(NAME)]"$(NONE)
	$(CC) ${CFLAGS} -o $(NAME) $(OBJS)
	echo $(GREEN)"$(NAME): ready to be executed"$(NONE)

clean:
	$(RM) $(OBJS)
	rm -rf $(BIN_PATH)
	echo $(RED)"[Object Files Deleted]"$(NONE)

fclean: clean
	$(RM) $(NAME)
	echo $(RED)"[Executable File Deleted]"$(NONE)

re: fclean
	$(MAKE)	

run: re
	./${NAME}

.PHONY: all clean fclean re

.SILENT:
