
##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Makefile
##

rwildc = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildc,$d/,$2))

SOURCEDIR = src

SRC = $(call rwildc,$(SOURCEDIR),*.cpp)

OBJ = $(SRC:.c=.o)

CXX = g++

NAME = plazza

CFLAGS = -Wall -Wextra -I ./include -std=c++20

all: $(NAME)

$(NAME):   $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CFLAGS) $(INCLUDE) $(LIB)

clean:
	rm -f $(OBJ)

fclean:    clean
	rm -f $(NAME)

re:        fclean all
