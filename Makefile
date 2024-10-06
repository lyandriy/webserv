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

SRCS =	src/response/main.cpp \
		src/response/Location.cpp \
		src/response/Server.cpp \
		src/response/Parser.cpp \
		src/response/Response.cpp \
		src/response/Request.cpp \
		src/response/SocketManager.cpp \

# Generar los archivos objeto respetando la estructura de directorios
OBJS = $(SRCS:$(SRCS_PATH)%.cpp=$(BIN_PATH)%.o)

CC = g++
CFLAGS = -std=c++98 -Wall -Werror -Wextra -g -O0
RM = rm -f

### --- RULES --- ###

all: $(NAME)

# Regla para compilar los archivos fuente en archivos objeto
$(BIN_PATH)%.o: $(SRCS_PATH)%.cpp
	# Crear la estructura de directorios según sea necesario
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para generar el ejecutable
$(NAME): $(OBJS) #$(INCS_PATH)/webserv.hpp
	echo $(PURPLE)"[Creating $(NAME)]"$(NONE)
	$(CC) -o $(NAME) $(OBJS)
	echo $(GREEN)"$(NAME): ready to be executed"$(NONE)

# Limpiar archivos objeto
clean:
	$(RM) $(OBJS)
	rm -rf $(BIN_PATH)
	echo $(RED)"[Object Files Deleted]"$(NONE)

# Limpiar todo, incluyendo el ejecutable
fclean: clean
	$(RM) $(NAME)
	echo $(RED)"[Executable File Deleted]"$(NONE)

# Regenerar todo desde cero
re: fclean
	$(MAKE)

# Regla para ejecutar
run: all
	./$(NAME)

.PHONY: all clean fclean re

.SILENT:
