/*
** EPITECH PROJECT, 2022
** Epitech-Template
** File description:
** main.c
*/

#include <string>
#include <iostream>
#include "Reception.hpp"

enum PizzaType
{
    Regina = 1 ,
    Margarita = 2 ,
    Americana = 4 ,
    Fantasia = 8
};

enum PizzaSize
{
    S = 1 ,
    M = 2 ,
    L = 4 ,
    XL = 8 ,
    XXL = 16
};

int main(int ac, char **av)
{
    try {
        Reception reception(ac, av);
        reception.run();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return (84);
    }
    return (0);
}
