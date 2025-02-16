NAME = chip8

SRC = $(addprefix src/, chip8.c print.c init.c utils.c decode_and_execute.c display.c)

OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

LDFLAGS = -lSDL2  # Add this to link the SDL2 library

OBJECTS_DIR = obj

INC_DIR = inc

CFLAGS =  -Wall -Werror -Wextra -I $(INC_DIR) 

all: $(NAME) 

$(NAME) : $(OBJ) $(INC_DIR)/chip8.h 
	cc $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

obj/%.o : src/%.c $(INC_DIR)/chip8.h | $(OBJECTS_DIR)
	cc -c $(CFLAGS) -c $< -o $@

$(OBJECTS_DIR) :
	mkdir -p obj

clean:
	@rm -rf obj

fclean: clean
	@rm -f $(NAME) 

re: fclean all

.PHONY: clean all bonus clean fclean re