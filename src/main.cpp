/*
** EPITECH PROJECT, 2022
** Epitech-Template
** File description:
** main.c
*/

#include <string>
#include <iostream>
#include "Reception.hpp"

int main(int ac, char **av)
{
//    try {
        Reception reception(ac, av);
        reception.run();
//    } catch (std::exception &e) {
//        std::cout << e.what() << std::endl;
//        return (84);
//    }
    return (0);
}
