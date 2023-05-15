
##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Makefile
##

rwildc = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildc,$d/,$2))

SOURCEDIR = src

SRC = $(call rwildc,$(SOURCEDIR),*.cpp)

OBJ = $(SRC:.cpp=.o)

CXX = g++

NAME = plazza

CXXFLAGS = -Wall -Wextra -I ./include -std=c++20

all: $(NAME)

$(NAME):   $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(CXXFLAGS) $(INCLUDE) $(LIB)

tests_run:
	make -C tests
	cd tests && ./tests ; rm tests

clean:
	rm -f $(OBJ)

fclean:    clean
	make -C tests fclean
	rm -f $(NAME)

re:        fclean all
