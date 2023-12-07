# Makefile değişecek

NAME = ircserv
CC = c++ -g
FLAGS = -Wall -Wextra -Werror -std=c++98 

SOURCES_DIRECTORY = ./
SOURCES		= $(wildcard $(SOURCES_DIRECTORY)/*.cpp) 

OBJECTS_DIRECTORY = objects
OBJECTS		= $(addprefix $(OBJECTS_DIRECTORY)/, $(notdir $(SOURCES:%.cpp=%.o)))

$(OBJECTS_DIRECTORY)/%.o: %.cpp | $(OBJECTS_DIRECTORY)
	$(CC) $(FLAGS) -c $< -o $@

$(NAME) : $(OBJECTS_DIRECTORY) $(OBJECTS)
	$(CC) $(FLAGS) $(SOURCES) -o $(NAME)

all: $(NAME)

clean:
	@rm -rf $(OBJECTS_DIRECTORY) ircserv.dSYM
	@mkdir objects
 

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: re clean fclean all	