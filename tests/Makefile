##
## EPITECH PROJECT, __YEAR__
## __BINARY_NAME__
## File description:
## Makefile
##

# rwildc = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildc,$d/,$2))

# SOURCEDIR = ./

# SRC = $(call rwildc,$(SOURCEDIR),*__FILE_TYPE__)

# __COMPILER_TYPE__ = __COMPILER__

# OBJ = $(SRC:__FILE_TYPE__=.o)

# NAME = __BINARY_NAME__

# __COMPILER_TYPE_FLAGS__ = __COMPILER_FLAGS__

# all: $(NAME)

# $(NAME):   $(OBJ)
# 	__COMPILER__ -o $(NAME) $(OBJ) $(__COMPILER_TYPE_FLAGS__)

# tests_run:
# 	cd tests && make && ./tests

# clean:
# 	rm -f $(OBJ)
# 	find . -name "vgcore.*" -delete
# 	find . -name "*~" -delete
# 	find . -name "\#*" -delete

# fclean:    clean
# 	rm -f $(NAME)

# re:        fclean all

all:
	echo "int main() { return 0; }" > main.c && gcc -o tests main.c
