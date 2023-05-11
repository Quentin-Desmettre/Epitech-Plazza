
##
## EPITECH PROJECT, 2023
## Makefile
## File description:
## Makefile
##

rwildc = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildc,$d/,$2))

SOURCEDIR = ../src/
SOURCEDIR_2 = ./

SRC_TMP = $(call rwildc,$(SOURCEDIR),*.cpp) $(call rwildc,$(SOURCEDIR_2),*.cpp)
SRC = $(filter-out ../src/main.cpp, $(SRC_TMP))

CXX = g++

NAME = tests

CFLAGS = -Wall -Wextra -I ../include -std=c++20 --coverage -g

all: $(NAME)

$(NAME):
	$(CXX) -o $(NAME) $(SRC) $(CFLAGS) $(INCLUDE) $(LIB)

clean:
	rm -f $(OBJ) || true
	find .. -name "*~" -delete || true
	find .. -name "*.gcno" -delete || true
	find .. -name "*.gcda" -delete || true

fclean:    clean
	rm -f $(NAME)

re:        fclean all
