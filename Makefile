name = server
client = client
compiler = g++ -g
flags = -Wall -Wextra -Werror
src = 



all:

clean: 

fclean: clean

re: fclean all


.PHONY: all clean fclean re